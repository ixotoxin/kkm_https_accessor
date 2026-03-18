// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

// TODO: А нужен ли здесь этот include?
#include <cmake/options.h>

#if !defined(WITH_CRTD) || !defined(WITH_SNTZ) || !defined(WITH_LEAKS)
#   error Requires WITH_CRTD and WITH_SNTZ macros to be defined
#endif

#if WITH_CRTD
#   if !defined(DEBUG) && !defined(_DEBUG)
#       error Memory profiling allowed only in debug version
#   endif
#   undef WITH_SNTZ
#   define WITH_SNTZ 0
#endif

#if WITH_SNTZ
#   if defined(DEBUG) || defined(_DEBUG)
#       error Sanitizers allowed only in release version
#   endif
#endif

#if WITH_LEAKS && !WITH_CRTD && !WITH_SNTZ
#   undef WITH_LEAKS
#   define WITH_LEAKS 0
#endif

#undef FORCE_MEMORY_LEAK

#if WITH_LEAKS
#   define FORCE_MEMORY_LEAK Debug::forceMemoryLeak()
#else
#   define FORCE_MEMORY_LEAK do {} while (false)
#endif

#define MEMORY_PROFILING_FLAG_KEY "!debug"

#if WITH_CRTD
#   define MEMORY_PROFILING_FLAG_VALUE "Memory profiling enabled (CRT Debug)"
#elif WITH_SNTZ
#   define MEMORY_PROFILING_FLAG_VALUE "Sanitizers enabled"
    // namespace __coe_win {
    //     bool ContinueOnError();
    //     void RawWrite(const char *);
    // }
    extern "C" {
        // bool __coe_win_ContinueOnError();
        // void __coe_win_RawWrite(const char *);
        const char * __asan_default_options();
        const char * __ubsan_default_options();
    }
#else
#   define MEMORY_PROFILING_FLAG_VALUE ""
#endif

namespace Config {
    [[maybe_unused]] void initProfiler();
}

namespace Debug {
    [[maybe_unused]] void forceMemoryLeak();
}
