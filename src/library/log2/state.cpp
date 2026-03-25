// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "state.h"

namespace Log {
    static bool s_isForegroundProcess { true };

    namespace Console {
        [[nodiscard]]
        bool allowed() noexcept {
            return s_isForegroundProcess;
        }

        [[nodiscard]]
        bool allowed(const Level level) noexcept {
            return s_isForegroundProcess && Meta::toUnderlying(level) >= s_level;
        }
    }

    namespace File {
        [[nodiscard]]
        bool allowed(const Level level) noexcept {
            return Meta::toUnderlying(level) >= (s_isForegroundProcess ? s_fgLevel : s_bgLevel);
        }
    }

    namespace EventLog {
        [[nodiscard]]
        bool allowed(const Level level) noexcept {
            return Meta::toUnderlying(level) >= (s_isForegroundProcess ? s_fgLevel : s_bgLevel);
        }
    }

    [[maybe_unused]]
    void asForegroundProcess() noexcept {
        s_isForegroundProcess = true;
    }

    [[maybe_unused]]
    void asBackgroundProcess() noexcept {
        s_isForegroundProcess = false;
    }
}
