// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "spinlock.h"
#include <cassert>
#include <new>
#include <type_traits>
#include <concepts>
#include <atomic>
#include <array>

namespace MtHelp {
    enum class QueueGrowthPolicy { Call, Round, Step };
    enum class QueueSlotState { Free, ProdLocked, Ready, ConsLocked };

#ifdef __cpp_lib_hardware_interference_size
    constexpr size_t c_queueAlignment { std::hardware_constructive_interference_size };
#else
    constexpr size_t c_queueAlignment { 64 };
#endif

    constexpr int c_queueDefaultBlockSize { 1024 };
    constexpr int c_queueDefaultBlocksNumberLimit { 16 };
    constexpr bool c_queueDefaultAutoCompletion { true };
    constexpr unsigned c_queueDefaultAcquireAttempts { 5 };

    class QueueSlotAutoCompletion {
    public:
        static constexpr bool c_autoComplete { true };

        QueueSlotAutoCompletion() noexcept = default;
        QueueSlotAutoCompletion(const QueueSlotAutoCompletion &) noexcept = default;
        QueueSlotAutoCompletion(QueueSlotAutoCompletion &&) noexcept = default;
        ~QueueSlotAutoCompletion() = default;

        QueueSlotAutoCompletion & operator=(const QueueSlotAutoCompletion &) = delete;
        QueueSlotAutoCompletion & operator=(QueueSlotAutoCompletion &&) = delete;

        [[maybe_unused]]
        void complete() noexcept {} // NOLINT
    };

    class QueueSlotManualCompletion {
    protected:
        bool m_complete { false };

    public:
        static constexpr bool c_autoComplete { false };

        QueueSlotManualCompletion() noexcept = default;
        QueueSlotManualCompletion(const QueueSlotManualCompletion & other) noexcept = default;
        QueueSlotManualCompletion(QueueSlotManualCompletion &&) noexcept = default;
        ~QueueSlotManualCompletion() = default;

        QueueSlotManualCompletion & operator=(const QueueSlotManualCompletion &) = delete;
        QueueSlotManualCompletion & operator=(QueueSlotManualCompletion &&) = delete;

        [[maybe_unused]]
        void complete() noexcept {
            m_complete = true;
        }
    };

    template<bool C>
    using QueueSlotCompletion = std::conditional_t<C, QueueSlotAutoCompletion, QueueSlotManualCompletion>;

