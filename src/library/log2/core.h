// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"
#include "strings.h"
#ifdef SINGLE_THREAD
#   include "writers.h"
#else
#   include "queue.h"
#   include "accessor.h"
#endif
#include "state.h"
#include <lib/wconv.h>
#include <lib/text.h>
#ifndef SINGLE_THREAD
#   include <variant>
#endif
#include <format>

namespace Log {
    namespace Wcs {
        constexpr std::wstring_view c_tsPrefix { L"{:04d}-{:02d}-{:02d} {:02d}:{:02d}:{:02d}: {}: {}: " };
    }

    constexpr size_t c_terseMsg1Offset { 21 };
    constexpr size_t c_terseMsg2Offset { 31 };

    [[nodiscard, maybe_unused]] std::wstring levelLabel(int);

#ifndef SINGLE_THREAD
    [[maybe_unused]] void enableAsync() noexcept;
    [[maybe_unused]] void disableAsync() noexcept;

    using RmlRecordAccessor = ExclusiveAccessor<std::recursive_mutex>;
    using RecordVariant = std::variant<RmlRecordAccessor, LoggerQueue::ProducerAccessor>;

    inline auto RecordReady = [] (auto & accessor) -> bool { return static_cast<bool>(accessor); };
    inline auto RecordRef = [] (auto & accessor) -> Record & { return *accessor; };
    inline auto RecordWrite = [] (auto & accessor) -> void { accessor.complete(); };

    [[nodiscard, maybe_unused]] RecordVariant acquireWritableRecord();
#endif

    [[maybe_unused]]
    inline bool formatPrefix(
        std::wstring & buffer,
        CategoryUnderlying category,
        const Level level,
        const std::wstring_view prefix
    ) noexcept try {
        buffer.clear();
        ::SYSTEMTIME localTime;
        ::GetLocalTime(&localTime);
        if (!Wcs::c_categoryLabels.contains(category)) {
            category = c_catUnknown;
        }
        std::format_to(
            std::back_inserter(buffer),
            Wcs::c_tsPrefix,
            localTime.wYear, localTime.wMonth, localTime.wDay,
            localTime.wHour, localTime.wMinute, localTime.wSecond,
            Wcs::c_categoryLabels.at(category),
            Wcs::c_levelLabels.at(level)
        );
        if (!prefix.empty()) {
            buffer.append(prefix);
        }
        return buffer.size() >= c_terseMsg1Offset;
    } catch (...) {
        return false;
    }

