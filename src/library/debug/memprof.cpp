// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "memprof.h"

#if (WITH_CRTD || WITH_SNTZ) && !defined(WITHOUT_LOGGING)
#   include <log2/core.h>
#endif

#if WITH_CRTD
// #   define _CRTDBG_MAP_ALLOC // NOLINT(*-reserved-identifier)
// #   include <lib/winapi.h>
// #   include <crtdbg.h>
// #   include <cstdlib>
#   include <format>
#endif

namespace Config {
    [[maybe_unused]]
    void initProfiler() {
#if WITH_CRTD
        constexpr auto reportMode = /*_CRTDBG_MODE_DEBUG |*/ _CRTDBG_MODE_FILE /*| _CRTDBG_MODE_WNDW*/;
        ::_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF /*| _CRTDBG_CHECK_CRT_DF*/ | _CRTDBG_LEAK_CHECK_DF);
        ::_CrtSetReportMode(_CRT_ASSERT, reportMode);
        ::_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_WARN, reportMode);
        ::_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
        ::_CrtSetReportMode(_CRT_ERROR, reportMode);
        ::_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
#   ifndef WITHOUT_LOGGING
        Log::write(Log::Category::Generic, Log::Level::Warning, {}, L"Memory profiling enabled (CRT Debug)");
#   endif
#elif WITH_SNTZ
#   ifndef WITHOUT_LOGGING
        Log::write(Log::Category::Generic, Log::Level::Warning, {}, L"AddressSanitizer enabled");
        Log::write(Log::Category::Generic, Log::Level::Warning, {}, L"UndefinedBehaviorSanitizer enabled");
#   endif
#endif
    }
}

namespace Debug {
    [[maybe_unused]]
    void forceMemoryLeak() {
#if WITH_CRTD || WITH_SNTZ
        auto memoryLeak [[maybe_unused]] = new unsigned short[32] { 0xadde, 0xefbe };
#   ifndef WITHOUT_LOGGING
        Log::write(
            Log::Category::Generic, Log::Level::Warning, {},
            L"I'll put {:x}{:x} {:x}{:x} here (test message indicating that a leak has taken place)",
            memoryLeak[0] & 0xff, memoryLeak[0] >> 8, memoryLeak[1] & 0xff, memoryLeak[1] >> 8
        );
        Log::write(
            Log::Category::Generic, Log::Level::Warning, {},
            std::format(L"Address of leaked memory block: 0x{:016X}",
            reinterpret_cast<uintptr_t>(memoryLeak))
        );
#   endif
#endif
    }
}

#if WITH_SNTZ
    // namespace __coe_win {
    //     bool ContinueOnError() { return false; }
    //     void RawWrite(const char*) {}
    // }
    extern "C" {
        // bool __coe_win_ContinueOnError() { return false; }
        // void __coe_win_RawWrite(const char*) {}
        const char * __asan_default_options() {
            return
                "halt_on_error=0"
                ":verbosity=0"
                ":log_path=stdout";
        }
        const char * __ubsan_default_options() {
            return
                "halt_on_error=0"
                ":verbosity=0"
                ":log_path=stdout";
        }
    }
#endif
