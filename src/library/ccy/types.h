// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <new>
#include <atomic>

namespace Ccy {
#ifdef __cpp_lib_hardware_interference_size
    constexpr size_t c_alignment { std::hardware_constructive_interference_size };
#else
    constexpr size_t c_alignment { 64 };
#endif

    template<class T>
    concept AnyAtomicInt
        = requires { typename T::value_type; }
          && std::is_integral_v<typename T::value_type>
          && requires(T t) { [] <class U> (std::atomic<U> &) {} (t); };

    template<class T>
    concept AnyAtomicUint
        = requires { typename T::value_type; }
          && std::is_integral_v<typename T::value_type>
          && std::is_unsigned_v<typename T::value_type>
          && requires(T t) { [] <class U> (std::atomic<U> &) {} (t); };

    using MemOrd = std::memory_order;

    enum class GrowthPolicy { Call, Round, Step };
    constexpr auto c_defaultGrowthPolicy { GrowthPolicy::Round };

    enum class WaitMethod { Pause, YieldThread, Active };
    constexpr auto c_defaultWaitMethod { WaitMethod::Pause };

    struct ThrowingTag {};
    constexpr ThrowingTag Throwing {};

    struct NonThrowingTag {};
    constexpr NonThrowingTag NonThrowing {};

    class Discard : public std::exception {};
}
