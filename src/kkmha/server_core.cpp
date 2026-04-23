// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_core.h"
#include "server_variables.h"
#include "server_strings.h"
#include "server_except.h"
#include "server_logger.h"
#include "server_cache_core.h"
#include "server_counter.h"
#include "server_default_handler.h"
#include "server_kkmop_handler.h"
#include "server_static_handler.h"
#include "server_config_handler.h"
#include "server_ping_handler.h"
#include "http_parser.h"
#include "http_request.h"
#include <constants.h>
#include <cassert>
#include <utility>
#include <memory>
#include <atomic>
#include <mutex>
#include <latch>
#include <thread>
#include <chrono>

namespace Server {
    using namespace std::chrono_literals;
    using namespace std::string_view_literals;

    static_assert(c_controlTimeout >= c_sleepQuantum);

    enum class State { Initial, Starting, Running, Shutdown, Stopping, Stopped };

    constexpr auto c_dummyJob = [] {};

    static std::atomic s_state { State::Initial };
    static Counter::Type s_concurrentRequestsCounter { 0 };
    static Counter::Type s_delayedSocketsCounter { 0 };
    static std::mutex s_hitmanJobMutex {};
    static std::function s_hitmanJob { c_dummyJob };
    static std::latch s_shutdownSync { 2 };

    static Default::Handler s_defaultHandler {};
    static KkmOp::Handler s_kkmHandler {};
    static Static::Handler s_staticHandler {};
    static Config::Handler s_configHandler {};
    static Ping::Handler s_pingHandler {};

    void emergencyBrake(const auto & e, const State state) noexcept {
        assert(state >= State::Shutdown);

        log(Log::Level::Error, e);

        switch (s_state.load()) {
            case State::Initial:
            case State::Starting:
                log(Log::Level::Error, Wcs::c_startingFailed);
                break;
            case State::Shutdown:
            case State::Stopping:
            case State::Stopped:
                log(Log::Level::Error, Wcs::c_stoppingFailed);
                break;
            default:
                log(Log::Level::Error, Wcs::c_servicingFailed);
        }

        s_state.store(state);
    }

    void completeHitmanJob(const auto & e) noexcept {
        emergencyBrake(e, State::Stopping);

        {
            std::scoped_lock lock { s_hitmanJobMutex };
            std::invoke(s_hitmanJob);
            s_hitmanJob = c_dummyJob;
        }

        s_shutdownSync.count_down();
    }

    void cancelHitmanJob(const auto & e) noexcept {
        {
            std::scoped_lock lock { s_hitmanJobMutex };
            s_hitmanJob = c_dummyJob;
        }

        emergencyBrake(e, State::Stopping);
        s_shutdownSync.count_down();
    }

    [[nodiscard]]
    ProtoHandler & lookupHandler(const Http::Request & request) {
        // ISSUE: При большем количестве обработчиков стоит оптимизировать.
        if (request.m_hint.size() >= 2) {
            const auto area = request.m_hint[1];
            if (area == "kkm") {
                return s_kkmHandler;
            }
            if (area == "static") {
                return s_staticHandler;
            }
            if (area == "config") {
                return s_configHandler;
            }
            if (area == "ping") {
                return s_pingHandler;
            }
        }
        return s_defaultHandler;
    }

    template<typename CompletionToken>
    auto performAsync(const ProtoHandler & handler, Http::Request & request, CompletionToken && token) {
        return
            asio::async_compose<CompletionToken, void()>(
                [& handler, & request] (auto && self) {
                    std::thread(
                        [task = std::move(self), & handler, & request] () mutable { // NOLINT(*-move-forwarding-reference)
                            handler(request);
                            task.complete();
                        }
                    ).detach();
                },
                std::forward<CompletionToken>(token)
            );
    }

