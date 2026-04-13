// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <thread>
#if defined(__clang__)
#   include <immintrin.h>
#elif defined(_MSC_VER)
#   include <intrin.h>
#else
#   error Unsupported compiler
#endif

namespace Ccy {
    enum class SpinMethod { Pause, YieldThread, WaitFlag, Active };
    constexpr auto c_defaultSpinMethod = SpinMethod::Pause;

    template<SpinMethod P = c_defaultSpinMethod>
    class SpinLock {
        std::atomic_flag m_flag {};

    public:
        SpinLock() noexcept = default;
        SpinLock(const SpinLock &) = delete;
        SpinLock(SpinLock &&) = delete;
        ~SpinLock() = default;

        SpinLock & operator=(const SpinLock &) = delete;
        SpinLock & operator=(SpinLock &&) = delete;

        void lock() noexcept {
            while (m_flag.test_and_set(MemOrd::acquire)) {
                if constexpr (P == SpinMethod::Pause) {
                    _mm_pause();
                } else if constexpr (P == SpinMethod::YieldThread) {
                    std::this_thread::yield();
                } else if constexpr (P == SpinMethod::WaitFlag) {
                    m_flag.wait(true, MemOrd::relaxed);
                }
            }
        }

        /** Lockable requirements **/
        [[maybe_unused]]
        bool try_lock() noexcept {
            return !m_flag.test_and_set(MemOrd::acquire);
        }

        void unlock() noexcept {
            m_flag.clear(MemOrd::release);
            if constexpr (P == SpinMethod::WaitFlag) {
                m_flag.notify_one();
            }
        }
    };

    template class SpinLock<SpinMethod::Pause>;
    template class SpinLock<SpinMethod::YieldThread>;
    template class SpinLock<SpinMethod::WaitFlag>;
    template class SpinLock<SpinMethod::Active>;
}
