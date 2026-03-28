// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <atomic>
#include <thread>
#if defined(__clang__)
#   include <immintrin.h>
#elif defined(_MSC_VER)
#   include <intrin.h>
#else
#   error Unsupported compiler
#endif

namespace Ccy {
    enum class Spin { Pause, YieldThread, WaitFlag, Active };

    template<Spin P = Spin::Pause>
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
            while (m_flag.test_and_set(std::memory_order_acquire)) {
                if constexpr (P == Spin::Pause) {
                    _mm_pause();
                } else if constexpr (P == Spin::YieldThread) {
                    std::this_thread::yield();
                } else if constexpr (P == Spin::WaitFlag) {
                    m_flag.wait(true, std::memory_order_relaxed);
                }
            }
        }

        /** Lockable requirements  **/
        [[maybe_unused]]
        bool try_lock() noexcept {
            return !m_flag.test_and_set(std::memory_order_acquire);
        }

        void unlock() noexcept {
            m_flag.clear(std::memory_order_release);
            if constexpr (P == Spin::WaitFlag) {
                m_flag.notify_one();
            }
        }
    };

    template class SpinLock<Spin::Pause>;
    template class SpinLock<Spin::YieldThread>;
    template class SpinLock<Spin::WaitFlag>;
    template class SpinLock<Spin::Active>;
}