    asio::awaitable<void> handle(Asio::TcpSocket && socket, Asio::SslContext & sslContext) noexcept try {
        Counter counter { s_concurrentRequestsCounter };
        Asio::Stream stream { std::move(socket), sslContext };
        auto & streamLowestLayer = stream.lowest_layer();
        Http::Request request { streamLowestLayer.remote_endpoint().address() };
        auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(s_requestTimeout);
        Asio::Timer timeoutTimer { co_await asio::this_coro::executor };
        Asio::Error error {};
        Asio::CancellationSignal signal {};
        volatile bool canceled { false };

        if (Log::allowed(Log::Level::Debug)) {
            request.m_logger->debug(Mbs::c_connectionWith, request.m_remote.to_string());
        }

        timeoutTimer.expires_at(deadline);
        timeoutTimer.async_wait(
            [& signal, & canceled]
            (const Asio::Error timerError) {
                if (!timerError) {
                    canceled = true;
                    signal.emit(asio::cancellation_type::all);
                }
            }
        );

        try {
            {
                co_await stream.async_handshake(
                    asio::ssl::stream_base::server,
                    asio::bind_cancellation_slot(
                        signal.slot(),
                        asio::redirect_error(asio::use_awaitable, error)
                    )
                );
                if (error) {
                    throw Failure(Cat<c_xsStrSize>(Mbs::c_sslHandshakeOperation, ": "sv, error.message())); // NOLINT(*-exception-baseclass)
                }
            }

            if (!canceled) {
                Asio::StreamBuffer buffer {};

                co_await asio::async_read_until(
                    stream, buffer,
                    "\r\n\r\n",
                    asio::bind_cancellation_slot(
                        signal.slot(),
                        asio::redirect_error(asio::use_awaitable, error)
                    )
                );
                if (error) {
                    throw Failure(Cat<c_xsStrSize>(Mbs::c_sslReadOperation, ": "sv, error.message())); // NOLINT(*-exception-baseclass)
                }

                Http::Parser parser(request);
                parser(buffer);

                if (request.m_method == Http::Method::Post) {
                    auto expecting = parser.expecting();
                    while (!canceled && expecting) {
                        co_await asio::async_read(
                            stream, buffer,
                            asio::transfer_at_least(expecting),
                            asio::bind_cancellation_slot(
                                signal.slot(),
                                asio::redirect_error(asio::use_awaitable, error)
                            )
                        );
                        if (error) {
                            throw Failure(Cat<c_xsStrSize>(Mbs::c_sslReadOperation, ": "sv, error.message())); // NOLINT(*-exception-baseclass)
                        }
                        parser(buffer);
                        expecting = parser.expecting();
                    }
                }
                parser.complete();
            }

            if (!canceled) {
                if (request.m_response.m_status == Http::Status::Ok) {
                    if (Log::allowed(Log::Level::Info)) {
                        request.m_logger->info(
                            Mbs::c_requestedMethod, request.m_remote.to_string(), request.m_verb, request.m_path
                        );
                    }
                }

                if (
                    request.m_response.m_status < Http::Status::BadRequest
                    && (!s_loopbackWithoutSecret || !Asio::isLoopback(request.m_remote))
                ) {
                    auto it = request.m_header.find("x-secret");
                    if (it == request.m_header.end() || it->second.empty() || it->second != s_secret) {
                        request.m_response.m_status = Http::Status::Forbidden;
                        request.m_response.m_data.emplace<std::string>(Mbs::c_forbidden);
                        request.m_logger->error(Wcs::c_forbidden);
                    }
                }

                if (request.m_response.m_status == Http::Status::Ok) {
                    ProtoHandler & handler = lookupHandler(request);
                    if (handler.asyncReady()) {
                        co_await performAsync(handler, request, asio::use_awaitable);
                    } else {
                        (handler)(request);
                    }
                }
            }

            if (!canceled) {
                Asio::StreamBuffer buffer {};
                request.m_response.render(buffer);
                co_await asio::async_write(
                    stream, buffer,
                    asio::bind_cancellation_slot(
                        signal.slot(),
                        asio::redirect_error(asio::use_awaitable, error)
                    )
                );
                if (error) {
                    throw Failure(Cat<c_xsStrSize>(Mbs::c_sslWriteOperation, ": "sv, error.message())); // NOLINT(*-exception-baseclass)
                }
            }

        } catch (const Basic::Failure & e) {
            if (!canceled) {
                request.m_response.m_status = Http::Status::InternalServerError;
                request.m_logger->error(e);
            }
        } catch (const std::exception & e) {
            if (!canceled) {
                request.m_response.m_status = Http::Status::InternalServerError;
                request.m_logger->error(e.what());
            }
        } catch (...) {
            if (!canceled) {
                request.m_response.m_status = Http::Status::InternalServerError;
                request.m_logger->error(Basic::Wcs::c_somethingWrong);
            }
        }

        timeoutTimer.cancel();

        {
            streamLowestLayer.cancel();

            if (streamLowestLayer.is_open()) {
                error.clear();
                stream.shutdown(error); // NOLINT(*-unused-return-value)
                if (error) {
                    if (error.category() == asio::error::get_ssl_category()) {
                        auto sslError = ERR_GET_REASON(error.value());
                        if (sslError == SSL_R_APPLICATION_DATA_AFTER_CLOSE_NOTIFY) {
                            error = asio::ssl::error::stream_truncated;
                        }
                    }
                    if (
                        error != asio::ssl::error::stream_truncated // <= обязательно игнорируем
                        && error != asio::error::eof
                        && error != asio::error::connection_aborted // <= обязательно игнорируем
                        && error != asio::error::connection_refused
                        && error != asio::error::connection_reset   // <= обязательно игнорируем
                        && error != asio::error::not_connected
                    ) {
                        request.m_logger->error(
                            SrcLoc::Point::current(), Text::concat<c_xsStrSize>(Mbs::c_sslShutdownOperation, ": "sv, error.message())
                        );
                    }
                }
            }

            if (streamLowestLayer.is_open()) {
                error.clear();
                streamLowestLayer.close(error); // NOLINT(*-unused-return-value)
                if (error) {
                    request.m_logger->error(
                        SrcLoc::Point::current(), Text::concat<c_xsStrSize>(Mbs::c_socketCloseOperation, ": "sv, error.message())
                    );
                }
            }
        }

        if (canceled) {
            request.m_logger->warning(Wcs::c_timeoutExpired);
        } else if (request.m_response.m_status < Http::Status::BadRequest) {
            request.m_logger->info(Wcs::c_processingSuccess);
        } else {
            request.m_logger->warning(Wcs::c_processingFailed);
        }
    } catch (const Basic::Failure & e) {
        log(Log::Level::Error, e);
    } catch (const std::exception & e) {
        log(Log::Level::Error, e.what());
    } catch (...) {
        log(Log::Level::Error, Basic::Wcs::c_somethingWrong);
    }

