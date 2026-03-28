// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"
#include "strings.h"
#include "queue.h"
#include "accessor.h"
#include "state.h"
#include <lib/wconv.h>
#include <lib/text.h>
#include <variant>
#include <string>
#include <format>

#define LOG_DEBUG(x, ...) Log::write(Log::Level::Debug, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_INFO(x, ...) Log::write(Log::Level::Info, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_WARNING(x, ...) Log::write(Log::Level::Warning, x __VA_OPT__(,) __VA_ARGS__)
#define LOG_ERROR(x, ...) Log::write(Log::Level::Error, x __VA_OPT__(,) __VA_ARGS__)

namespace Log {
    namespace Wcs {
        constexpr std::wstring_view c_prefix { L"{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}: {}: " };
    }

    constexpr size_t c_excessSize { 21 };

    [[nodiscard, maybe_unused]] std::wstring levelLabel(int);

#ifndef SINGLE_THREAD
    [[maybe_unused]] void enableAsync() noexcept;
    [[maybe_unused]] void disableAsync() noexcept;
#endif

    using RmlRecordAccessor = ExclusiveAccessor<std::recursive_mutex>;
    using RecordVariant = std::variant<RmlRecordAccessor, LoggerQueue::ProducerAccessor>;

    inline auto RecordReady = [] (auto & accessor) -> bool { return static_cast<bool>(accessor); };
    inline auto RecordRef = [] (auto & accessor) -> Record & { return *accessor; };
    inline auto RecordWrite = [] (auto & accessor) -> void { accessor.complete(); };

    [[nodiscard, maybe_unused]] RecordVariant getFreeRecord();

    [[maybe_unused]]
    inline bool writePrefix(std::wstring & result, const Level level) noexcept try {
        result.clear();
        ::SYSTEMTIME localTime;
        ::GetLocalTime(&localTime);
        std::format_to(
            std::back_inserter(result),
            Wcs::c_prefix,
            localTime.wYear, localTime.wMonth, localTime.wDay,
            localTime.wHour, localTime.wMinute, localTime.wSecond,
            Wcs::c_levelLabels.at(level)
        );
        return result.size() >= c_excessSize;
    } catch (...) {
        return false;
    }

    template<Meta::View T, typename ... Args>
    [[maybe_unused]]
    void write(const Level level, const T message, Args && ... args) noexcept try {
        if (message.empty()) {
            return;
        }
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
            if (auto recordVariant = getFreeRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
                auto & buffer = record.m_message;
                if (writePrefix(buffer, level)) {
                    if constexpr (Meta::isWide<T>) {
                        if constexpr (sizeof...(Args) > 0) {
                            std::vformat_to(std::back_inserter(buffer), message, std::make_wformat_args(args...));
                        } else {
                            buffer.append(message);
                        }
                    } else {
                        if constexpr (sizeof...(Args) > 0) {
                            std::string mbMessage {};
                            mbMessage.reserve(s_lineSize);
                            std::vformat_to(std::back_inserter(mbMessage), message, std::make_format_args(args...));
                            Text::appendConverted(buffer, mbMessage);
                        } else {
                            Text::appendConverted(buffer, message);
                        }
                    }
                    record.m_terse = std::wstring_view(buffer.data() + c_excessSize, buffer.size() - c_excessSize);
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
                    std::visit(RecordWrite, recordVariant);
                }
            }
        }
    } catch (...) {}

    template<Meta::Char T, typename ... Args>
    [[maybe_unused]]
    void write(const Level level, const T * message, Args && ... args) noexcept {
        write<typename Meta::TextTrait<T>::View>(level, message, std::forward<Args>(args)...);
    }

    template<Meta::String T, typename ... Args>
    [[maybe_unused]]
    void write(const Level level, const T & message, Args && ... args) noexcept {
        write<typename Meta::TextTrait<T>::View>(level, message, std::forward<Args>(args)...);
    }

    [[maybe_unused]]
    inline void write(const Level level, const Basic::Failure & e) noexcept try {
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
            if (auto recordVariant = getFreeRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
                auto & buffer = record.m_message;
                if (writePrefix(buffer, level)) {
                    e.appendExplanation(buffer, s_appendLocation);
                    record.m_terse = std::wstring_view(buffer.data() + c_excessSize, buffer.size() - c_excessSize);
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
                    std::visit(RecordWrite, recordVariant);
                }
            }
        }
    } catch (...) {}

    [[maybe_unused]]
    inline void write(const Level level, const std::exception & e) noexcept try {
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
            if (auto recordVariant = getFreeRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
                auto & buffer = record.m_message;
                if (writePrefix(buffer, level)) {
                    Text::appendConverted(buffer, e.what());
                    record.m_terse = std::wstring_view(buffer.data() + c_excessSize, buffer.size() - c_excessSize);
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
                    std::visit(RecordWrite, recordVariant);
                }
            }
        }
    } catch (...) {}

    [[maybe_unused]]
    inline void write(const Level level, const std::error_code & e) noexcept try {
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
            if (auto recordVariant = getFreeRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
                auto & buffer = record.m_message;
                if (writePrefix(buffer, level)) {
                    Text::appendConverted(buffer, e.message());
                    record.m_terse = std::wstring_view(buffer.data() + c_excessSize, buffer.size() - c_excessSize);
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
                    std::visit(RecordWrite, recordVariant);
                }
            }
        }
    } catch (...) {}

    template<class T>
    requires requires (const T & t) { { t() } -> Meta::String; }
    [[maybe_unused]]
    void write(const Level level, const T & func) noexcept try {
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
            if (auto recordVariant = getFreeRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
                auto & buffer = record.m_message;
                if (writePrefix(buffer, level)) {
                    if constexpr (Meta::isWide<std::remove_cvref_t<decltype(func())>>) {
                        buffer.append(func());
                    } else {
                        const auto & temp = func();
                        Text::appendConverted(buffer, temp);
                    }
                    record.m_terse = std::wstring_view(buffer.data() + c_excessSize, buffer.size() - c_excessSize);
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
                    std::visit(RecordWrite, recordVariant);
                }
            }
        }
    } catch (...) {}
}

namespace Config {
    [[maybe_unused]] void initLogger();
    [[maybe_unused]] void reinitLogger();
}
