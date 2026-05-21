// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "ccy_types.h"
#include <cassert>
#include <type_traits>
#include <concepts>
#include <atomic>
#include <array>

namespace Ccy {
    constexpr int c_queueDefaultBlockSize { 1024 };
    constexpr int c_queueDefaultBlocksNumberLimit { 16 };
    constexpr bool c_queueDefaultAutoCompletion { true };
    constexpr bool c_queueDefaultSlotHandling { false };
    // constexpr bool c_queueDefaultNoExceptHandling { false };
    // constexpr bool c_queueDefaultNoThrowAccess { false };
    constexpr unsigned c_queueDefaultAcquireAttempts { 5 };

    enum class QueueGrowthPolicy { Call, Round, Step };
    enum class QueueSlotState { Free, ProdLocked, Ready, ConsLocked };

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
        std::default_initializable T, /** Тип элемента очереди **/
        int S = c_queueDefaultBlockSize, /** Размер блока очереди **/
        bool C = c_queueDefaultAutoCompletion,
        bool H = c_queueDefaultSlotHandling,
        Except R = Except::Allow,
        bool O = c_queueDefaultNoThrowAccess,
        unsigned A = c_queueDefaultAcquireAttempts,
        QueueGrowthPolicy G = QueueGrowthPolicy::Round
    >
    requires (S >= 4 && A > 0)
    class alignas(c_alignment) MpmcQueue {
        using SlotState = QueueSlotState;
        using SlotCompletion = QueueSlotCompletion<C>;
        struct Slot;
        struct Block;

        static constexpr bool c_handleSlots { H };
        static constexpr bool c_noExceptHandling { N || !c_handleSlots };
        static constexpr bool c_noExceptConstruct { std::is_nothrow_default_constructible_v<T> && c_noExceptHandling };
        static constexpr bool c_noThrowAccess { O && !c_noExceptHandling };

        Block * m_head;
        Block * m_tail;
        std::atomic<Slot *> m_producerCursor { nullptr };
        std::atomic<Slot *> m_consumerCursor { nullptr };
        std::atomic_int_fast32_t m_capacity { S };
        std::atomic_int_fast32_t m_free { S };
        int m_blocksNumberLimit;
        std::atomic_flag m_producing {};
        std::atomic_flag m_consuming {};
        SpinLock<Spin::Pause> m_spinLock {};

        bool grow() noexcept;

    public:
        using Payload = T;
        using SizeType = decltype(m_capacity)::value_type;

        class ProducerAccessor;
        class ConsumerAccessor;

        explicit MpmcQueue(int = c_queueDefaultBlocksNumberLimit) noexcept(c_noExceptConstruct);
        MpmcQueue(const MpmcQueue &) = delete;
        MpmcQueue(MpmcQueue &&) = delete;
        virtual ~MpmcQueue();

        MpmcQueue & operator=(const MpmcQueue &) = delete;
        MpmcQueue & operator=(MpmcQueue &&) = delete;

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
            return m_free.load(MemOrd::acquire) == m_capacity.load(MemOrd::acquire);
        }

        [[nodiscard, maybe_unused]]
        bool producing() const noexcept {
            return m_producing.test(MemOrd::acquire);
        }

        [[nodiscard, maybe_unused]]
        bool consuming() const noexcept {
            return m_consuming.test(MemOrd::acquire);
        }

        [[nodiscard]] ProducerAccessor producerSlot(unsigned = A) noexcept(c_noExceptConstruct);
        [[nodiscard]] ConsumerAccessor consumerSlot(unsigned = A) noexcept;

        [[maybe_unused]]
        void shutdown() noexcept {
            m_producing.clear(MemOrd::release);
        }

        [[maybe_unused]]
        void stop() noexcept {
            m_producing.clear(MemOrd::release);
            m_consuming.clear(MemOrd::release);
        }