    asio::awaitable<void> close(Asio::TcpSocket && socket0) noexcept try {
        Counter counter { s_delayedSocketsCounter };
        Asio::TcpSocket socket { std::move(socket0) };
        Asio::Timer timer { co_await asio::this_coro::executor };

        timer.expires_after(c_closingDelay);
        co_await timer.async_wait(asio::use_awaitable);
        socket.cancel();
        socket.shutdown(Asio::TcpSocket::shutdown_both);
        socket.close();
    } catch (...) {}

    asio::awaitable<void> accept(Asio::SslStreamConfig config) noexcept try {
        assert(s_state.load() == State::Starting);

        {
            auto executor = co_await asio::this_coro::executor;
            Asio::Acceptor acceptor { executor, config.m_endpoint };

            log(Log::Level::Info, Wcs::c_started);
            s_state.store(State::Running);

            do {
                auto [error, socket] = co_await acceptor.async_accept();
                if (error) {
                    log(Log::Level::Error, Mbs::c_connectionAcceptStatus, error.message());
                    log(Log::Level::Error, Wcs::c_servicingFailed);
                } else if (!socket.is_open()) {
                    log(Log::Level::Error, Wcs::c_socketOpeningError);
                    log(Log::Level::Error, Wcs::c_servicingFailed);
                } else if (s_concurrentRequestsCounter >= s_concurrencyLimit) {
                    if (s_delayedSocketsCounter >= c_delayedSockets) {
                        socket.cancel();
                        socket.shutdown(Asio::TcpSocket::shutdown_both);
                        socket.close();
                    } else {
                        log(Log::Level::Warning, Wcs::c_maximumIsExceeded);
                        asio::co_spawn(executor, close(std::move(socket)), asio::detached);
                    }
                } else {
                    asio::co_spawn(executor, handle(std::move(socket), config.m_sslContext), asio::detached);
                }
            } while (s_state.load() == State::Running);

            {
                Asio::Error error {};

                acceptor.cancel(error); // NOLINT(*-unused-return-value)
                if (error) {
                    log(Log::Level::Warning, Mbs::c_acceptorCancelStatus, error.message());
                }

                error.clear();
                acceptor.close(error); // NOLINT(*-unused-return-value)
                if (error) {
                    log(Log::Level::Warning, Mbs::c_acceptorCloseStatus, error.message());
                }
            }
        }

        {
            Asio::Timer timer { co_await asio::this_coro::executor };

            while (s_state.load() == State::Shutdown) {
                timer.expires_after(c_sleepQuantum);
                co_await timer.async_wait(asio::use_awaitable);
            }
        }
    } catch (const Basic::Failure & e) {
        completeHitmanJob(e);
    } catch (const std::exception & e) {
        completeHitmanJob(e.what());
    } catch (...) {
        completeHitmanJob(Basic::Wcs::c_somethingWrong);
    }

    void watchdog() noexcept {
        while (s_state.load() < State::Shutdown) {
            try {
                /** Сюда добавляем полезную фоновую нагрузку {{{ **/
                Cache::maintain();
                /** }}} **/
                std::this_thread::sleep_for(c_watchdogSleep);
            } catch (const Basic::Failure & e) {
                emergencyBrake(e, State::Shutdown);
            } catch (const std::exception & e) {
                emergencyBrake(e.what(), State::Shutdown);
            } catch (...) {
                emergencyBrake(Basic::Wcs::c_somethingWrong, State::Shutdown);
            }
        }

        Log::disableAsync();
        log(Log::Level::Debug, Wcs::c_stopping);

        auto counter = c_controlTimeout / c_sleepQuantum;
        while (counter > 0 && s_concurrentRequestsCounter > 0) {
            std::this_thread::sleep_for(c_sleepQuantum);
            --counter;
        }
        s_state.store(State::Stopping);
        std::this_thread::sleep_for(c_controlOvertime1);

        if (counter > 0) {
            std::scoped_lock lock { s_hitmanJobMutex };
            std::invoke(s_hitmanJob);
        }

        s_shutdownSync.arrive_and_wait();
        s_state.store(State::Stopped);
    }

