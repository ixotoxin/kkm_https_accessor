// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"

namespace Log {
    namespace Console {
        [[nodiscard, maybe_unused]] bool allowed() noexcept;
        [[nodiscard, maybe_unused]] bool allowed(Level) noexcept;

        class ScopedLevelDown {
            const LevelUnderlying m_prevLevel { s_level };

        public:
            ScopedLevelDown() = delete;
            ScopedLevelDown(const ScopedLevelDown &) = delete;
            ScopedLevelDown(ScopedLevelDown &&) = delete;

            [[maybe_unused]]
            explicit ScopedLevelDown(const Level level) noexcept {
                if (Meta::toUnderlying(level) < s_level) {
                    s_level = Meta::toUnderlying(level);
                }
            }

            ~ScopedLevelDown() {
                s_level = m_prevLevel;
            }

            ScopedLevelDown & operator=(const ScopedLevelDown &) = delete;
            ScopedLevelDown & operator=(ScopedLevelDown &&) = delete;
        };

        class ScopedLevelUp {
            const LevelUnderlying m_prevLevel { s_level };

        public:
            ScopedLevelUp() = delete;
            ScopedLevelUp(const ScopedLevelUp &) = delete;
            ScopedLevelUp(ScopedLevelUp &&) = delete;

            explicit ScopedLevelUp(const Level level) noexcept {
                if (Meta::toUnderlying(level) > s_level) {
                    s_level = Meta::toUnderlying(level);
                }
            }

            ~ScopedLevelUp() {
                s_level = m_prevLevel;
            }

            ScopedLevelUp & operator=(const ScopedLevelUp &) = delete;
            ScopedLevelUp & operator=(ScopedLevelUp &&) = delete;
        };

        class ScopedSolo {
            const LevelUnderlying m_prevFileFgLevel { File::s_fgLevel };
            const LevelUnderlying m_prevFileBgLevel { File::s_bgLevel };
            const LevelUnderlying m_prevEventLogFgLevel { EventLog::s_fgLevel };
            const LevelUnderlying m_prevEventLogBgLevel { EventLog::s_bgLevel };

        public:
            ScopedSolo() noexcept {
                File::s_fgLevel = c_levelNone;
                File::s_bgLevel = c_levelNone;
                EventLog::s_fgLevel = c_levelNone;
                EventLog::s_bgLevel = c_levelNone;
            }

            ScopedSolo(const ScopedSolo &) = delete;
            ScopedSolo(ScopedSolo &&) = delete;

            ~ScopedSolo() {
                File::s_fgLevel = m_prevFileFgLevel;
                File::s_bgLevel = m_prevFileBgLevel;
                EventLog::s_fgLevel = m_prevEventLogFgLevel;
                EventLog::s_bgLevel = m_prevEventLogBgLevel;
            }

            ScopedSolo & operator=(const ScopedSolo &) = delete;
            ScopedSolo & operator=(ScopedSolo &&) = delete;
        };

        class ScopedMute {
            const LevelUnderlying m_prevLevel { s_level };

        public:
            ScopedMute() noexcept {
                s_level = c_levelNone;
            }

            ScopedMute(const ScopedMute &) = delete;
            ScopedMute(ScopedMute &&) = delete;

            ~ScopedMute() {
                s_level = m_prevLevel;
            }

            ScopedMute & operator=(const ScopedMute &) = delete;
            ScopedMute & operator=(ScopedMute &&) = delete;
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
    [[nodiscard, maybe_unused]] bool allowed(Level) noexcept;
}
