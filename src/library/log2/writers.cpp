// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "writers.h"
#include "variables.h"
#include "strings.h"
#include "state.h"
#include <lib/winapi.h>
#include <cassert>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace Log {
    namespace Console {
        [[maybe_unused]]
        void write(const Level level, const std::wstring_view message, const std::wstring_view location) noexcept try {
            std::wostream & output {
                s_output == Output::StdOut ? std::wcout
                : (s_output == Output::StdErr ? std::wcerr
                : (level >= Level::Warning ? std::wcerr : std::wcout))
            };
            output << message;
            if (!location.empty()) {
                output << L" (" << Wcs::c_source << L' ' << location << L')';
            }
            output << L'\n';
            if (s_flushEveryWrite) {
                output.flush();
            }
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
            std::wstring fileName {};
            fileName.reserve(c_xsStrSize);
#ifdef EXTERNAL_LOG_VARIABLES
            std::vformat_to(
                std::back_inserter(fileName),
                c_filenameFormat,
                std::make_wformat_args(localTime.wYear, localTime.wMonth)
            );
#else
            std::format_to(std::back_inserter(fileName), c_filenameFormat, localTime.wYear, localTime.wMonth);
#endif
            filePath /= fileName;
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
        void write(const std::wstring_view message, const std::wstring_view location) noexcept try {
            s_file << message;
            if (!location.empty()) {
                s_file << L" (" << Wcs::c_source << L' ' << location << L')';
            }
            s_file << L'\n';
            if (s_flushEveryWrite) {
                s_file.flush();
            }
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

        static const std::unordered_map<Level, ::DWORD> c_event1Id {
            { Level::Debug, MSG_GENERIC_INFO },
            { Level::Info, MSG_GENERIC_INFO },
            { Level::Warning, MSG_GENERIC_WARNING },
            { Level::Error, MSG_GENERIC_ERROR }
        };

        static const std::unordered_map<Level, ::DWORD> c_event2Id {
            { Level::Debug, MSG_GENERIC_INFO_WITH_SOURCE },
            { Level::Info, MSG_GENERIC_INFO_WITH_SOURCE },
            { Level::Warning, MSG_GENERIC_WARNING_WITH_SOURCE },
            { Level::Error, MSG_GENERIC_ERROR_WITH_SOURCE }
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
        void write(
            const Category category,
            const Level level,
            const wchar_t * message
        ) noexcept try {
            assert(c_types.contains(level));
            assert(c_event1Id.contains(level));

            const wchar_t * strings[1] { message };

            ::ReportEventW(
                s_sourceHandle,                 // Event log handle
                c_types.at(level),              // Event type
                static_cast<::WORD>(category),  // Event category
                c_event1Id.at(level),           // Event identifier
                nullptr,                        // No security identifier
                1,                              // Size of strings array
                0,                              // No binary data
                strings,                        // Array of strings
                nullptr                         // No binary data
            );
        } catch (...) {
            close();
        }

        [[maybe_unused]]
        void write(
            const Category category,
            const Level level,
            const wchar_t * message,
            const wchar_t * location
        ) noexcept try {
            assert(c_types.contains(level));
            assert(c_event2Id.contains(level));

            const wchar_t * strings[2] { message, location };

            ::ReportEventW(
                s_sourceHandle,                 // Event log handle
                c_types.at(level),              // Event type
                static_cast<::WORD>(category),  // Event category
                c_event2Id.at(level),           // Event identifier
                nullptr,                        // No security identifier
                2,                              // Size of strings array
                0,                              // No binary data
                strings,                        // Array of strings
                nullptr                         // No binary data
            );
        } catch (...) {
            close();
        }
    }

    static bool s_atExitCloseWriters { false };

    [[maybe_unused]]
    void initWriters() {
        // ISSUE: а так ли это необходимо?
        // std::ios_base::sync_with_stdio(false);
        // std::cin.tie(nullptr);
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
                Console::write(event.m_level, event.m_terseMsg1, event.m_location);
            } else {
                Console::write(event.m_level, event.m_message, event.m_location);
            }
        }
        if (event.m_toFile && File::open()) {
            File::write(event.m_message, event.m_location);
        }
        if (event.m_toEventLog && EventLog::open()) {
            if (event.m_location.empty()) {
                EventLog::write(event.m_category, event.m_level, event.m_terseMsg2.data());
            } else {
                EventLog::write(event.m_category, event.m_level, event.m_terseMsg2.data(), event.m_location.data());
            }
        }
    }
}
