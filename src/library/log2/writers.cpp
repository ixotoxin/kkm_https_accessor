// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "writers.h"
#include "state.h"
#include "strings.h"
#include <lib/winapi.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Log {
    namespace Console {
        [[maybe_unused]]
        void write(const Level level, const std::wstring_view message) noexcept try {
            std::wostream & output { level >= Level::Warning ? std::wcerr : std::wcout };
            output << message << std::endl;
        } catch (...) {}
    }

    namespace File {
        static std::wofstream s_file {};
        static ::WORD s_currentMonth { 0 };

        [[maybe_unused]]
        bool open() noexcept try {
            ::SYSTEMTIME localTime;
            ::GetLocalTime(&localTime);
            if (s_file.is_open()) {
                if (localTime.wMonth == s_currentMonth && s_file.good()) {
                    return true;
                }
                s_file.close();
            }
            std::filesystem::path filePath { s_directory };
            if (!std::filesystem::is_directory(filePath)) {
                std::filesystem::create_directories(filePath);
                if (!std::filesystem::is_directory(filePath)) {
                    return false;
                }
            }
            s_currentMonth = localTime.wMonth;
#ifdef EXTERNAL_LOG_VARIABLES
            filePath /= std::vformat(c_filenameFormat, std::make_wformat_args(localTime.wYear, localTime.wMonth));
#else
            filePath /= std::format(c_filenameFormat, localTime.wYear, localTime.wMonth);
#endif
            s_file.open(filePath, std::ios::out | std::ios::app);
            s_file.imbue(std::locale(".utf-8"));
            if (!s_file.good()) {
                s_file.close();
            }
            return s_file.is_open() && s_file.good();
        } catch (...) {
            return false;
        }

        [[maybe_unused]]
        void close() noexcept try {
            if (s_file.is_open()) {
                s_file.close();
            }
        } catch (...) {}

        [[maybe_unused]]
        void write(const std::wstring_view message) noexcept try {
            s_file << message << std::endl;
        } catch (...) {
            close();
        }
    }

    namespace EventLog {
        static const std::unordered_map<Level, ::WORD> c_types {
            { Level::Debug, static_cast<::WORD>(EVENTLOG_INFORMATION_TYPE) },
            { Level::Info, static_cast<::WORD>(EVENTLOG_INFORMATION_TYPE) },
            { Level::Warning, static_cast<::WORD>(EVENTLOG_WARNING_TYPE) },
            { Level::Error, static_cast<::WORD>(EVENTLOG_ERROR_TYPE) }
        };

        static HANDLE s_sourceHandle { nullptr };

        [[maybe_unused]]
        bool open() noexcept {
            if (!s_sourceHandle) {
                s_sourceHandle = ::RegisterEventSourceW(nullptr, c_eventSource);
            }
            return static_cast<bool>(s_sourceHandle);
        }

        [[maybe_unused]]
        void close() noexcept {
            if (s_sourceHandle) {
                ::DeregisterEventSource(s_sourceHandle);
                s_sourceHandle = nullptr;
            }
        }

        [[maybe_unused]]
        void write(const Level level, const wchar_t * message) noexcept try {
            assert(c_types.contains(level));

            const wchar_t * strings[2] {
                c_eventSource,
                message
            };

            ::ReportEventW(
                s_sourceHandle,     // Event log handle
                c_types.at(level),  // Event type
                c_eventCategory,    // Event category
                c_eventId,          // Event identifier
                nullptr,            // No security identifier
                2,                  // Size of strings array
                0,                  // No binary data
                strings,            // Array of strings
                nullptr             // No binary data
            );
        } catch (...) {
            close();
        }
    }

    static bool s_atExitCloseWriters { false };

    [[maybe_unused]]
    void initWriters() {
        if (!s_atExitCloseWriters) {
            s_atExitCloseWriters = true;
            std::atexit([] {
                Console::s_level = c_levelNone;
                File::s_fgLevel = c_levelNone;
                File::s_bgLevel = c_levelNone;
                EventLog::s_fgLevel = c_levelNone;
                EventLog::s_bgLevel = c_levelNone;
                File::close();
                EventLog::close();
            });
        }
    }

    [[maybe_unused]]
    void reinitWriters() noexcept {
        File::close();
        EventLog::close();
    }

    [[maybe_unused]]
    void write(const Record & event) noexcept {
        if (event.m_toConsole && Console::allowed()) {
            if (Console::s_terse) {
                Console::write(event.m_level, event.m_terse);
            } else {
                Console::write(event.m_level, event.m_message);
            }
        }
        if (event.m_toFile && File::open()) {
            File::write(event.m_message);
        }
        if (event.m_toEventLog && EventLog::open()) {
            EventLog::write(event.m_level, event.m_message.c_str());
        }
    }
}
