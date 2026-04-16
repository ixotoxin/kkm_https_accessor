// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <type_traits>
#include <new>
#include <algorithm>
// #include <bit>
#include <atomic>

namespace Ccy {
#ifdef __cpp_lib_hardware_interference_size
    constexpr size_t c_hwDIS { std::hardware_destructive_interference_size };
    constexpr size_t c_hwCIS { std::hardware_constructive_interference_size };
#else
    constexpr size_t c_hwDIS { 64 };
    constexpr size_t c_hwCIS { 64 };
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

    // template<size_t A, size_t C>
    // requires (A > 0) && (C > 0)
    // struct alignas(std::bit_ceil(A)) AlignedPadding {
    //     char m_padding[std::bit_ceil(A) * C] {};
    // };
    //
    // template<typename T, size_t C>
    // using TypeAlignedPadding = AlignedPadding<std::max({sizeof(T), alignof(T)}), C>;
}
