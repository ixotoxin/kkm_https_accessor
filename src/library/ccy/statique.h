// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "algo.h"
#include "quecommons.h"
#include <cassert>
#include <thread>
#include <chrono>
#if defined(__clang__)
#   include <immintrin.h>
#elif defined(_MSC_VER)
#   include <intrin.h>
#else
#   error Unsupported compiler
#endif

namespace Ccy {
    template<
        std::default_initializable T,               /** Тип элемента очереди **/
        int S = c_queueDefaultBlockSize,            /** Размер очереди **/
        bool C = c_queueDefaultAutoCompletion,      /** Автокомплит при уничтожении аксессора **/
        bool H = c_queueDefaultSlotHandling,        /** Вкл/выкл обработку слотов при инициализации и доступе **/
        bool E = c_queueDefaultNoExceptAccess,      /** Вкл/выкл исключения при создании аксессора **/
        unsigned A = c_queueDefaultAcquireAttempts  /** Количество по-умолчанию попыток захватить слот **/
    >
    requires (S > 1 && A > 0)
    class alignas(c_alignment) StaticMpmcQueue {
    protected:
        using InternalSizeType = std::atomic_int_fast32_t;
        using InternalIndexType = std::atomic_uint_fast64_t;

        static constexpr bool c_handleSlot { H };
        static constexpr auto c_noExceptAccess = E;
        static constexpr auto c_noExceptPayload = std::is_nothrow_default_constructible_v<T>;

    public:
        using Payload = T;
        using SizeType = InternalSizeType::value_type;
        using IndexType = InternalIndexType::value_type;
        class ProducerAccessor;
        class ConsumerAccessor;

        explicit StaticMpmcQueue() noexcept(c_noExceptPayload);
        StaticMpmcQueue(const StaticMpmcQueue &) = delete;
        StaticMpmcQueue(StaticMpmcQueue &&) = delete;
        virtual ~StaticMpmcQueue() = default;

        StaticMpmcQueue & operator=(const StaticMpmcQueue &) = delete;
        StaticMpmcQueue & operator=(StaticMpmcQueue &&) = delete;

        [[nodiscard, maybe_unused]]
        SizeType capacity() const noexcept { // NOLINT
            return S;
        }

        [[nodiscard, maybe_unused]]
        SizeType freeSlots() const noexcept {
            return m_free.load(MemOrd::relaxed);
        }

        [[nodiscard, maybe_unused]]
        bool empty() const noexcept {
            return m_free.load(MemOrd::acquire) >= S;
        }

        [[nodiscard, maybe_unused]]
        bool producing() const noexcept {
            return m_producing.test(MemOrd::acquire);
        }

        [[nodiscard, maybe_unused]]
        bool consuming() const noexcept {
            return m_consuming.test(MemOrd::acquire);
        }

        [[nodiscard]] ProducerAccessor producerSlot(unsigned = A) noexcept(c_noExceptAccess);
        [[nodiscard]] ConsumerAccessor consumerSlot() noexcept;

        template<WaitMethod W = c_defaultWaitMethod>
        [[maybe_unused]]
        void waitForEmpty() const noexcept {
            while (m_free.load(MemOrd::acquire) < S) {
                if constexpr (W == WaitMethod::Pause) {
                    _mm_pause();
                } else if constexpr (W == WaitMethod::YieldThread) {
                    std::this_thread::yield();
                }
            }
        }

        template<WaitMethod W = c_defaultWaitMethod>
        [[maybe_unused]]
        bool waitForEmpty(const std::chrono::steady_clock::duration timeout) const noexcept {
            const auto deadline = std::chrono::steady_clock::now() + timeout;
            while (m_free.load(MemOrd::acquire) < S) {
                if constexpr (W == WaitMethod::Pause) {
                    _mm_pause();
                } else if constexpr (W == WaitMethod::YieldThread) {
                    std::this_thread::yield();
                }
                if (std::chrono::steady_clock::now() > deadline) {
                    return false;
                }
            }
            return true;
        }

