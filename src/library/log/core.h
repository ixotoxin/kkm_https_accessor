// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"
#include <lib/defer.h>
#include <string>

#define LOG_DEBUG_CLI(M) \
    do { if (Log::Console::ready(Log::Level::Debug)) Log::Console::write(Log::Level::Debug, M); } while (false)

#define LOG_DEBUG_FMT(F, A, ...) LOG_DEBUG_CLI(std::format(F, A __VA_OPT__(,) __VA_ARGS__))

#define LOG_INFO_CLI(M) \
    do { if (Log::Console::ready(Log::Level::Info)) Log::Console::write(Log::Level::Info, M); } while (false)

#define LOG_INFO_FMT(F, A, ...) LOG_INFO_CLI(std::format(F, A __VA_OPT__(,) __VA_ARGS__))

#define LOG_WARNING_CLI(M) Log::Console::write(Log::Level::Warning, M)
#define LOG_WARNING_FMT(F, A, ...) LOG_WARNING_CLI(std::format(F, A __VA_OPT__(,) __VA_ARGS__))
#define LOG_ERROR_CLI(M) Log::Console::write(Log::Level::Error, M)
#define LOG_ERROR_FMT(F, A, ...) LOG_ERROR_CLI(std::format(F, A __VA_OPT__(,) __VA_ARGS__))

namespace Log {
    namespace Console {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;

        [[maybe_unused]]
        inline Level levelDown(const Level level) {
            const auto prevLevel = static_cast<Level>(s_level);
            if (Meta::toUnderlying(level) < s_level) {
                s_level = Meta::toUnderlying(level);
            }
            return prevLevel;
        }

        [[maybe_unused]]
        inline Level levelUp(const Level level) {
            const auto prevLevel = static_cast<Level>(s_level);
            if (Meta::toUnderlying(level) > s_level) {
                s_level = Meta::toUnderlying(level);
            }
            return prevLevel;
        }

        [[nodiscard, maybe_unused]]
        inline auto postLevelDownRestorer(const Level level) {
            return [prevLevel = levelDown(level)] { levelUp(prevLevel); };
        }

        [[nodiscard, maybe_unused]]
        inline auto postLevelUpRestorer(const Level level) {
            return [prevLevel = levelUp(level)] { levelDown(prevLevel); };
        }

        class ScopeLevelDown : public Deferred::Exec<decltype(postLevelDownRestorer(Level::Debug))> {
        public:
            ScopeLevelDown() = delete;
            ScopeLevelDown(const ScopeLevelDown &) = delete;
            ScopeLevelDown(ScopeLevelDown &&) = delete;
            [[maybe_unused]] explicit ScopeLevelDown(const Level level) : Exec(postLevelDownRestorer(level)) {}
            ~ScopeLevelDown() = default;

            ScopeLevelDown & operator=(const ScopeLevelDown &) = delete;
            ScopeLevelDown & operator=(ScopeLevelDown &&) = delete;
        };

        class ScopeLevelUp : public Deferred::Exec<decltype(postLevelUpRestorer(Level::Debug))> {
        public:
            ScopeLevelUp() = delete;
            ScopeLevelUp(const ScopeLevelUp &) = delete;
            ScopeLevelUp(ScopeLevelUp &&) = delete;
            [[maybe_unused]] explicit ScopeLevelUp(const Level level) : Exec(postLevelUpRestorer(level)) {}
            ~ScopeLevelUp() = default;

            ScopeLevelUp & operator=(const ScopeLevelUp &) = delete;
            ScopeLevelUp & operator=(ScopeLevelUp &&) = delete;
        };
    }

    namespace File {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, std::wstring_view) noexcept;
    }

    namespace EventLog {
        [[nodiscard, maybe_unused]] bool ready(Level) noexcept;
        [[maybe_unused]] void write(Level, const std::wstring &) noexcept;
    }

    void fallbackLog() noexcept;
    void reconfig() noexcept;
    [[maybe_unused]] void asForegroundProcess() noexcept;
    [[maybe_unused]] void asBackgroundProcess() noexcept;
    [[maybe_unused]] std::wstring levelLabel(int);
}

namespace Config {
    [[maybe_unused]] void initLogger();
}
