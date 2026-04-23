// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "quecommons.h"
#include "spinlock.h"
#include <cassert>
#include <memory>
#include <mutex>
#include <chrono>
#if defined(__clang__)
#   include <immintrin.h> // NOLINT
#elif defined(_MSC_VER)
#   include <intrin.h> // NOLINT
#else
#   error Unsupported compiler
#endif

namespace Ccy {
    template<
        std::default_initializable T,               /** Тип элемента очереди **/
        int S = c_queueDefaultBlockSize,            /** Размер блока очереди **/
        bool C = c_queueDefaultAutoCompletion,      /** Автокомплит при уничтожении аксессора **/
        bool H = c_queueDefaultSlotHandling,        /** Вкл/выкл обработку слотов при доступе **/
        bool O = c_queueDefaultNoExceptConstr,      /** Вкл/выкл исключения при создании очереди **/
        bool E = c_queueDefaultNoExceptAccess,      /** Вкл/выкл исключения при создании аксессора **/
        unsigned A = c_queueDefaultAcquireAttempts, /** Количество попыток по-умолчанию захватить слот **/
        GrowthPolicy G = c_defaultGrowthPolicy      /** Политика роста очереди **/
    >
    requires (S > 1 && A > 0)
    class DynamicMpmcQueue;

    template<class Q>
    concept AnyDynamicMpmcQueue
        = requires(Q q) {
            [] <std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
                (DynamicMpmcQueue<T, S, C, H, O, E, A, G> &) {} (q);
        };

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    class alignas(c_trueSharingAlign) DynamicMpmcQueue
    : public std::enable_shared_from_this<DynamicMpmcQueue<T, S, C, H, O, E, A, G>> {
    protected:
        struct KeyTag {};
        using InternalSizeType = std::atomic_int_fast32_t;

        static constexpr bool c_handleSlot { H };
        static constexpr auto c_noExceptConstr { O };
        static constexpr auto c_noExceptAccess { E };
        static constexpr auto c_noExceptPayload { std::is_nothrow_default_constructible_v<T> };

    public:
        using Payload = T;
        using Pointer = std::shared_ptr<DynamicMpmcQueue>;
        using SizeType = InternalSizeType::value_type;
        class ProducerAccessor;
        class ConsumerAccessor;

        template<AnyDynamicMpmcQueue U, typename ... Args>
        friend std::shared_ptr<U> make(Args ...) noexcept(U::c_noExceptConstr);

        DynamicMpmcQueue() = delete;
        DynamicMpmcQueue(KeyTag, ThrowingTag, int);
        DynamicMpmcQueue(KeyTag, NonThrowingTag, int) noexcept;
        DynamicMpmcQueue(const DynamicMpmcQueue &) = delete;
        DynamicMpmcQueue(DynamicMpmcQueue &&) = delete;
        virtual ~DynamicMpmcQueue() = default;

        DynamicMpmcQueue & operator=(const DynamicMpmcQueue &) = delete;
        DynamicMpmcQueue & operator=(DynamicMpmcQueue &&) = delete;

        [[nodiscard, maybe_unused]]
        SizeType capacity() const noexcept {
            return m_capacity.load(MemOrd::relaxed);
        }

        [[nodiscard, maybe_unused]]
        SizeType freeSlots() const noexcept {
            return m_free.load(MemOrd::relaxed);
        }

        [[nodiscard, maybe_unused]]
        bool empty() const noexcept {
            return m_free.load(MemOrd::acquire) >= m_capacity.load(MemOrd::acquire);
        }

        [[nodiscard, maybe_unused]]
        bool producing() const noexcept {
            return m_producer.m_enable.test(MemOrd::acquire);
        }

        [[nodiscard, maybe_unused]]
        bool consuming() const noexcept {
            return m_consumer.m_enable.test(MemOrd::acquire);
        }

        [[nodiscard]] ProducerAccessor producerSlot(unsigned = A) noexcept(c_noExceptAccess);
        [[nodiscard]] ConsumerAccessor consumerSlot() noexcept;

        template<WaitMethod W = c_defaultWaitMethod>
        [[maybe_unused]]
        void waitForEmpty() const noexcept {
            while (m_free.load(MemOrd::acquire) < m_capacity.load(MemOrd::acquire)) {
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
            while (m_free.load(MemOrd::acquire) < m_capacity.load(MemOrd::acquire)) {
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
            m_producer.m_enable.clear(MemOrd::release);
        }

        [[maybe_unused]]
        void stop() noexcept {
            m_producer.m_enable.clear(MemOrd::release);
            m_consumer.m_enable.clear(MemOrd::release);
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
        virtual bool prepare(Payload &) noexcept(c_noExceptAccess) {
            return true;
        }

        virtual bool clear(Payload &) noexcept(c_noExceptAccess) {
            return true;
        }

    private:
        using State = QueueSlotState;
        using Completion = QueueAccessorCompletion<C>;
        struct Slot;
        struct Block;
        using BlockPointer = std::shared_ptr<Block>;

        BlockPointer m_head { nullptr };
        Block * m_tail { nullptr };
        InternalSizeType m_capacity { 0 };
        InternalSizeType m_available;
        SpinLock<> m_spinLock {};
        struct alignas(c_falseSharingAlign) {
            std::atomic<Slot *> m_cursor { nullptr };
            std::atomic_flag m_enable {};
        } m_producer;
        struct alignas(c_falseSharingAlign) {
            std::atomic<Slot *> m_cursor { nullptr };
            std::atomic_flag m_enable {};
        } m_consumer;
        alignas(c_falseSharingAlign) InternalSizeType m_free { 0 };

        template<bool X> bool init() noexcept(X);
        bool grow() noexcept(c_noExceptAccess);
    };

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    struct alignas(c_trueSharingAlign) DynamicMpmcQueue<T, S, C, H, O, E, A, G>::Slot {
        Slot * m_next { nullptr };
        alignas(c_falseSharingAlign) std::atomic<State> m_state { State::Free };
        alignas(c_falseSharingAlign) Payload m_payload {};

        Slot() noexcept(c_noExceptPayload) = default;
        Slot(const Slot &) = delete;
        Slot(Slot &&) = delete;
        ~Slot() = default;

        Slot & operator=(const Slot &) = delete;
        Slot & operator=(Slot &&) = delete;
    };

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    struct alignas(c_trueSharingAlign) DynamicMpmcQueue<T, S, C, H, O, E, A, G>::Block {
        BlockPointer m_next { nullptr };
        Slot m_slots[static_cast<size_t>(S)] {};

        Block() noexcept(c_noExceptPayload) = default;
        Block(const Block &) = delete;
        Block(Block &&) = delete;
        ~Block() = default;

        Block & operator=(const Block &) = delete;
        Block & operator=(Block &&) = delete;

        [[nodiscard]]
        Slot * firstSlot() noexcept {
            return &m_slots[0];
        }

        [[nodiscard]]
        Slot * lastSlot() noexcept {
            return &m_slots[S - 1];
        }

        Slot * assemble(Slot * next = nullptr) noexcept {
            auto first = &m_slots[0];
            auto last = &m_slots[S - 1];
            auto it = first;
            while (it != last) {
                auto current = it++;
                current->m_next = it;
            }
            last->m_next = next ? next : first;
            return first;
        }
    };

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    class DynamicMpmcQueue<T, S, C, H, O, E, A, G>::ProducerAccessor : public Completion {
        friend class DynamicMpmcQueue;

        Pointer m_queue { nullptr };
        Slot * m_slot { nullptr };

        void release(DynamicMpmcQueue * queue) noexcept {
            m_slot->m_state.store(State::Free, MemOrd::release);
            queue->m_free.fetch_add(1, MemOrd::acq_rel);
        }

        void commit() noexcept {
            m_slot->m_state.store(State::Ready, MemOrd::release);
        }

        void reset() noexcept {
            m_queue.reset();
            m_slot = nullptr;
        }

    public:
        ProducerAccessor() noexcept = default;
        ProducerAccessor(const ProducerAccessor &) = delete;

        ProducerAccessor(ProducerAccessor && other) noexcept
        : m_queue { std::move(other.m_queue) }, m_slot { other.m_slot } {
            assert((m_queue && m_slot) || (!m_queue && !m_slot));
            assert(!m_slot || m_slot->m_state.load(MemOrd::acquire) == State::ProdLocked);
            other.reset();
        }

        ~ProducerAccessor() {
            if (auto queue = m_queue.get(); queue) {
                if constexpr (Completion::c_autoComplete) {
                    commit();
                } else {
                    if (Completion::m_complete) {
                        commit();
                    } else {
                        release(queue);
                    }
                }
            }
        }

        ProducerAccessor & operator=(const ProducerAccessor &) = delete;

        ProducerAccessor & operator=(ProducerAccessor && other) noexcept {
            assert(m_slot != other.m_slot || (!m_slot && !other.m_slot));
            if (this == std::addressof(other)) {
                return *this;
            }
            if (auto queue = m_queue.get(); queue) {
                assert(m_slot->m_state.load(MemOrd::acquire) == State::ProdLocked);
                release(queue);
            }
            if (other.m_slot) {
                assert(other.m_slot->m_state.load(MemOrd::acquire) == State::ProdLocked);
                m_queue = std::move(other.m_queue);
                m_slot = other.m_slot;
                other.reset();
            } else {
                reset();
            }
            return *this;
        }

        [[nodiscard, maybe_unused]]
        Payload * operator->() const noexcept {
            assert(m_queue);
            assert(m_slot);
            assert(m_slot->m_state.load(MemOrd::acquire) == State::ProdLocked);
            return std::addressof(m_slot->m_payload);
        }

        [[nodiscard, maybe_unused]]
        Payload & operator*() const noexcept {
            assert(m_queue);
            assert(m_slot);
            assert(m_slot->m_state.load(MemOrd::acquire) == State::ProdLocked);
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            assert((m_queue && m_slot) || (!m_queue && !m_slot));
            assert(!m_slot || m_slot->m_state.load(MemOrd::acquire) == State::ProdLocked);
            return m_slot;
        }

    protected:
        ProducerAccessor(Pointer queuePtr, Slot * slot) noexcept(c_noExceptAccess)
        : Completion {}, m_queue { std::move(queuePtr) }, m_slot { slot } {
            auto queue = m_queue.get();
            assert(queue);
            assert(m_slot);
            queue->m_free.fetch_sub(1, MemOrd::acq_rel);
            if constexpr (c_handleSlot) {
                if constexpr (c_noExceptAccess) {
                    if (queue->prepare(m_slot->m_payload)) {
                        return;
                    }
                } else {
                    try {
                        if (queue->prepare(m_slot->m_payload)) {
                            return;
                        }
                    } catch (...) {
                        release(queue);
                        // reset();
                        throw;
                    }
                }
                release(queue);
                reset();
            }
        }
    };

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    class DynamicMpmcQueue<T, S, C, H, O, E, A, G>::ConsumerAccessor : public Completion {
        friend class DynamicMpmcQueue;

        Pointer m_queue { nullptr };
        Slot * m_slot { nullptr };

        void release() noexcept {
            m_slot->m_state.store(State::Ready, MemOrd::release);
        }

        void commit(DynamicMpmcQueue * queue) noexcept {
            m_slot->m_state.store(State::Free, MemOrd::release);
            queue->m_free.fetch_add(1, MemOrd::acq_rel);
        }

        void reset() noexcept {
            m_queue.reset();
            m_slot = nullptr;
        }

    public:
        ConsumerAccessor() noexcept = default;
        ConsumerAccessor(const ConsumerAccessor &) = delete;

        ConsumerAccessor(ConsumerAccessor && other) noexcept
        : m_queue { std::move(other.m_queue) }, m_slot { other.m_slot } {
            assert((m_queue && m_slot) || (!m_queue && !m_slot));
            assert(!m_slot || m_slot->m_state.load(MemOrd::acquire) == State::ConsLocked);
            other.reset();
        }

        ~ConsumerAccessor() {
            if (auto queue = m_queue.get(); queue) {
                if constexpr (Completion::c_autoComplete) {
                    if constexpr (c_handleSlot) {
                        if constexpr (c_noExceptAccess) {
                            queue->clear(m_slot->m_payload);
                        } else {
                            try {
                                queue->clear(m_slot->m_payload);
                            } catch (...) {}
                        }
                    }
                    commit(queue);
                } else {
                    if (Completion::m_complete) {
                        if constexpr (c_handleSlot) {
                            if constexpr (c_noExceptAccess) {
                                m_queue->clear(m_slot->m_payload);
                            } else {
                                try {
                                    m_queue->clear(m_slot->m_payload);
                                } catch (...) {}
                            }
                        }
                        commit(queue);
                    } else {
                        release();
                    }
                }
            }
        }

        ConsumerAccessor & operator=(const ConsumerAccessor &) = delete;

        ConsumerAccessor & operator=(ConsumerAccessor && other) noexcept {
            assert(m_slot != other.m_slot || (!m_slot && !other.m_slot));
            if (this == std::addressof(other)) {
                return *this;
            }
            if (m_slot) {
                assert(m_slot->m_state.load(MemOrd::acquire) == State::ConsLocked);
                release();
            }
            if (other.m_slot) {
                assert(other.m_slot->m_state.load(MemOrd::acquire) == State::ConsLocked);
                m_queue = std::move(other.m_queue);
                m_slot = other.m_slot;
                other.reset();
            } else {
                reset();
            }
            return *this;
        }

        [[nodiscard, maybe_unused]]
        const Payload * operator->() const noexcept {
            assert(m_queue);
            assert(m_slot);
            assert(m_slot->m_state.load(MemOrd::acquire) == State::ConsLocked);
            return std::addressof(m_slot->m_payload);
        }

        [[nodiscard, maybe_unused]]
        const Payload & operator*() const noexcept {
            assert(m_queue);
            assert(m_slot);
            assert(m_slot->m_state.load(MemOrd::acquire) == State::ConsLocked);
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            assert((m_queue && m_slot) || (!m_queue && !m_slot));
            assert(!m_slot || m_slot->m_state.load(MemOrd::acquire) == State::ConsLocked);
            return m_slot;
        }

    protected:
        ConsumerAccessor(Pointer queuePtr, Slot * slot) noexcept
        : Completion {}, m_queue { std::move(queuePtr) }, m_slot { slot } {
            assert(m_queue);
            assert(m_slot);
        }
    };

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    DynamicMpmcQueue<T, S, C, H, O, E, A, G>::DynamicMpmcQueue(KeyTag, ThrowingTag, const int maxBlocks)
    : m_head { std::make_shared<Block>() }, m_tail { m_head.get() },
      m_capacity { S }, m_available { maxBlocks - 1 }, m_free { S } {
        assert(m_available.load(MemOrd::acquire) >= 0);
        auto firstSlot = m_head->assemble();
        m_producer.m_cursor.store(firstSlot, MemOrd::release);
        m_consumer.m_cursor.store(firstSlot, MemOrd::release);
        m_producer.m_enable.test_and_set(MemOrd::acquire);
        m_consumer.m_enable.test_and_set(MemOrd::acquire);
    }

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    DynamicMpmcQueue<T, S, C, H, O, E, A, G>::DynamicMpmcQueue(KeyTag, NonThrowingTag, const int maxBlocks) noexcept
    : m_available { maxBlocks } {
        assert(m_available.load(MemOrd::acquire) > 0);
        init<true>();
    }

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    template<bool I>
    bool DynamicMpmcQueue<T, S, C, H, O, E, A, G>::init() noexcept(I) {
        assert(m_available.load(MemOrd::acquire) > 0);
        Block * head { nullptr };
        Slot * firstSlot { nullptr };
        if constexpr (I) {
            try {
                m_head = std::make_shared<Block>();
                head = m_head.get();
                firstSlot = head->assemble();
            } catch (...) {
                m_head.reset();
                return false;
            }
        } else {
            m_head = std::make_shared<Block>();
            head = m_head.get();
            firstSlot = head->assemble();
        }
        m_tail = head;
        m_producer.m_cursor.store(firstSlot, MemOrd::release);
        m_consumer.m_cursor.store(firstSlot, MemOrd::release);
        m_available.fetch_sub(1, MemOrd::acq_rel);
        m_capacity.store(S, MemOrd::release);
        m_free.store(S, MemOrd::release);
        m_producer.m_enable.test_and_set(MemOrd::acquire);
        m_consumer.m_enable.test_and_set(MemOrd::acquire);
        return true;
    }

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    bool DynamicMpmcQueue<T, S, C, H, O, E, A, G>::grow() noexcept(c_noExceptAccess) {
        std::scoped_lock lock { m_spinLock };

        if (m_free.load(MemOrd::acquire)) {
            return true;
        }

        auto lastSlot = m_tail->lastSlot();
        try {
            m_tail->m_next = std::make_unique<Block>();
            lastSlot->m_next = m_tail->m_next->assemble(lastSlot->m_next);
        } catch (...) {
            lastSlot->m_next = m_head->firstSlot();
            m_tail->m_next.reset();
            if constexpr (!c_noExceptAccess) {
                throw;
            }
            return false;
        }

        m_tail = m_tail->m_next.get();
        m_available.fetch_sub(1, MemOrd::acq_rel);
        m_capacity.fetch_add(S, MemOrd::release);
        m_free.fetch_add(S, MemOrd::acq_rel);
        m_producer.m_cursor.store(lastSlot->m_next, MemOrd::release);
        return true;
    }

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    auto
    DynamicMpmcQueue<T, S, C, H, O, E, A, G>::producerSlot(unsigned acquireAttempts)
    noexcept(c_noExceptAccess) -> ProducerAccessor {
        assert(acquireAttempts > 0);

        if constexpr (c_noExceptConstr) {
            if (m_capacity.load(MemOrd::acquire) == 0) {
                std::scoped_lock lock { m_spinLock };
                if (!init<c_noExceptAccess>()) {
                    return {};
                }
            }
        }

        if (!m_producer.m_enable.test(MemOrd::acquire)) {
            return {};
        }
        if (m_free.load(MemOrd::acquire) == 0 && (m_available.load(MemOrd::acquire) == 0 || !grow())) {
            return {};
        }

        // v3 {{{
        // auto current = m_producerCursor.load(MemOrd::acquire);
        // }}} v3
        for (;;) {
            for (auto count = m_capacity.load(MemOrd::acquire); count; --count) {
                auto state = State::Free;
            // v1 {{{
                // auto current = m_producerCursor.exchange(m_producerCursor.load(MemOrd::acquire)->m_next, MemOrd::acq_rel);
            // }}} v1
            // v2 {{{
                auto current = m_producer.m_cursor.load(MemOrd::acquire);
                m_producer.m_cursor.store(current->m_next, MemOrd::release);
            // }}} v2
            // v3 {{{
                // m_producerCursor.compare_exchange_strong(current, current->m_next, MemOrd::acq_rel, MemOrd::acquire);
            // }}} v3
                if (current->m_state.compare_exchange_strong(state, State::ProdLocked, MemOrd::acq_rel, MemOrd::acquire)) {
                    return { this->shared_from_this(), current };
                }
                if (!m_producer.m_enable.test(MemOrd::acquire)) {
                    return {};
                }
                if constexpr (G == GrowthPolicy::Step) {
                    if (m_free.load(MemOrd::acquire) == 0 && (m_available.load(MemOrd::acquire) == 0 || !grow())) {
                        return {};
                    }
                }
            }
            if (!--acquireAttempts) {
                return {};
            }
            if constexpr (G == GrowthPolicy::Round) {
                if (m_free.load(MemOrd::acquire) == 0 && (m_available.load(MemOrd::acquire) == 0 || !grow())) {
                    return {};
                }
            }
        }
    }

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    auto DynamicMpmcQueue<T, S, C, H, O, E, A, G>::consumerSlot() noexcept -> ConsumerAccessor {
        if constexpr (c_noExceptConstr) {
            if (m_capacity.load(MemOrd::acquire) == 0) {
                return {};
            }
        }

        // v3 {{{
        // auto current = m_consumerCursor.load(MemOrd::acquire);
        // }}} v3
        while (
            m_consumer.m_enable.test(MemOrd::acquire)
            && m_free.load(MemOrd::acquire) < m_capacity.load(MemOrd::acquire)
        ) {
            auto state = State::Ready;
        // v1 {{{
            // auto current = m_consumerCursor.exchange(m_consumerCursor.load(MemOrd::acquire)->m_next, MemOrd::acq_rel);
        // }}} v1
        // v2 {{{
            auto current = m_consumer.m_cursor.load(MemOrd::acquire);
            m_consumer.m_cursor.store(current->m_next, MemOrd::release);
        // }}} v2
        // v3 {{{
            // m_consumerCursor.compare_exchange_strong(current, current->m_next, MemOrd::acq_rel, MemOrd::acquire);
        // }}} v3
            if (current->m_state.compare_exchange_strong(state, State::ConsLocked, MemOrd::acq_rel, MemOrd::acquire)) {
                return { this->shared_from_this(), current };
            }
        }

        return {};
    }

    template<std::default_initializable T, int S, bool C, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    auto DynamicMpmcQueue<T, S, C, H, O, E, A, G>::clear() noexcept(c_noExceptAccess) -> std::pair<SizeType, SizeType> {
        if (m_producer.m_enable.test(MemOrd::acquire) || m_consumer.m_enable.test(MemOrd::acquire)) {
            return { 0, 0 };
        }

        std::pair<SizeType, SizeType> result {};
        auto block = m_head.get();

        while (block) {
            for (size_t index {}; index < S; ++index ) {
                auto state = block->m_slots[index].m_state.load(MemOrd::acquire);
                if (state == State::Ready) {
                    if constexpr (c_handleSlot) {
                        clear(block->m_slots[index].m_payload);
                    }
                    block->m_slots[index].m_state.store(State::Free, MemOrd::release);
                    m_free.fetch_add(1, MemOrd::acq_rel);
                    ++result.first;
                } else if (state != State::Free) {
                    ++result.second;
                }
            }
            block = block->m_next.get();
        }

        return result;
    }

    template<AnyDynamicMpmcQueue T, typename ... Args>
    std::shared_ptr<T> make(Args ... args) noexcept(T::c_noExceptConstr) {
        using TheThrowingTag = std::conditional_t<T::c_noExceptConstr, NonThrowingTag, ThrowingTag>;
        if constexpr (T::c_noExceptConstr) {
            try {
                return std::make_shared<T>(typename T::KeyTag {}, TheThrowingTag {}, args ...);
            } catch (...) {
                return nullptr;
            }
        } else {
            return std::make_shared<T>(typename T::KeyTag {}, TheThrowingTag {}, args ...);
        }
    }
}