        [[maybe_unused]]
        void shutdown() noexcept {
            m_producing.clear(MemOrd::release);
        }

        [[maybe_unused]]
        void stop() noexcept {
            m_producing.clear(MemOrd::release);
            m_consuming.clear(MemOrd::release);
        }

        template<WaitMethod W = c_defaultWaitMethod>
        [[maybe_unused]]
        void gracefulShutdown() noexcept {
            shutdown();
            waitForEmpty<W>();
            stop();
        }

        template<WaitMethod W = c_defaultWaitMethod>
        [[maybe_unused]]
        bool gracefulShutdown(const std::chrono::steady_clock::duration timeout) noexcept {
            shutdown();
            auto fault = waitForEmpty<W>(timeout);
            stop();
            return fault;
        }

        [[maybe_unused]]
        std::pair<SizeType, SizeType> clear() noexcept(c_noExceptAccess);

    protected:
        static constexpr IndexType c_invalidIndex = std::numeric_limits<IndexType>::max();

        virtual bool prepare(Payload &) noexcept(c_noExceptAccess) {
            return true;
        }

        virtual bool clear(Payload &) noexcept(c_noExceptAccess) {
            return true;
        }

    private:
        using State = QueueSlotState;
        using Completion = QueueAccessorCompletion<C>;

        InternalIndexType m_producerIndex { 0 };
        InternalIndexType m_consumerIndex { 0 };
        InternalSizeType m_free { S };
        std::atomic_flag m_producing {};
        std::atomic_flag m_consuming {};
        alignas(c_alignment) std::atomic<State> m_state[static_cast<size_t>(S)] {};
        alignas(c_alignment) Payload m_payload[static_cast<size_t>(S)] {};
    };

    template<std::default_initializable T, int S, bool C, bool H, bool E, unsigned A>
    requires (S > 1 && A > 0)
    class StaticMpmcQueue<T, S, C, H, E, A>::ProducerAccessor : public Completion {
        friend class StaticMpmcQueue;

        StaticMpmcQueue * m_queue { nullptr };
        IndexType m_index { c_invalidIndex };

        void release() noexcept {
            m_queue->m_state[m_index].store(State::Free, MemOrd::release);
            m_queue->m_free.fetch_add(1, MemOrd::acq_rel);
        }

        void commit() noexcept {
            m_queue->m_state[m_index].store(State::Ready, MemOrd::release);
        }

        void reset() noexcept {
            m_queue = nullptr;
            m_index = c_invalidIndex;
        }

    public:
        ProducerAccessor() noexcept = default;
        ProducerAccessor(const ProducerAccessor &) = delete;

        ProducerAccessor(ProducerAccessor && other) noexcept
        : m_queue { other.m_queue }, m_index { other.m_index } {
            assert(!m_queue || m_queue->m_state[other.m_index].load(MemOrd::acquire) == State::ProdLocked);
            other.reset();
        }

        ~ProducerAccessor() {
            if (m_queue) {
                if constexpr (Completion::c_autoComplete) {
                    commit();
                } else {
                    if (Completion::m_complete) {
                        commit();
                    } else {
                        release();
                    }
                }
            }
        }

        ProducerAccessor & operator=(const ProducerAccessor &) = delete;

        ProducerAccessor & operator=(ProducerAccessor && other) noexcept {
            assert(m_queue != other.m_queue || m_index != other.m_index || (!m_queue && !other.m_queue));
            if (this == std::addressof(other)) {
                return *this;
            }
            if (m_queue) {
                assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ProdLocked);
                release();
            }
            if (other.m_queue) {
                assert(other.m_queue->m_state[other.m_index].load(MemOrd::acquire) == State::ProdLocked);
                m_queue = other.m_queue;
                m_index = other.m_index;
                other.reset();
            } else {
                reset();
            }
            return *this;
        }

