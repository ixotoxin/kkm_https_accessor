// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"
#include <mutex>

namespace Log {
    inline std::recursive_mutex s_mutex {};

    namespace Console {
        [[nodiscard, maybe_unused]] bool allowed() noexcept;
        [[nodiscard, maybe_unused]] bool allowed(Level) noexcept;

        class ScopeLevelDown {
            const LevelUnderlying m_prevLevel { s_level };

        public:
            ScopeLevelDown() = delete;
            ScopeLevelDown(const ScopeLevelDown &) = delete;
            ScopeLevelDown(ScopeLevelDown &&) = delete;

            [[maybe_unused]]
            explicit ScopeLevelDown(const Level level) noexcept {
                if (Meta::toUnderlying(level) < s_level) {
                    s_level = Meta::toUnderlying(level);
                }
            }

            ~ScopeLevelDown() {
                s_level = m_prevLevel;
            }

            ScopeLevelDown & operator=(const ScopeLevelDown &) = delete;
            ScopeLevelDown & operator=(ScopeLevelDown &&) = delete;
        };

        class ScopeLevelUp {
            const LevelUnderlying m_prevLevel { s_level };

        public:
            ScopeLevelUp() = delete;
            ScopeLevelUp(const ScopeLevelUp &) = delete;
            ScopeLevelUp(ScopeLevelUp &&) = delete;

            explicit ScopeLevelUp(const Level level) noexcept {
                if (Meta::toUnderlying(level) > s_level) {
                    s_level = Meta::toUnderlying(level);
                }
            }

            ~ScopeLevelUp() {
                s_level = m_prevLevel;
            }

            ScopeLevelUp & operator=(const ScopeLevelUp &) = delete;
            ScopeLevelUp & operator=(ScopeLevelUp &&) = delete;
        };

        class ScopeSolo {
            const LevelUnderlying m_prevFileFgLevel { File::s_fgLevel };
            const LevelUnderlying m_prevFileBgLevel { File::s_bgLevel };
            const LevelUnderlying m_prevEventLogFgLevel { EventLog::s_fgLevel };
            const LevelUnderlying m_prevEventLogBgLevel { EventLog::s_bgLevel };

        public:
            ScopeSolo() noexcept {
                File::s_fgLevel = c_levelNone;
                File::s_bgLevel = c_levelNone;
                EventLog::s_fgLevel = c_levelNone;
                EventLog::s_bgLevel = c_levelNone;
            }

            ScopeSolo(const ScopeSolo &) = delete;
            ScopeSolo(ScopeSolo &&) = delete;

            ~ScopeSolo() {
                File::s_fgLevel = m_prevFileFgLevel;
                File::s_bgLevel = m_prevFileBgLevel;
                EventLog::s_fgLevel = m_prevEventLogFgLevel;
                EventLog::s_bgLevel = m_prevEventLogBgLevel;
            }

            ScopeSolo & operator=(const ScopeSolo &) = delete;
            ScopeSolo & operator=(ScopeSolo &&) = delete;
        };
    }

    namespace File {
        [[nodiscard, maybe_unused]] bool allowed(Level) noexcept;
    }

    namespace EventLog {
        [[nodiscard, maybe_unused]] bool allowed(Level) noexcept;
    }

    [[maybe_unused]] void asForegroundProcess() noexcept;
    [[maybe_unused]] void asBackgroundProcess() noexcept;
}