    template<
        std::default_initializable T,
        class U = void,
        int S = c_queueDefaultBlockSize,
        bool C = c_queueDefaultAutoCompletion,
        unsigned A = c_queueDefaultAcquireAttempts,
        QueueGrowthPolicy G = QueueGrowthPolicy::Round
    >
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    class alignas(c_queueAlignment) Queue {
    protected:
        using MO = std::memory_order;
        using SlotState = QueueSlotState;
        using SlotCompletion = QueueSlotCompletion<C>;
        struct Slot;
        struct Block;

        static constexpr bool c_ntdct = std::is_nothrow_default_constructible_v<T>;
        Block * m_firstBlock;
        Block * m_lastBlock;
        std::atomic<Slot *> m_producerCursor { nullptr };
        std::atomic<Slot *> m_consumerCursor { nullptr };
        std::atomic_int_fast32_t m_capacity { S };
        std::atomic_int_fast32_t m_free { S };
        int m_blocksNumberLimit;
        std::atomic_bool m_producing { true };
        std::atomic_bool m_consuming { true };
        SpinLock<Spin::YieldThread> m_spinlock {};

        bool grow() noexcept;

    public:
        using Payload = T;
        using Interface = std::conditional_t<std::is_same_v<U, void>, T, U>;
        using SizeType = decltype(m_capacity)::value_type;

        class ProducerAccessor;
        class ConsumerAccessor;

        explicit Queue(int = c_queueDefaultBlocksNumberLimit) noexcept(c_ntdct);
        Queue(const Queue &) = delete;
        Queue(Queue &&) = delete;
        ~Queue();

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

        [[nodiscard]] ProducerAccessor producerSlot(unsigned = A) noexcept;
        [[nodiscard]] ConsumerAccessor consumerSlot(unsigned = A) noexcept;

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

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    struct Queue<T, U, S, C, A, G>::Slot {
        Payload m_payload {};
        Slot * m_next { nullptr };
        std::atomic<SlotState> m_state { SlotState::Free };

        Slot() noexcept(c_ntdct) = default;
        Slot(const Slot &) = delete;
        Slot(Slot &&) = delete;
        ~Slot() = default;

        Slot & operator=(const Slot &) = delete;
        Slot & operator=(Slot &&) = delete;
    };

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    struct Queue<T, U, S, C, A, G>::Block {
        std::array<Slot, static_cast<size_t>(S)> m_slots {};
        Block * m_next { nullptr };

        Block() noexcept(c_ntdct);
        Block(const Block &) = delete;
        Block(Block &&) = delete;
        explicit Block(Block * &) noexcept(c_ntdct);

        ~Block() {
            delete m_next;
        }

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

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    Queue<T, U, S, C, A, G>::Block::Block() noexcept(c_ntdct) {
        auto it = m_slots.begin();
        auto last = m_slots.end() - 1;

        while (it != last) {
            auto current = it++;
            current->m_next = &(*it);
        }

        last->m_next = &m_slots[0];
    }

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    Queue<T, U, S, C, A, G>::Block::Block(Block * & lastBlock) noexcept(c_ntdct) {
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
    }

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    class Queue<T, U, S, C, A, G>::ProducerAccessor : public SlotCompletion {
    protected:
        Queue & m_queue;
        Slot * m_slot;

    public:
        ProducerAccessor() = delete;
        ProducerAccessor(const ProducerAccessor &) = delete;

        ProducerAccessor(ProducerAccessor && other) noexcept
        : SlotCompletion { other }, m_queue { other.m_queue }, m_slot { other.m_slot } {
            other.m_slot = nullptr;
        }

        ProducerAccessor(Queue & queue, Slot * slot) noexcept
        : SlotCompletion {}, m_queue { queue }, m_slot { slot } {
            if (slot) {
                queue.m_free.fetch_sub(1, MO::acq_rel);
            }
        }

        ~ProducerAccessor() {
            if (m_slot) {
                if constexpr (SlotCompletion::c_autoComplete) {
                    m_slot->m_state.store(SlotState::Ready, MO::release);
                } else {
                    if (SlotCompletion::m_complete) {
                        m_slot->m_state.store(SlotState::Ready, MO::release);
                    } else {
                        m_queue.m_free.fetch_add(1, MO::acq_rel);
                        m_slot->m_state.store(SlotState::Free, MO::release);
                    }
                }
            }
        }

        ProducerAccessor & operator=(const ProducerAccessor &) = delete;
        ProducerAccessor & operator=(ProducerAccessor &&) = delete;

        [[nodiscard, maybe_unused]]
        Interface * operator->() const noexcept {
            return std::addressof(m_slot->m_payload);
        }

        [[nodiscard, maybe_unused]]
        Interface & operator*() const noexcept {
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            return m_slot && m_slot->m_state.load(MO::acquire) == SlotState::ProdLocked;
        }
    };

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    class Queue<T, U, S, C, A, G>::ConsumerAccessor : public SlotCompletion {
    protected:
        Queue & m_queue;
        Slot * m_slot;

    public:
        ConsumerAccessor() = delete;
        ConsumerAccessor(const ConsumerAccessor &) = delete;

        ConsumerAccessor(ConsumerAccessor && other) noexcept
        : SlotCompletion { other }, m_queue { other.m_queue }, m_slot { other.m_slot } {
            other.m_slot = nullptr;
        }

        ConsumerAccessor(Queue & queue, Slot * slot) noexcept
        : SlotCompletion {}, m_queue { queue }, m_slot { slot } {}

        ~ConsumerAccessor() {
            if (m_slot) {
                if constexpr (SlotCompletion::c_autoComplete) {
                    m_queue.m_free.fetch_add(1, MO::acq_rel);
                    m_slot->m_state.store(SlotState::Free, MO::release);
                } else {
                    if (SlotCompletion::m_complete) {
                        m_queue.m_free.fetch_add(1, MO::acq_rel);
                        m_slot->m_state.store(SlotState::Free, MO::release);
                    } else {
                        m_slot->m_state.store(SlotState::Ready, MO::release);
                    }
                }
            }
        }

        ConsumerAccessor & operator=(const ConsumerAccessor &) = delete;
        ConsumerAccessor & operator=(ConsumerAccessor &&) = delete;

        [[nodiscard, maybe_unused]]
        const Interface * operator->() const noexcept {
            return std::addressof(m_slot->m_payload);
        }

        [[nodiscard, maybe_unused]]
        const Interface & operator*() const noexcept {
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            return m_slot && m_slot->m_state.load(MO::acquire) == SlotState::ConsLocked;
        }
    };

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    Queue<T, U, S, C, A, G>::Queue(const int blocksNumberLimit) noexcept(c_ntdct)
    : m_firstBlock { new Block }, m_lastBlock { m_firstBlock },
      m_blocksNumberLimit { blocksNumberLimit - 1 } {
        assert(m_blocksNumberLimit >= 0);
        Slot * firstSlot { m_firstBlock->firstSlot() };
        m_producerCursor.store(firstSlot, MO::relaxed);
        m_consumerCursor.store(firstSlot, MO::relaxed);
    }

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    Queue<T, U, S, C, A, G>::~Queue() {
        delete m_firstBlock;
    }

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    auto Queue<T, U, S, C, A, G>::producerSlot(unsigned acquireAttempts) noexcept -> ProducerAccessor {
        if (!m_free.load(MO::acquire) && !grow()) {
            return { *this, nullptr };
        }

        if (acquireAttempts > 0) {
            --acquireAttempts;
        }

        Slot * current { m_producerCursor.exchange(m_producerCursor.load(MO::acquire)->m_next, MO::acq_rel) };
        const Slot * sentinel { current };

        while (m_producing.load(MO::relaxed)) {
            auto slotState { SlotState::Free };
            if (current->m_state.compare_exchange_strong(slotState, SlotState::ProdLocked, MO::acq_rel, MO::acquire)) {
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

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    auto Queue<T, U, S, C, A, G>::consumerSlot(unsigned acquireAttempts) noexcept -> ConsumerAccessor {
        if (acquireAttempts > 0) {
            --acquireAttempts;
        }

        Slot * current { m_consumerCursor.exchange(m_consumerCursor.load(MO::acquire)->m_next, MO::acq_rel) };
        const Slot * sentinel { current };

        while (m_consuming.load(MO::relaxed) && m_free.load(MO::acquire) != m_capacity.load(MO::acquire)) {
            auto slotState { SlotState::Ready };
            if (current->m_state.compare_exchange_strong(slotState, SlotState::ConsLocked, MO::acq_rel, MO::acquire)) {
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

    template<std::default_initializable T, class U, int S, bool C, unsigned A, QueueGrowthPolicy G>
    requires ((std::is_same_v<U, void> || std::derived_from<T, U>) && S >= 4 && A > 0)
    bool Queue<T, U, S, C, A, G>::grow() noexcept {
        ScopedLock lock { m_spinlock };

        if (m_free.load(MO::acquire)) {
            return true;
        }
        if (!m_blocksNumberLimit) {
            return false;
        }

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
        --m_blocksNumberLimit;

        return true;
    }

    template<class T>
    concept MpmcQueue = requires(T t) {
        []
        <std::default_initializable U, class V, int S, bool C, unsigned A, QueueGrowthPolicy G>
        (Queue<U, V, S, C, A, G> &)
        {}
        (t);
    };
}