    void listen(Asio::SslStreamConfig config) noexcept try {
        assert(s_state.load() == State::Starting);

        Asio::IoContext ioContext { std::clamp(static_cast<int>(std::thread::hardware_concurrency()), 1, 4) };
        Asio::SignalSet signals(ioContext, SIGINT, SIGTERM);
        signals.async_wait([] (auto, auto) noexcept { s_state.store(State::Shutdown); });

        {
            std::scoped_lock lock { s_hitmanJobMutex };
            s_hitmanJob = [& ioContext] { ioContext.stop(); };
        }

        std::thread(watchdog).detach();
        asio::co_spawn(ioContext, accept(std::move(config)), asio::detached);
        ioContext.run();

        {
            std::scoped_lock lock { s_hitmanJobMutex };
            s_hitmanJob = c_dummyJob;
        }

        log(Log::Level::Info, Wcs::c_stopped);
        s_shutdownSync.arrive_and_wait();
    } catch (const Basic::Failure & e) {
        cancelHitmanJob(e);
    } catch (const std::exception & e) {
        cancelHitmanJob(e.what());
    } catch (...) {
        cancelHitmanJob(Basic::Wcs::c_somethingWrong);
    }

    Asio::SslStreamConfig init() {
        assert(s_state.load() == State::Initial);

        Log::enableAsync();
        log(Log::Level::Debug, Wcs::c_starting);
        s_state.store(State::Starting);

        Asio::SslStreamConfig config {
            .m_endpoint = Asio::Endpoint { s_ipv4Only ? asio::ip::tcp::v4() : asio::ip::tcp::v6(), s_port },
            .m_sslContext = Asio::SslContext { Asio::SslContext::tls_server }
        };

        constexpr auto alwaysClear
            = Asio::SslContext::no_tlsv1_2 | Asio::SslContext::no_tlsv1_3;

        constexpr auto alwaysSet
            = Asio::SslContext::default_workarounds | Asio::SslContext::no_sslv2 | Asio::SslContext::no_sslv3;

        constexpr auto noLegacyTls
            = Asio::SslContext::no_tlsv1 | Asio::SslContext::no_tlsv1_1;

        if (s_enableLegacyTls) {
            ::SSL_CTX_set_security_level(config.m_sslContext.native_handle(), 0);
            ::SSL_CTX_set_min_proto_version(config.m_sslContext.native_handle(), TLS1_VERSION);
            config.m_sslContext.clear_options(alwaysClear | noLegacyTls);
            config.m_sslContext.set_options(alwaysSet);
        } else {
            if (s_securityLevel >= 0) {
                ::SSL_CTX_set_security_level(config.m_sslContext.native_handle(), s_securityLevel);
            }
            config.m_sslContext.clear_options(alwaysClear);
            config.m_sslContext.set_options(alwaysSet | noLegacyTls);
        }

        config.m_sslContext.set_password_callback([] (auto, auto) { return s_privateKeyPassword; });
        // TODO: Облагородить способ установки ключа и сертификатов.
        config.m_sslContext.use_certificate_chain_file(Text::convert(s_certificateChainFile.wstring()));
        config.m_sslContext.use_private_key_file(Text::convert(s_privateKeyFile.wstring()), Asio::SslContext::pem);
        config.m_sslContext.set_verify_mode(asio::ssl::verify_none);

        return config;
    }

    void run() {
        listen(init());
    }

    bool start() {
        std::thread server { listen, init() };

        for (auto i = c_controlTimeout / c_sleepQuantum; i; --i) {
            std::this_thread::sleep_for(c_sleepQuantum);
            const auto state = s_state.load();
            if (state == State::Running) {
                server.detach();
                return true;
            }
            if (state == State::Shutdown || state == State::Stopping) {
                break;
            }
        }

        {
            std::scoped_lock lock { s_hitmanJobMutex };
            std::invoke(s_hitmanJob);
        }

        if (server.joinable()) {
            server.join();
        }

        return false;
    }

    bool stop() {
        s_state.store(State::Shutdown);

        auto counter = (c_controlTimeout + c_controlOvertime2) / c_sleepQuantum;
        while (counter > 0 && s_state.load() != State::Stopped) {
            std::this_thread::sleep_for(c_sleepQuantum);
            --counter;
        }

        return s_state.load() == State::Stopped;
    }
}
