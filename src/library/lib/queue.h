// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "spinlock.h"
#include <cassert>
#include <new>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <concepts>
#include <atomic>
#include <array>

namespace MtHelp {
    enum class QueueGrowthPolicy { Call, Round, Step };
    enum class QueueSlotState { Free, ProdLocked, Ready, ConsLocked };

#ifdef __cpp_lib_hardware_interference_size
    constexpr std::size_t c_queueAlignment { std::hardware_constructive_interference_size };
#else
    constexpr std::size_t c_queueAlignment { 64 };
#endif

    constexpr int32_t c_queueDefaultBlockSize [[maybe_unused]] { 0x10 };
    constexpr int32_t c_queueDefaultCapacityLimit [[maybe_unused]] { c_queueDefaultBlockSize * 0x1'0000 };
    constexpr int32_t c_queueMaxCapacityLimit [[maybe_unused]] { std::numeric_limits<int32_t>::max() };
    constexpr bool c_queueDefaultCompletion [[maybe_unused]] { true };
    constexpr unsigned c_queueDefaultAttempts [[maybe_unused]] { 5 };
    constexpr unsigned c_queueMaxAttempts [[maybe_unused]] { std::numeric_limits<unsigned>::max() };

    class AutoCompletion {
    public:
        static constexpr bool c_autoComplete [[maybe_unused]] { true };

        AutoCompletion() noexcept = default;
        AutoCompletion(const AutoCompletion &) = delete;
        AutoCompletion(AutoCompletion &&) = delete;
        ~AutoCompletion() noexcept = default;

        AutoCompletion & operator=(const AutoCompletion &) = delete;
        AutoCompletion & operator=(AutoCompletion &&) = delete;

        [[maybe_unused]]
        void complete() noexcept {}
    };

    class ManualCompletion {
    protected:
        bool m_complete { false };

    public:
        static constexpr bool c_autoComplete [[maybe_unused]] { false };

        ManualCompletion() noexcept = default;
        ManualCompletion(const ManualCompletion &) = delete;
        ManualCompletion(ManualCompletion &&) = delete;
        ~ManualCompletion() = default;

        ManualCompletion & operator=(const ManualCompletion &) = delete;
        ManualCompletion & operator=(ManualCompletion &&) = delete;

        [[maybe_unused]]
        void complete() noexcept {
            m_complete = true;
        }
    };

    template<bool C = c_queueDefaultCompletion>
    using QueueSlotCompletion = std::conditional_t<C, AutoCompletion, ManualCompletion>;

    template<
        std::default_initializable T,
        int32_t S = c_queueDefaultBlockSize,
        int32_t L = c_queueDefaultCapacityLimit,
        bool C = c_queueDefaultCompletion,
        unsigned A = c_queueDefaultAttempts,
        QueueGrowthPolicy G = QueueGrowthPolicy::Round
    >
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    class alignas(c_queueAlignment) Queue {
        struct Slot;
        struct Block;
        using SlotCompletion = QueueSlotCompletion<C>;
        class ProducerAccessor;
        class ConsumerAccessor;
        using MO = std::memory_order;
        using State = QueueSlotState;

        static constexpr bool c_ntdct = std::is_nothrow_default_constructible_v<T>;
        Block * m_firstBlock;
        Block * m_lastBlock;
        std::atomic<Slot *> m_producerCursor { nullptr };
        std::atomic<Slot *> m_consumerCursor { nullptr };
        std::atomic_int_fast32_t m_capacity { S };
        std::atomic_int_fast32_t m_free { S };
        std::atomic_bool m_producing { true };
        std::atomic_bool m_consuming { true };
        SpinLock<Spin::YieldThread> m_spinlock {};

        bool grow() noexcept;

    public:
        using PayloadType [[maybe_unused]] = T;
        using SizeType = decltype(m_capacity)::value_type;

        static constexpr SizeType c_blockSize [[maybe_unused]] { S };
        static constexpr SizeType c_maxCapacity [[maybe_unused]] { L };
        static constexpr bool c_autoComplete [[maybe_unused]] { C };
        static constexpr unsigned c_defaultAttempts [[maybe_unused]] { A };
        static constexpr QueueGrowthPolicy c_growthPolicy [[maybe_unused]] { G };

        Queue() noexcept(c_ntdct);
        Queue(const Queue &) = delete;
        Queue(Queue &&) = delete;
        ~Queue() { delete m_firstBlock; }

        Queue & operator=(const Queue &) = delete;
        Queue & operator=(Queue &&) = delete;

        [[nodiscard, maybe_unused]]
        SizeType capacity() const noexcept {
            return m_capacity.load(MO::relaxed);
        }

        [[nodiscard, maybe_unused]]
        SizeType freeSlots() const noexcept {
            return m_free.load(MO::relaxed);
        }

        [[nodiscard, maybe_unused]]
        bool empty() const noexcept {
            return m_free.load(MO::acquire) == m_capacity.load(MO::acquire);
        }

        [[nodiscard, maybe_unused]]
        bool producing() const noexcept {
            return m_producing.load(MO::relaxed);
        }

        [[nodiscard, maybe_unused]]
        bool consuming() const noexcept {
            return m_consuming.load(MO::relaxed);
        }

        [[nodiscard]] ProducerAccessor producerSlot(unsigned = c_defaultAttempts) noexcept;
        [[nodiscard]] ConsumerAccessor consumerSlot(unsigned = c_defaultAttempts) noexcept;

        [[maybe_unused]]
        void shutdown() noexcept {
            m_producing.store(false, MO::relaxed);
        }

        [[maybe_unused]]
        void stop() noexcept {
            m_producing.store(false, MO::relaxed);
            m_consuming.store(false, MO::relaxed);
        }
    };

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    struct Queue<T, S, L, C, A, G>::Slot {
        Slot * m_next { nullptr };
        std::atomic<State> m_state { State::Free };
        T m_payload {};

        Slot() noexcept(c_ntdct) = default;
        Slot(const Slot &) = delete;
        Slot(Slot &&) = delete;
        ~Slot() = default;

        Slot & operator=(const Slot &) = delete;
        Slot & operator=(Slot &&) = delete;
    };

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    struct Queue<T, S, L, C, A, G>::Block {
        std::array<Slot, static_cast<size_t>(S)> m_slots {};
        Block * m_next { nullptr };

        Block() noexcept(c_ntdct);
        Block(const Block &) = delete;
        Block(Block &&) = delete;
        explicit Block(Block * &) noexcept(c_ntdct);
        ~Block() { delete m_next; }

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
    };

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    Queue<T, S, L, C, A, G>::Block::Block() noexcept(c_ntdct) {
        auto it = m_slots.begin();
        auto last = m_slots.end() - 1;

        while (it != last) {
            auto current = it++;
            current->m_next = &(*it);
        }

        last->m_next = &m_slots[0];
    }

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    Queue<T, S, L, C, A, G>::Block::Block(Block * & lastBlock) noexcept(c_ntdct) {
        assert(lastBlock);
        auto it = m_slots.begin();
        auto last = m_slots.end() - 1;

        while (it != last) {
            auto current = it++;
            current->m_next = &(*it);
        }

        auto tail = lastBlock->lastSlot();
        last->m_next = tail->m_next;
        tail->m_next = &m_slots[0];
        lastBlock->m_next = this;

        /** It is guaranteed that no exceptions will be thrown after modifying the object pointed to by last_block. **/
    }

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    class Queue<T, S, L, C, A, G>::ProducerAccessor : public SlotCompletion {
    protected:
        Queue & m_queue;
        Slot * const m_slot;

    public:
        ProducerAccessor() = delete;
        ProducerAccessor(const ProducerAccessor &) = delete;
        ProducerAccessor(ProducerAccessor &&) = delete;

        ProducerAccessor(Queue & queue, Slot * slot) noexcept
        : SlotCompletion {}, m_queue { queue }, m_slot { slot } {
            if (slot) {
                queue.m_free.fetch_sub(1, MO::acq_rel);
            }
        }

        ~ProducerAccessor();

        ProducerAccessor & operator=(const ProducerAccessor &) = delete;
        ProducerAccessor & operator=(ProducerAccessor &&) = delete;

        [[nodiscard, maybe_unused]]
        T * operator->() noexcept {
            return &m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        T & operator*() noexcept {
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() noexcept {
            return m_slot && m_slot->m_state.load(MO::acquire) == State::ProdLocked;
        }
    };

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    Queue<T, S, L, C, A, G>::ProducerAccessor::~ProducerAccessor() {
        if (m_slot) {
            if constexpr (SlotCompletion::c_autoComplete) {
                m_slot->m_state.store(State::Ready, MO::release);
            } else {
                if (SlotCompletion::m_complete) {
                    m_slot->m_state.store(State::Ready, MO::release);
                } else {
                    m_queue.m_free.fetch_add(1, MO::acq_rel);
                    m_slot->m_state.store(State::Free, MO::release);
                }
            }
        }
    }

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    class Queue<T, S, L, C, A, G>::ConsumerAccessor : public SlotCompletion {
    protected:
        Queue & m_queue;
        Slot * const m_slot;

    public:
        ConsumerAccessor() = delete;
        ConsumerAccessor(const ConsumerAccessor &) = delete;
        ConsumerAccessor(ConsumerAccessor &&) = delete;

        ConsumerAccessor(Queue & queue, Slot * slot) noexcept
        : SlotCompletion {}, m_queue { queue }, m_slot { slot } {}

        ~ConsumerAccessor();

        ConsumerAccessor & operator=(const ConsumerAccessor &) = delete;
        ConsumerAccessor & operator=(ConsumerAccessor &&) = delete;

        [[nodiscard, maybe_unused]]
        const T * operator->() noexcept {
            return &m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        const T & operator*() noexcept {
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() noexcept {
            return m_slot && m_slot->m_state.load(MO::acquire) == State::ConsLocked;
        }
    };

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    Queue<T, S, L, C, A, G>::ConsumerAccessor::~ConsumerAccessor() {
        if (m_slot) {
            if constexpr (SlotCompletion::c_autoComplete) {
                m_queue.m_free.fetch_add(1, MO::acq_rel);
                m_slot->m_state.store(State::Free, MO::release);
            } else {
                if (SlotCompletion::m_complete) {
                    m_queue.m_free.fetch_add(1, MO::acq_rel);
                    m_slot->m_state.store(State::Free, MO::release);
                } else {
                    m_slot->m_state.store(State::Ready, MO::release);
                }
            }
        }
    }

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    Queue<T, S, L, C, A, G>::Queue() noexcept(c_ntdct)
    : m_firstBlock { new Block }, m_lastBlock { m_firstBlock } {
        Slot * first_slot { m_firstBlock->first_slot() };
        m_producerCursor.store(first_slot, MO::relaxed);
        m_consumerCursor.store(first_slot, MO::relaxed);
    }

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    auto Queue<T, S, L, C, A, G>::producerSlot(unsigned acquireAttempts) noexcept -> ProducerAccessor {
        if (!m_free.load(MO::acquire) && !grow()) {
            return { *this, nullptr };
        }

        if (acquireAttempts > 0) {
            --acquireAttempts;
        }

        Slot * current { m_producerCursor.exchange(m_producerCursor.load(MO::acquire)->m_next, MO::acq_rel) };
        const Slot * sentinel { current };

        while (m_producing.load(MO::relaxed)) {
            auto slotState { State::Free };
            if (current->m_state.compare_exchange_strong(slotState, State::ProdLocked, MO::acq_rel, MO::acquire)) {
                return { *this, current };
            }
            current = m_producerCursor.exchange(m_producerCursor.load(MO::acquire)->m_next, MO::acq_rel);

            if (current == sentinel) {
                if (acquireAttempts < 1) {
                    break;
                }
                --acquireAttempts;
                if constexpr (G == QueueGrowthPolicy::Round) {
                    if (!m_free.load(MO::acquire) && !grow()) {
                        return { *this, nullptr };
                    }
                }
            }
            if constexpr (G == QueueGrowthPolicy::Step) {
                if (!m_free.load(MO::acquire) && !grow()) {
                    return { *this, nullptr };
                }
            }
        }
        return { *this, nullptr };
    }

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    auto Queue<T, S, L, C, A, G>::consumerSlot(unsigned acquireAttempts) noexcept -> ConsumerAccessor {
        if (acquireAttempts > 0) {
            --acquireAttempts;
        }

        Slot * current { m_consumerCursor.exchange(m_consumerCursor.load(MO::acquire)->m_next, MO::acq_rel) };
        const Slot * sentinel { current };

        while (m_consuming.load(MO::relaxed) && m_free.load(MO::acquire) != m_capacity.load(MO::acquire)) {
            auto slotState { State::Ready };
            if (current->m_state.compare_exchange_strong(slotState, State::ConsLocked, MO::acq_rel, MO::acquire)) {
                return { *this, current };
            }
            current = m_consumerCursor.exchange(m_consumerCursor.load(MO::acquire)->m_next, MO::acq_rel);

            if (current == sentinel) {
                if (acquireAttempts < 1) {
                    break;
                }
                --acquireAttempts;
            }
        }

        return { *this, nullptr };
    }

    template<std::default_initializable T, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((S >= 4) && (L <= c_queueMaxCapacityLimit) && (S <= L) && (A > 0) && (A <= c_queueMaxAttempts))
    bool Queue<T, S, L, C, A, G>::grow() noexcept {
        ScopedLock lock { m_spinlock };

        if (m_free.load(MO::acquire)) {
            return true;
        }
        if (m_capacity.load(MO::acquire) + S > L) {
            return false;
        }

        /** This object will not be modified if an exception is thrown. **/

        if constexpr (c_ntdct) {
            auto newBlock = new(std::nothrow) Block { m_lastBlock };
            if (!newBlock) {
                return false;
            }
            m_lastBlock = newBlock;
        } else {
            try {
                auto newBlock = new(std::nothrow) Block { m_lastBlock };
                if (!newBlock) {
                    return false;
                }
                m_lastBlock = newBlock;
            } catch (...) {
                return false;
            }
        }

        m_capacity.fetch_add(S, MO::release);
        m_free.fetch_add(S, MO::acq_rel);

        return true;
    }

    template<class T>
    concept MpmcQueue = requires(T t) {
        [] <std::default_initializable U, int32_t S, int32_t L, bool C, unsigned A, QueueGrowthPolicy G>
        (Queue<U, S, L, C, A, G> &) {} (t);
    };
}
