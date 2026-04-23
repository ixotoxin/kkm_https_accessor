// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "core.h"
#include "strings.h"
#ifndef SINGLE_THREAD
#   include <atomic>
#   include <barrier>
#endif

namespace Log {
#ifndef SINGLE_THREAD
    static Record s_record {};
    static std::recursive_mutex s_mutex {};

    RecordVariant syncRecordAccessor() {
        return RecordVariant { std::in_place_type<RmlRecordAccessor>, s_record, s_mutex };
    }

    static std::atomic s_recordAccessor { syncRecordAccessor };

    static std::atomic<std::shared_ptr<LoggerQueue>> s_queue { nullptr };
    static std::thread s_asyncWriter {};
    static bool s_atExitDisableAsync { false };

    RecordVariant asyncRecordAccessor() {
        return RecordVariant {
            std::in_place_type<LoggerQueue::ProducerAccessor>, s_queue.load(std::memory_order_acquire)->producerSlot()
        };
    }

    void asyncWriterFunc() {
        bool notEmpty {};
        // while (s_queue.load(std::memory_order_acquire)->consuming() || notEmpty) {
        //     auto slot = s_queue.load(std::memory_order_acquire)->consumerSlot();
        //     notEmpty = static_cast<bool>(slot);
        //     if (notEmpty) {
        //         write(*slot);
        //         slot.complete();
        //     } else {
        //         std::this_thread::yield();
        //     }
        // }
        for (;;) {
            auto queue = s_queue.load(std::memory_order_acquire).get(); // NOLINT
            if (!queue || (!queue->consuming() && !notEmpty)) {
                break;
            }
            auto slot = queue->consumerSlot();
            notEmpty = static_cast<bool>(slot);
            if (notEmpty) {
                write(*slot);
            } else {
                std::this_thread::yield();
            }
        }
    }

    [[maybe_unused]]
    void enableAsync() noexcept {
        std::scoped_lock lock { s_mutex };
        if (!s_enableAsync || s_queue.load(std::memory_order_acquire)) {
            return;
        }
        write(Category::Generic, Level::Debug, {}, Wcs::c_enableAsync);
        s_queue.store(Ccy::make<LoggerQueue>(), std::memory_order_release);
        if (s_queue.load(std::memory_order_acquire)) {
            s_asyncWriter = std::thread { asyncWriterFunc };
            s_recordAccessor.store(asyncRecordAccessor, std::memory_order_release);
            if (!s_atExitDisableAsync) {
                s_atExitDisableAsync = true;
                std::atexit([] {
                    disableAsync();
                });
            }
        }
    }

    [[maybe_unused]]
    void disableAsync() noexcept {
        std::scoped_lock lock { s_mutex };
        if (!s_queue.load(std::memory_order_acquire)) {
            return;
        }
        s_recordAccessor.store(syncRecordAccessor, std::memory_order_release);
        s_queue.load(std::memory_order_acquire)->gracefulShutdown<Ccy::WaitMethod::YieldThread>();
        if (s_asyncWriter.joinable()) {
            s_asyncWriter.join();
        }
        s_queue.load(std::memory_order_acquire).reset();
        write(Category::Generic, Level::Debug, {}, Wcs::c_disableAsync);
    }

    [[nodiscard, maybe_unused]]
    RecordVariant acquireWritableRecord() {
        return s_recordAccessor.load(std::memory_order_relaxed)();
    }
#endif

    [[nodiscard, maybe_unused]]
    std::wstring levelLabel(LevelUnderlying level) {
        assert(level >= c_levelDebug && level <= c_levelNone);
        if (level == c_levelNone) {
            return L"none";
        }
        if (Wcs::c_levelLabels.contains(static_cast<Level>(level))) {
            return Text::lowered(Wcs::c_levelLabels.at(static_cast<Level>(level)));
        }
        return { Basic::Wcs::c_fallbackErrorMessage.data(), Basic::Wcs::c_fallbackErrorMessage.size() };
    }
}

namespace Config {
    [[maybe_unused]]
    void initLogger() {
        Log::initWriters();
    }

    [[maybe_unused]]
    void reinitLogger() {
#ifndef SINGLE_THREAD
        Log::s_record.m_message.reserve(Log::s_lineSize);
#endif
        Log::reinitWriters();
    }
}
