// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <atomic>
#include <thread>

namespace MtHelp {
    enum class Spin { Active, YieldThread, WaitFlag };

    template<Spin P = Spin::Active>
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
                if constexpr (P == Spin::YieldThread) {
                    std::this_thread::yield();
                } else if constexpr (P == Spin::WaitFlag) {
                    m_flag.wait(true, std::memory_order_relaxed);
                }
            }
        }

        bool tryLock() noexcept {
            return !m_flag.test_and_set(std::memory_order_acquire);
        }

        void unlock() noexcept {
            m_flag.clear(std::memory_order_release);
            if constexpr (P == Spin::WaitFlag) {
                m_flag.notify_one();
            }
        }
    };

    template<Spin P>
    class ScopedLock {
        SpinLock<P> & m_spinLock {};

    public:
        ScopedLock() = delete;
        ScopedLock(const ScopedLock &) = delete;
        ScopedLock(ScopedLock &&) = delete;

        explicit ScopedLock(SpinLock<P> & spinLock) noexcept
        : m_spinLock { spinLock } {
            m_spinLock.lock();
        }

        ~ScopedLock() {
            m_spinLock.unlock();
        }

        ScopedLock & operator=(const ScopedLock &) = delete;
        ScopedLock & operator=(ScopedLock &&) = delete;
    };
}