    protected:
        virtual void init(Payload &) noexcept(c_noExceptHandling) {}
        virtual void prepare(Payload &) noexcept(c_noExceptHandling) {}
        virtual void clean(Payload &) noexcept(c_noExceptHandling) {}
    };

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    struct MpmcQueue<T, S, C, H, N, O, A, G>::Slot {
        Payload m_payload {};
        Slot * m_next { nullptr };
        std::atomic<SlotState> m_state { SlotState::Free };

        Slot() noexcept(c_noExceptConstruct) = default;
        Slot(const Slot &) = delete;
        Slot(Slot &&) = delete;
        ~Slot() = default;

        Slot & operator=(const Slot &) = delete;
        Slot & operator=(Slot &&) = delete;
    };

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    struct MpmcQueue<T, S, C, H, N, O, A, G>::Block {
        std::array<Slot, static_cast<size_t>(S)> m_slots {};
        MpmcQueue * m_queue;
        Block * m_next { nullptr };

        Block() = delete;
        Block(const Block &) = delete;
        Block(Block &&) = delete;

        Block(HeadTag, MpmcQueue * queue) noexcept(c_noExceptConstruct)
        : m_queue { queue } {
            assert(m_queue);

            auto it = m_slots.begin();
            auto last = m_slots.end() - 1;
            while (it != last) {
                if constexpr (c_handleSlots) {
                    m_queue->init(it->m_payload);
                }
                auto current = it++;
                current->m_next = &*it;
            }
            if constexpr (c_handleSlots) {
                m_queue->init(it->m_payload);
            }

            last->m_next = &m_slots[0];
        }

        Block(TailTag, MpmcQueue * queue) noexcept(c_noExceptConstruct)
        : m_queue { queue } {
            assert(m_queue);

            auto it = m_slots.begin();
            auto last = m_slots.end() - 1;
            while (it != last) {
                if constexpr (c_handleSlots) {
                    m_queue->init(it->m_payload);
                }
                auto current = it++;
                current->m_next = &(*it);
            }
            if constexpr (c_handleSlots) {
                m_queue->init(it->m_payload);
            }

            auto currentTail = m_queue->m_tail->lastSlot();
            last->m_next = currentTail->m_next;
            currentTail->m_next = &m_slots[0];
            m_queue->m_tail->m_next = this;
            m_queue->m_tail = this;
        }

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

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    class MpmcQueue<T, S, C, H, N, O, A, G>::ProducerAccessor : public SlotCompletion {
        friend class MpmcQueue/*<T, S, C, H, N, O, A, G>*/;

        MpmcQueue * m_queue { nullptr };
        Slot * m_slot { nullptr };

        void swap(ProducerAccessor && other) noexcept {
            std::swap(m_queue, other.m_queue);
            std::swap(m_slot, other.m_slot);
        }

        void release() noexcept {
            if (m_queue) {
                swap(ProducerAccessor {});
            }
        }

    public:
        ProducerAccessor() noexcept = default;
        ProducerAccessor(const ProducerAccessor &) = delete;

        ProducerAccessor(ProducerAccessor && other) noexcept {
            swap(std::move(other));
        }

        ~ProducerAccessor() {
            if (m_slot) {
                if constexpr (SlotCompletion::c_autoComplete) {
                    m_slot->m_state.store(SlotState::Ready, MemOrd::release);
                } else {
                    if (SlotCompletion::m_complete) {
                        m_slot->m_state.store(SlotState::Ready, MemOrd::release);
                    } else {
                        m_slot->m_state.store(SlotState::Free, MemOrd::release);
                        m_queue->m_free.fetch_add(1, MemOrd::acq_rel);
                    }
                }
            }
        }

        ProducerAccessor & operator=(const ProducerAccessor &) = delete;

        ProducerAccessor & operator=(ProducerAccessor && other) noexcept {
            assert(m_slot != other.m_slot);
            if (this != std::addressof(other)) {
                release();
                swap(std::move(other));
            }
            return *this;
        }

        [[nodiscard, maybe_unused]]
        Payload * operator->() const noexcept {
            return std::addressof(m_slot->m_payload);
        }

        [[nodiscard, maybe_unused]]
        Payload & operator*() const noexcept {
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            return m_slot && m_slot->m_state.load(MemOrd::acquire) == SlotState::ProdLocked;
        }

    protected:
        ProducerAccessor(MpmcQueue * queue, Slot * slot) noexcept(c_noExceptA)
        : SlotCompletion {}, m_queue { queue }, m_slot { slot } {
            assert(m_queue);
            assert(m_slot);
            m_queue->m_free.fetch_sub(1, MemOrd::acq_rel);
            if constexpr (c_handleSlots) {
                if constexpr (c_noThrowAccess) {
                    m_queue->prepare(m_slot->m_payload);
                } else {
                    try {
                        m_queue->prepare(m_slot->m_payload);
                    } catch (...) {
                        m_slot->m_state.store(SlotState::Free, MemOrd::release);
                        m_queue->m_free.fetch_add(1, MemOrd::acq_rel);
                        m_queue = nullptr;
                        m_slot = nullptr;
                    }
                }
            }
        }
    };

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    class MpmcQueue<T, S, C, H, N, O, A, G>::ConsumerAccessor : public SlotCompletion {
        friend class MpmcQueue<T, S, C, H, N, O, A, G>;

        MpmcQueue * m_queue { nullptr };
        Slot * m_slot { nullptr };

        void swap(ConsumerAccessor && other) noexcept {
            std::swap(m_queue, other.m_queue);
            std::swap(m_slot, other.m_slot);
        }

        void release() noexcept {
            if (m_queue) {
                swap(ConsumerAccessor {});
            }
        }

    public:
        ConsumerAccessor() noexcept = default;
        ConsumerAccessor(const ConsumerAccessor &) = delete;

        ConsumerAccessor(ConsumerAccessor && other) noexcept {
            swap(std::move(other));
        }

        ~ConsumerAccessor() {
            if (m_slot) {
                if constexpr (SlotCompletion::c_autoComplete) {
                    try {
                        m_queue->clean(m_slot->m_payload);
                    } catch (...) {}
                    m_slot->m_state.store(SlotState::Free, MemOrd::release);
                    m_queue->m_free.fetch_add(1, MemOrd::acq_rel);
                } else {
                    if (SlotCompletion::m_complete) {
                        try {
                            m_queue->clean(m_slot->m_payload);
                        } catch (...) {}
                        m_slot->m_state.store(SlotState::Free, MemOrd::release);
                        m_queue->m_free.fetch_add(1, MemOrd::acq_rel);
                    } else {
                        m_slot->m_state.store(SlotState::Ready, MemOrd::release);
                    }
                }
            }
        }

        ConsumerAccessor & operator=(const ConsumerAccessor &) = delete;

        ConsumerAccessor & operator=(ConsumerAccessor && other) noexcept {
            assert(m_slot != other.m_slot);
            if (this != std::addressof(other)) {
                release();
                swap(std::move(other));
            }
            return *this;
        }

        [[nodiscard, maybe_unused]]
        const Payload * operator->() const noexcept {
            return std::addressof(m_slot->m_payload);
        }

        [[nodiscard, maybe_unused]]
        const Payload & operator*() const noexcept {
            return m_slot->m_payload;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            return m_slot && m_slot->m_state.load(MemOrd::acquire) == SlotState::ConsLocked;
        }

    protected:
        ConsumerAccessor(MpmcQueue * queue, Slot * slot) noexcept
        : SlotCompletion {}, m_queue { queue }, m_slot { slot } {
            assert(m_queue);
            assert(m_slot);
        }
    };

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    MpmcQueue<T, S, C, H, N, O, A, G>::MpmcQueue(const int blocksNumberLimit)
    : m_head { new Block { Block::head, this } }, m_tail { m_head },
      m_producerCursor { m_head->firstSlot() }, m_consumerCursor { m_producerCursor.load(MemOrd::relaxed) },
      m_blocksNumberLimit { blocksNumberLimit - 1 } {
        assert(m_blocksNumberLimit >= 0);
        m_producing.test_and_set();
        m_consuming.test_and_set();
    }

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    MpmcQueue<T, S, C, H, N, O, A, G>::~MpmcQueue() {
        delete m_head;
    }

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    auto MpmcQueue<T, S, C, H, N, O, A, G>::producerSlot(unsigned acquireAttempts) -> ProducerAccessor {
        if (!m_free.load(MemOrd::acquire) && !grow()) {
            return {};
        }

        if (acquireAttempts > 0) {
            --acquireAttempts;
        }

        Slot * current { m_producerCursor.exchange(m_producerCursor.load(MemOrd::acquire)->m_next, MemOrd::acq_rel) };
        const Slot * sentinel { current };

        while (m_producing.test(MemOrd::acquire)) {
            auto slotState { SlotState::Free };
            if (current->m_state.compare_exchange_strong(slotState, SlotState::ProdLocked, MemOrd::acq_rel, MemOrd::acquire)) {
                return { this, current };
            }
            current = m_producerCursor.exchange(m_producerCursor.load(MemOrd::acquire)->m_next, MemOrd::acq_rel);

            if (current == sentinel) {
                if (acquireAttempts < 1) {
                    break;
                }
                --acquireAttempts;
                if constexpr (G == QueueGrowthPolicy::Round) {
                    if (!m_free.load(MemOrd::acquire) && !grow()) {
                        return {};
                    }
                }
            }
            if constexpr (G == QueueGrowthPolicy::Step) {
                if (!m_free.load(MemOrd::acquire) && !grow()) {
                    return {};
                }
            }
        }
        return {};
    }

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    auto MpmcQueue<T, S, C, H, N, O, A, G>::consumerSlot(unsigned acquireAttempts) -> ConsumerAccessor {
        if (acquireAttempts > 0) {
            --acquireAttempts;
        }

        Slot * current { m_consumerCursor.exchange(m_consumerCursor.load(MemOrd::acquire)->m_next, MemOrd::acq_rel) };
        const Slot * sentinel { current };

        while (m_consuming.test(MemOrd::acquire) && m_free.load(MemOrd::acquire) != m_capacity.load(MemOrd::acquire)) {
            auto slotState { SlotState::Ready };
            if (current->m_state.compare_exchange_strong(slotState, SlotState::ConsLocked, MemOrd::acq_rel, MemOrd::acquire)) {
                return { this, current };
            }
            current = m_consumerCursor.exchange(m_consumerCursor.load(MemOrd::acquire)->m_next, MemOrd::acq_rel);

            if (current == sentinel) {
                if (acquireAttempts < 1) {
                    break;
                }
                --acquireAttempts;
            }
        }

        return {};
    }

    template<std::default_initializable T, int S, bool C, bool H, bool N, bool O, unsigned A, QueueGrowthPolicy G>
    requires (S >= 4 && A > 0)
    bool MpmcQueue<T, S, C, H, N, O, A, G>::grow() noexcept {
        std::scoped_lock lock { m_spinLock };

        if (m_free.load(MemOrd::acquire)) {
            return true;
        }
        if (!m_blocksNumberLimit) {
            return false;
        }

        if (!new(std::nothrow) Block { Block::tail, this }) {
            return false;
        }

        m_capacity.fetch_add(S, MemOrd::release);
        m_free.fetch_add(S, MemOrd::acq_rel);
        --m_blocksNumberLimit;

        return true;
    }
}