        [[nodiscard, maybe_unused]]
        Payload * operator->() const noexcept {
            assert(m_queue);
            assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ProdLocked);
            return std::addressof(m_queue->m_payload[m_index]);
        }

        [[nodiscard, maybe_unused]]
        Payload & operator*() const noexcept {
            assert(m_queue);
            assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ProdLocked);
            return m_queue->m_payload[m_index];
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            assert(!m_queue || m_queue->m_state[m_index].load(MemOrd::acquire) == State::ProdLocked);
            return static_cast<bool>(m_queue);
        }

    protected:
        ProducerAccessor(StaticMpmcQueue * queue, const IndexType index) noexcept(c_noExceptAccess)
        : Completion {}, m_queue { queue }, m_index { index } {
            assert(m_queue);
            assert(m_index != c_invalidIndex);
            assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ProdLocked);
            m_queue->m_free.fetch_sub(1, MemOrd::acq_rel);
            if constexpr (c_handleSlot) {
                if constexpr (c_noExceptAccess) {
                    if (m_queue->prepare(m_queue->m_payload[m_index])) {
                        return;
                    }
                } else {
                    try {
                        if (m_queue->prepare(m_queue->m_payload[m_index])) {
                            return;
                        }
                    } catch (...) {
                        release();
                        throw;
                    }
                }
                release();
                reset();
            }
        }
    };

    template<std::default_initializable T, int S, bool C, bool H, bool E, unsigned A>
    requires (S > 1 && A > 0)
    class StaticMpmcQueue<T, S, C, H, E, A>::ConsumerAccessor : public Completion {
        friend class StaticMpmcQueue;

        StaticMpmcQueue * m_queue { nullptr };
        IndexType m_index { c_invalidIndex };

        void release() noexcept {
            m_queue->m_state[m_index].store(State::Ready, MemOrd::release);
        }

        void commit() noexcept {
            m_queue->m_state[m_index].store(State::Free, MemOrd::release);
            m_queue->m_free.fetch_add(1, MemOrd::acq_rel);
        }

        void reset() noexcept {
            m_queue = nullptr;
            m_index = c_invalidIndex;
        }

    public:
        ConsumerAccessor() noexcept = default;
        ConsumerAccessor(const ConsumerAccessor &) = delete;

        ConsumerAccessor(ConsumerAccessor && other) noexcept
        : m_queue { other.m_queue }, m_index { other.m_index } {
            assert(!m_queue || m_queue->m_state[m_index].load(MemOrd::acquire) == State::ConsLocked);
            other.reset();
        }

        ~ConsumerAccessor() {
            if (m_queue) {
                if constexpr (Completion::c_autoComplete) {
                    if constexpr (c_handleSlot) {
                        if constexpr (c_noExceptAccess) {
                            m_queue->clear(m_queue->m_payload[m_index]);
                        } else {
                            try {
                                m_queue->clear(m_queue->m_payload[m_index]);
                            } catch (...) {}
                        }
                    }
                    commit();
                } else {
                    if (Completion::m_complete) {
                        if constexpr (c_handleSlot) {
                            if constexpr (c_noExceptAccess) {
                                m_queue->clear(m_queue->m_payload[m_index]);
                            } else {
                                try {
                                    m_queue->clear(m_queue->m_payload[m_index]);
                                } catch (...) {}
                            }
                        }
                        commit();
                    } else {
                        release();
                    }
                }
            }
        }

        ConsumerAccessor & operator=(const ConsumerAccessor &) = delete;

        ConsumerAccessor & operator=(ConsumerAccessor && other) noexcept {
            assert(m_queue != other.m_queue || m_index != other.m_index || (!m_queue && !other.m_queue));
            if (this == std::addressof(other)) {
                return *this;
            }
            if (m_queue) {
                assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ConsLocked);
                release();
            }
            if (other.m_queue) {
                assert(other.m_queue->m_state[other.m_index].load(MemOrd::acquire) == State::ConsLocked);
                m_queue = other.m_queue;
                m_index = other.m_index;
                other.reset();
            } else {
                reset();
            }
            return *this;
        }

        [[nodiscard, maybe_unused]]
        const Payload * operator->() const noexcept {
            assert(m_queue);
            assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ConsLocked);
            return std::addressof(m_queue->m_payload[m_index]);
        }

        [[nodiscard, maybe_unused]]
        const Payload & operator*() const noexcept {
            assert(m_queue);
            assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ConsLocked);
            return m_queue->m_payload[m_index];
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            assert(!m_queue || m_queue->m_state[m_index].load(MemOrd::acquire) == State::ConsLocked);
            return static_cast<bool>(m_queue);
        }

    protected:
        ConsumerAccessor(StaticMpmcQueue * queue, const IndexType index) noexcept
        : Completion {}, m_queue { queue }, m_index { index } {
            assert(m_queue);
            assert(m_index != c_invalidIndex);
            assert(m_queue->m_state[m_index].load(MemOrd::acquire) == State::ConsLocked);
        }
    };

    template<std::default_initializable T, int S, bool C, bool H, bool E, unsigned A>
    requires (S > 1 && A > 0)
    StaticMpmcQueue<T, S, C, H, E, A>::StaticMpmcQueue() noexcept(c_noExceptPayload) {
        for (auto & el : m_state) {
            el.store(State::Free, MemOrd::relaxed);
        }
        m_producing.test_and_set(MemOrd::acquire);
        m_consuming.test_and_set(MemOrd::acquire);
    }

    template<std::default_initializable T, int S, bool C, bool H, bool E, unsigned A>
    requires (S > 1 && A > 0)
    auto
    StaticMpmcQueue<T, S, C, H, E, A>::producerSlot(unsigned acquireAttempts)
    noexcept(c_noExceptAccess) -> ProducerAccessor {
        assert(acquireAttempts > 0);

        do {
            for (auto count = S; count && m_producing.test(MemOrd::acquire) && m_free.load(MemOrd::acquire); --count) {
                auto state = State::Free;
                auto index = iteratePostInc<S>(m_producerIndex);
                if (m_state[index].compare_exchange_strong(state, State::ProdLocked, MemOrd::acq_rel, MemOrd::acquire)) {
                    return { this, index };
                }
            }
        } while (--acquireAttempts);

        return {};
    }

    template<std::default_initializable T, int S, bool C, bool H, bool E, unsigned A>
    requires (S > 1 && A > 0)
    auto StaticMpmcQueue<T, S, C, H, E, A>::consumerSlot() noexcept -> ConsumerAccessor {
        while (m_consuming.test(MemOrd::acquire) && m_free.load(MemOrd::acquire) < S) {
            auto state = State::Ready;
            auto index = iteratePostInc<S>(m_consumerIndex);
            if (m_state[index].compare_exchange_strong(state, State::ConsLocked, MemOrd::acq_rel, MemOrd::acquire)) {
                return { this, index };
            }
        }

        return {};
    }

    template<std::default_initializable T, int S, bool C, bool H, bool E, unsigned A>
    requires (S > 1 && A > 0)
    auto StaticMpmcQueue<T, S, C, H, E, A>::clear() noexcept(c_noExceptAccess) -> std::pair<SizeType, SizeType> {
        if (m_producing.test(MemOrd::acquire) || m_consuming.test(MemOrd::acquire)) {
            return { 0, 0 };
        }

        std::pair<SizeType, SizeType> result {};

        for (IndexType index {}; index < S; ++index ) {
            auto state = m_state[index].load(MemOrd::acquire);
            if (state == State::Ready) {
                if constexpr (c_handleSlot) {
                    clear(m_payload[index]);
                }
                m_state[index].store(State::Free, MemOrd::release);
                m_free.fetch_add(1, MemOrd::acq_rel);
                ++result.first;
            } else if (state != State::Free) {
                ++result.second;
            }
        }

        return result;
    }

    template<class Q>
    concept AnyStaticMpmcQueue
        = requires(Q q) {
            [] <std::default_initializable T, int S, bool C, bool H, bool E, unsigned A>
                (StaticMpmcQueue<T, S, C, H, E, A> &) {} (q);
        };
}
