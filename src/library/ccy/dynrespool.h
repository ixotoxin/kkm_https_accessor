// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "algo.h"
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
    constexpr int c_poolDefaultBlockSize { 256 };
    constexpr int c_poolDefaultBlocksNumberLimit { 16 };
    constexpr bool c_poolDefaultSlotHandling { false };
    constexpr bool c_poolDefaultNoExceptConstr { false };
    constexpr bool c_poolDefaultNoExceptAccess { false };
    constexpr unsigned c_poolDefaultAcquireAttempts { 5 };

    template<
        std::default_initializable T,               /** Тип элемента пула **/
        int S = c_poolDefaultBlockSize,             /** Размер блока пула **/
        bool H = c_poolDefaultSlotHandling,         /** Вкл/выкл обработку элементов пула при доступе **/
        bool O = c_poolDefaultNoExceptConstr,       /** Вкл/выкл исключения при создании пула **/
        bool E = c_poolDefaultNoExceptAccess,       /** Вкл/выкл исключения при создании аксессора **/
        unsigned A = c_poolDefaultAcquireAttempts,  /** Количество попыток по-умолчанию захватить ресурс **/
        GrowthPolicy G = c_defaultGrowthPolicy      /** Политика роста пула **/
    >
    requires (S > 1 && A > 0)
    class alignas(c_hwCIS) DynamicResourcePool;

    template<class P>
    concept AnyDynamicResourcePool
        = requires(P p) {
            [] <std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
                (DynamicResourcePool<T, S, H, O, E, A, G> &) {} (p);
        };

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    class alignas(c_hwCIS) DynamicResourcePool
    : public std::enable_shared_from_this<DynamicResourcePool<T, S, H, O, E, A, G>> {
    protected:
        struct KeyTag {};
        using InternalSizeType = std::atomic_int_fast32_t;
        using InternalIndexType = std::atomic_uint_fast64_t;

        static constexpr bool c_handleSlot { H };
        static constexpr auto c_noExceptConstr = O;
        static constexpr auto c_noExceptAccess = E;
        static constexpr auto c_noExceptPayload = std::is_nothrow_default_constructible_v<T>;

    public:
        using Payload = T;
        using Pointer = std::shared_ptr<DynamicResourcePool>;
        using SizeType = InternalSizeType::value_type;
        using IndexType = InternalIndexType::value_type;
        class Accessor;

        template<AnyDynamicResourcePool U, typename ... Args>
        friend std::shared_ptr<U> make(Args ...) noexcept(U::c_noExceptConstr);

        DynamicResourcePool() = delete;
        DynamicResourcePool(KeyTag, ThrowingTag, int);
        DynamicResourcePool(KeyTag, NonThrowingTag, int) noexcept;
        DynamicResourcePool(const DynamicResourcePool &) = delete;
        DynamicResourcePool(DynamicResourcePool &&) = delete;
        virtual ~DynamicResourcePool() = default;

        DynamicResourcePool & operator=(const DynamicResourcePool &) = delete;
        DynamicResourcePool & operator=(DynamicResourcePool &&) = delete;

        [[nodiscard, maybe_unused]]
        SizeType capacity() const noexcept {
            return m_capacity.load(MemOrd::relaxed);
        }

        [[nodiscard, maybe_unused]]
        SizeType freeItems() const noexcept {
            return m_free.load(MemOrd::relaxed);
        }

        [[nodiscard, maybe_unused]]
        bool empty() const noexcept {
            return m_free.load(MemOrd::acquire) <= 0;
        }

        [[nodiscard]] Accessor acquire(unsigned = A) noexcept(c_noExceptAccess);

        template<WaitMethod W = c_defaultWaitMethod>
        [[maybe_unused]]
        void waitForAllReleased() const noexcept {
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
        bool waitForAllReleased(const std::chrono::steady_clock::duration timeout) const noexcept {
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

    protected:
        static constexpr IndexType c_invalidIndex = std::numeric_limits<IndexType>::max();

        virtual bool prepare(Payload &) noexcept(c_noExceptAccess) {
            return true;
        }

        virtual bool clear(Payload &) noexcept(c_noExceptAccess) {
            return true;
        }

    private:
        struct Block;
        using BlockPointer = std::shared_ptr<Block>;

        BlockPointer m_head { nullptr };
        Block * m_tail { nullptr };
        InternalSizeType m_capacity { 0 };
        InternalSizeType m_takenBlocks { 0 };
        InternalSizeType m_availableBlocks;
        SpinLock<> m_spinLock {};
        alignas(c_hwDIS) InternalSizeType m_free { 0 };
        alignas(c_hwDIS) std::atomic<Block *> m_blockCursor { nullptr };

        template<bool X> bool init() noexcept(X);
        bool grow() noexcept(c_noExceptAccess);
    };

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    struct alignas(c_hwCIS) DynamicResourcePool<T, S, H, O, E, A, G>::Block : std::enable_shared_from_this<Block> {
        std::atomic<BlockPointer> m_next { nullptr };
        alignas(c_hwDIS) InternalSizeType m_free { S };
        alignas(c_hwDIS) InternalIndexType m_index { 0 };
        alignas(c_hwDIS) std::atomic_flag m_flags[static_cast<size_t>(S)] {};
        alignas(c_hwDIS) Payload m_payload[static_cast<size_t>(S)] {};

        Block() noexcept(c_noExceptPayload) = default;
        Block(const Block &) = delete;
        Block(Block &&) = delete;
        ~Block() = default;

        Block & operator=(const Block &) = delete;
        Block & operator=(Block &&) = delete;
    };

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    class DynamicResourcePool<T, S, H, O, E, A, G>::Accessor {
        friend class DynamicResourcePool;

        Pointer m_pool { nullptr };
        BlockPointer m_block { nullptr };
        IndexType m_index { c_invalidIndex };

        void release(DynamicResourcePool * pool, Block * block) noexcept {
            block->m_flags[m_index].clear(MemOrd::release);
            block->m_free.fetch_add(1, MemOrd::acq_rel);
            pool->m_free.fetch_add(1, MemOrd::acq_rel);
        }

        void reset() noexcept {
            m_pool.reset();
            m_block.reset();
            m_index = c_invalidIndex;
        }

    public:
        Accessor() noexcept = default;
        Accessor(const Accessor &) = delete;

        Accessor(Accessor && other) noexcept
        : m_pool { std::move(other.m_pool) }, m_block { std::move(other.m_block) }, m_index { other.m_index } {
            assert(
                (!m_pool && !m_block && m_index == c_invalidIndex)
                || (m_pool && m_block && m_index < S && m_block->m_flags[m_index].test(MemOrd::acquire))
            );
            other.reset();
        }

        ~Accessor() {
            if (auto block = m_block.get(); block) {
                auto pool = m_pool.get();
                if constexpr (c_handleSlot) {
                    if constexpr (c_noExceptAccess) {
                        pool->clear(block->m_payload[m_index]);
                    } else {
                        try {
                            pool->clear(block->m_payload[m_index]);
                        } catch (...) {}
                    }
                }
                release(pool, block);
            }
        }

        Accessor & operator=(const Accessor &) = delete;

        Accessor & operator=(Accessor && other) noexcept {
            assert(m_block != other.m_block || m_index != other.m_index || (!m_block && !other.m_block));
            if (this == std::addressof(other)) {
                return *this;
            }
            if (auto block = m_block.get(); block) {
                release(m_pool.get(), block);
            }
            if (other.m_block) {
                assert(
                    other.m_pool && other.m_block && other.m_index < S
                    && other.m_block->m_flags[other.m_index].test(MemOrd::acquire)
                );
                m_pool = std::move(other.m_pool);
                m_block = std::move(other.m_block);
                m_index = other.m_index;
                other.reset();
            } else {
                reset();
            }
            return *this;
        }

        [[nodiscard, maybe_unused]]
        Payload * operator->() const noexcept {
            assert(m_pool && m_block && m_index < S && m_block->m_flags[m_index].test(MemOrd::acquire));
            return std::addressof(m_block->m_payload[m_index]);
        }

        [[nodiscard, maybe_unused]]
        Payload & operator*() const noexcept {
            assert(m_pool && m_block && m_index < S && m_block->m_flags[m_index].test(MemOrd::acquire));
            return m_block->m_payload[m_index];
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            assert(
                (!m_pool && !m_block && m_index == c_invalidIndex)
                || (m_pool && m_block && m_index < S && m_block->m_flags[m_index].test(MemOrd::acquire))
            );
            return static_cast<bool>(m_block);
        }

    protected:
        Accessor(Pointer poolPtr, BlockPointer blockPtr, IndexType const index) noexcept(c_noExceptAccess)
        : m_pool { std::move(poolPtr) }, m_block { std::move(blockPtr) }, m_index { index } {
            auto pool = m_pool.get();
            auto block = m_block.get();
            assert(pool);
            assert(block);
            assert(m_index < S);
            assert(block->m_flags[m_index].test(MemOrd::acquire));
            pool->m_free.fetch_sub(1, MemOrd::acq_rel);
            block->m_free.fetch_sub(1, MemOrd::acq_rel);
            if constexpr (c_handleSlot) {
                if constexpr (c_noExceptAccess) {
                    if (pool->prepare(block->m_payload[m_index])) {
                        return;
                    }
                } else {
                    try {
                        if (pool->prepare(block->m_payload[m_index])) {
                            return;
                        }
                    } catch (...) {
                        release(pool, block);
                        // reset();
                        throw;
                    }
                }
                release(pool, block);
                reset();
            }
        }
    };

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    DynamicResourcePool<T, S, H, O, E, A, G>::DynamicResourcePool(KeyTag, ThrowingTag, const int maxBlocks)
    : m_head { std::make_shared<Block>() }, m_tail { m_head.get() },
      m_capacity { S }, m_takenBlocks { 1 }, m_availableBlocks { maxBlocks - 1 },
      m_free { S }, m_blockCursor { m_tail } {
        assert(m_availableBlocks.load(MemOrd::acquire) >= 0);
    }

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    DynamicResourcePool<T, S, H, O, E, A, G>::DynamicResourcePool(KeyTag, NonThrowingTag, const int maxBlocks) noexcept
    : m_availableBlocks { maxBlocks } {
        assert(m_availableBlocks.load(MemOrd::acquire) > 0);
        init<true>();
    }

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    template<bool X>
    bool DynamicResourcePool<T, S, H, O, E, A, G>::init() noexcept(X) {
        assert(m_availableBlocks.load(MemOrd::acquire) > 0);
        if constexpr (X) {
            try {
                m_head = std::make_shared<Block>();
            } catch (...) {
                return false;
            }
        } else {
            m_head = std::make_shared<Block>();
        }
        m_tail = m_head.get();
        m_takenBlocks.fetch_add(1, MemOrd::acq_rel);
        m_availableBlocks.fetch_sub(1, MemOrd::acq_rel);
        m_capacity.store(S, MemOrd::release);
        m_free.store(S, MemOrd::release);
        m_blockCursor.store(m_tail, MemOrd::release);
        return true;
    }

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    bool DynamicResourcePool<T, S, H, O, E, A, G>::grow() noexcept(c_noExceptAccess) {
        assert(m_availableBlocks.load(MemOrd::acquire) > 0);

        std::scoped_lock lock { m_spinLock };

        if (m_free.load(MemOrd::acquire) > 0) {
            return true;
        }

        try {
            auto newBlock = std::make_shared<Block>();
            m_tail->m_next.store(newBlock, MemOrd::release);
            m_tail = newBlock.get();
        } catch (...) {
            if constexpr (!c_noExceptAccess) {
                throw;
            }
            return false;
        }

        m_takenBlocks.fetch_add(1, MemOrd::acq_rel);
        m_availableBlocks.fetch_sub(1, MemOrd::acq_rel);
        m_capacity.fetch_add(S, MemOrd::release);
        m_free.fetch_add(S, MemOrd::acq_rel);
        m_blockCursor.store(m_tail, MemOrd::release);
        return true;
    }

    template<std::default_initializable T, int S, bool H, bool O, bool E, unsigned A, GrowthPolicy G>
    requires (S > 1 && A > 0)
    auto DynamicResourcePool<T, S, H, O, E, A, G>::acquire(unsigned acquireAttempts)
    noexcept(c_noExceptAccess) -> Accessor {
        assert(acquireAttempts > 0);

        if constexpr (c_noExceptConstr) {
            if (m_takenBlocks.load(MemOrd::acquire) == 0) {
                std::scoped_lock lock { m_spinLock };
                if (!init<c_noExceptAccess>()) {
                    return {};
                }
            }
        }

        if (m_free.load(MemOrd::acquire) == 0 && (m_availableBlocks.load(MemOrd::acquire) == 0 || !grow())) {
            return {};
        }

        auto roundCount = m_takenBlocks.load(MemOrd::acquire);
        Block * block { m_blockCursor.load(MemOrd::acquire) };

        for (;;) {
            while (block->m_free.load(MemOrd::acquire) > 0) {
                auto index = iteratePostInc<S>(block->m_index);
                if (!block->m_flags[index].test_and_set(MemOrd::acq_rel)) {
                    return { this->shared_from_this(), block->shared_from_this(), index };
                }
            }

            if (!--roundCount) {
                if (!--acquireAttempts) {
                    return {};
                }
                if constexpr (G == GrowthPolicy::Round) {
                    if (m_free.load(MemOrd::acquire) == 0 && (m_availableBlocks.load(MemOrd::acquire) == 0 || !grow())) {
                        return {};
                    }
                }
                roundCount = m_takenBlocks.load(MemOrd::acquire);
            }
            if constexpr (G == GrowthPolicy::Step) {
                if (m_free.load(MemOrd::acquire) == 0 && (m_availableBlocks.load(MemOrd::acquire) == 0 || !grow())) {
                    return {};
                }
            }

            auto next = block->m_next.load(MemOrd::acquire).get();
            if (!next) {
                next = m_head.get();
            }
            if (m_blockCursor.compare_exchange_strong(block, next, MemOrd::acq_rel, MemOrd::acquire)) {
                block = next;
            }
        }
    }

    template<AnyDynamicResourcePool T, typename ... Args>
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