    template<Meta::View T, typename ... Args>
    [[maybe_unused]]
    void write(
        const Category category,
        const Level level,
        const std::wstring_view prefix,
        const T message,
        Args && ... args
    ) noexcept try {
        if (message.empty()) {
            return;
        }
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
#ifdef SINGLE_THREAD
            Record record {};
#else
            if (auto recordVariant = acquireWritableRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
#endif
                auto & buffer = record.m_message;
                if (formatPrefix(buffer, Meta::toUnderlying(category), level, prefix)) {
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
                    record.m_terseMsg1
                        = std::wstring_view(buffer.data() + c_terseMsg1Offset, buffer.size() - c_terseMsg1Offset);
                    record.m_terseMsg2
                        = std::wstring_view(buffer.data() + c_terseMsg2Offset, buffer.size() - c_terseMsg2Offset);
                    record.m_location.clear();
                    record.m_category = category;
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
#ifdef SINGLE_THREAD
                    write(record);
#else
                    std::visit(RecordWrite, recordVariant);
                }
#endif
            }
        }
    } catch (...) {}

    template<Meta::Char T, typename ... Args>
    [[maybe_unused]]
    void write(
        const Category category,
        const Level level,
        const std::wstring_view prefix,
        const T * message,
        Args && ... args
    ) noexcept {
        write<typename Meta::TextTrait<T>::View>(category, level, prefix, message, std::forward<Args>(args)...);
    }

    template<Meta::String T, typename ... Args>
    [[maybe_unused]]
    void write(
        const Category category,
        const Level level,
        const std::wstring_view prefix,
        const T & message,
        Args && ... args
    ) noexcept {
        write<typename Meta::TextTrait<T>::View>(category, level, prefix, message, std::forward<Args>(args)...);
    }

    template<Meta::View T, typename ... Args>
    [[maybe_unused]]
    void write(
        const SrcLoc::Point & location,
        const Category category,
        const Level level,
        const std::wstring_view prefix,
        const T message,
        Args && ... args
    ) noexcept try {
        if (message.empty()) {
            return;
        }
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
#ifdef SINGLE_THREAD
            Record record {};
#else
            if (auto recordVariant = acquireWritableRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
#endif
                auto & buffer = record.m_message;
                if (formatPrefix(buffer, Meta::toUnderlying(category), level, prefix)) {
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
                    record.m_terseMsg1
                        = std::wstring_view(buffer.data() + c_terseMsg1Offset, buffer.size() - c_terseMsg1Offset);
                    record.m_terseMsg2
                        = std::wstring_view(buffer.data() + c_terseMsg2Offset, buffer.size() - c_terseMsg2Offset);
                    if (s_appendLocation) {
                        Text::convert(record.m_location, location.file_name());
                        record.m_location.append(L":");
                        record.m_location.append(std::to_wstring(location.line()));
                    } else {
                        record.m_location.clear();
                    }
                    record.m_category = category;
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
#ifdef SINGLE_THREAD
                    write(record);
#else
                    std::visit(RecordWrite, recordVariant);
                }
#endif
            }
        }
    } catch (...) {}

    template<Meta::Char T, typename ... Args>
    [[maybe_unused]]
    void write(
        const SrcLoc::Point & location,
        const Category category,
        const Level level,
        const std::wstring_view prefix,
        const T * message,
        Args && ... args
    ) noexcept {
        write<typename Meta::TextTrait<T>::View>(
            location, category, level, prefix, message, std::forward<Args>(args)...
        );
    }

    template<Meta::String T, typename ... Args>
    [[maybe_unused]]
    void write(
        const SrcLoc::Point & location,
        const Category category,
        const Level level,
        const std::wstring_view prefix,
        const T & message,
        Args && ... args
    ) noexcept {
        write<typename Meta::TextTrait<T>::View>(
            location, category, level, prefix, message, std::forward<Args>(args)...
        );
    }

    [[maybe_unused]]
    inline void write(
        const Category clarifyingCategory,
        const Level level,
        const std::wstring_view prefix,
        const Basic::Failure & e
    ) noexcept try {
        const bool writeToConsole = Console::allowed(level);
        const bool writeToFile = File::allowed(level);
        const bool writeToEventLog = EventLog::allowed(level);
        if (writeToConsole || writeToFile || writeToEventLog) {
#ifdef SINGLE_THREAD
            Record record {};
#else
            if (auto recordVariant = acquireWritableRecord(); std::visit(RecordReady, recordVariant)) {
                auto & record = std::visit(RecordRef, recordVariant);
#endif
                auto & buffer = record.m_message;
                auto category = e.category();
                if (category == 0) {
                    category = Meta::toUnderlying(clarifyingCategory);
                }
                if (formatPrefix(buffer, category, level, prefix)) {
                    e.explain(buffer);
                    auto & location { e.where() };
                    record.m_terseMsg1
                        = std::wstring_view(buffer.data() + c_terseMsg1Offset, buffer.size() - c_terseMsg1Offset);
                    record.m_terseMsg2
                        = std::wstring_view(buffer.data() + c_terseMsg2Offset, buffer.size() - c_terseMsg2Offset);
                    if (s_appendLocation) {
                        Text::convert(record.m_location, location.file_name());
                        record.m_location.append(L":");
                        record.m_location.append(std::to_wstring(location.line()));
                    } else {
                        record.m_location.clear();
                    }
                    record.m_category = static_cast<Category>(category);
                    record.m_level = level;
                    record.m_toConsole = writeToConsole;
                    record.m_toFile = writeToFile;
                    record.m_toEventLog = writeToEventLog;
#ifdef SINGLE_THREAD
                    write(record);
#else
                    std::visit(RecordWrite, recordVariant);
                }
#endif
            }
        }
    } catch (...) {}
}

namespace Config {
    [[maybe_unused]] void initLogger();
    [[maybe_unused]] void reinitLogger();
}
