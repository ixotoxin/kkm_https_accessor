// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "strings.h"
#include <lib/except.h>
#include <ccy/dynrespool.h>
#include <rapidjson/allocators.h>
#include <rapidjson/document.h>
#include <atomic>

namespace Json {
    constexpr int c_allocatorsBlockSize { 16 };
    constexpr int c_maxAllocatorsBlocks { 512 };

    using MemoryPoolAllocatorBase = rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>;

    class MemoryPoolAllocator : public MemoryPoolAllocatorBase {
    public:
        MemoryPoolAllocator() noexcept = default;
        MemoryPoolAllocator(const MemoryPoolAllocator &) = delete;
        MemoryPoolAllocator(MemoryPoolAllocator &&) = delete;
        ~MemoryPoolAllocator() = default;

        MemoryPoolAllocator &operator=(const MemoryPoolAllocator &) = delete;
        MemoryPoolAllocator &operator=(MemoryPoolAllocator &&) = delete;
    };

    using AllocatorsPoolBase = Ccy::DynamicResourcePool<MemoryPoolAllocator, c_allocatorsBlockSize, true, true>;

    class AllocatorsPool : public AllocatorsPoolBase {
        std::atomic_uint64_t m_allocatorsInUse {};
        std::atomic_uint64_t m_peakAllocatorsInUse {};

    public:
        AllocatorsPool() = delete;
        AllocatorsPool(const AllocatorsPool &) = delete;
        AllocatorsPool(AllocatorsPool &&) = delete;

        AllocatorsPool(KeyTag, Ccy::ThrowingTag, const int maxBlocks)
        : AllocatorsPoolBase(KeyTag {}, Ccy::ThrowingTag {}, maxBlocks) {}

        AllocatorsPool(KeyTag, Ccy::NonThrowingTag, const int maxBlocks) noexcept
        : AllocatorsPoolBase(KeyTag {}, Ccy::NonThrowingTag {}, maxBlocks) {}

        ~AllocatorsPool() override = default;

        AllocatorsPool &operator=(const AllocatorsPool &) = delete;
        AllocatorsPool &operator=(AllocatorsPool &&) = delete;

        std::pair<uint64_t, uint64_t> stat() const {
            using Ccy::MemOrd;
            return { m_allocatorsInUse.load(MemOrd::acquire), m_peakAllocatorsInUse.load(MemOrd::acquire) };
        }

    protected:
        bool prepare(Payload &) noexcept(c_noExceptAccess) override {
            using Ccy::MemOrd;
            const auto allocatorsInUse = 1 + m_allocatorsInUse.fetch_add(1, MemOrd::acq_rel);
            auto peakAllocatorsInUse = m_peakAllocatorsInUse.load(MemOrd::acquire); // NOLINT
            if (allocatorsInUse > peakAllocatorsInUse) {
                m_peakAllocatorsInUse.compare_exchange_weak(
                    peakAllocatorsInUse, allocatorsInUse,
                    MemOrd::acq_rel, MemOrd::acquire
                );
            }
            return true;
        }

        bool clear(Payload & payload) noexcept(c_noExceptAccess) override {
            using Ccy::MemOrd;
            payload.Clear();
            m_allocatorsInUse.fetch_sub(1, MemOrd::acq_rel);
            return true;
        }
    };

    class AllocatorBase {
    protected:
        inline static auto s_pool = Ccy::make<AllocatorsPool>(c_maxAllocatorsBlocks);

    public:
        AllocatorBase() = default;
        AllocatorBase(const AllocatorBase &) = default;
        AllocatorBase(AllocatorBase &&) = default;
        virtual ~AllocatorBase() = default;

        AllocatorBase &operator=(const AllocatorBase &) = default;
        AllocatorBase &operator=(AllocatorBase &&) = default;

        static bool ready() {
            return static_cast<bool>(s_pool);
        }

        static std::pair<uint64_t, uint64_t> poolStat() {
            return s_pool->stat();
        }
    };

    class Allocator : public AllocatorBase {
        using AllocatorBase::s_pool;
        AllocatorsPool::Accessor m_allocator;

    public:
        Allocator() : m_allocator { s_pool->acquire() } {
            using namespace std::string_view_literals;
            if (!static_cast<bool>(m_allocator)) {
                throw Basic::Failure(Wcs::c_cannotAcquireAllocator);
            }
        }

        Allocator(const Allocator &) = delete;
        Allocator(Allocator &&) = delete;
        ~Allocator() override = default;

        Allocator &operator=(const Allocator &) = delete;
        Allocator &operator=(Allocator &&) = delete;

        // NOLINTNEXTLINE(readability-make-member-function-const)
        void * Malloc (const size_t size) {
            return m_allocator->Malloc(size);
        }

        // NOLINTNEXTLINE(readability-make-member-function-const)
        void * Realloc (void * originalPtr, const size_t originalSize, const size_t newSize) {
            return m_allocator->Realloc(originalPtr, originalSize, newSize);
        }

        static void Free(void *) RAPIDJSON_NOEXCEPT {}

        static const bool kNeedFree { false }; // NOLINT
        static const bool kRefCounted { true }; // NOLINT
    };

    class SharedAllocator : public AllocatorBase {
        using AllocatorBase::s_pool;
        std::shared_ptr<AllocatorsPool::Accessor> m_allocator;

    public:
        SharedAllocator() : m_allocator { std::make_shared<AllocatorsPool::Accessor>(s_pool->acquire()) } {
            using namespace std::string_view_literals;
            if (!static_cast<bool>(m_allocator)) {
                throw Basic::Failure(Wcs::c_cannotAcquireAllocator);
            }
        }

        SharedAllocator(const SharedAllocator &) = default;
        SharedAllocator(SharedAllocator &&) = default;
        ~SharedAllocator() override = default;

        SharedAllocator &operator=(const SharedAllocator &) = default;
        SharedAllocator &operator=(SharedAllocator &&) = default;

        // NOLINTNEXTLINE(readability-make-member-function-const)
        void * Malloc (const size_t size) {
            if (!static_cast<bool>(m_allocator)) {
                throw Basic::Failure(Wcs::c_cannotAcquireAllocator);
            }
            return (*m_allocator)->Malloc(size);
        }

        // NOLINTNEXTLINE(readability-make-member-function-const)
        void * Realloc (void * originalPtr, const size_t originalSize, const size_t newSize) {
            if (!static_cast<bool>(m_allocator)) {
                throw Basic::Failure(Wcs::c_cannotAcquireAllocator);
            }
            return (*m_allocator)->Realloc(originalPtr, originalSize, newSize);
        }

        static void Free(void *) RAPIDJSON_NOEXCEPT {}

        static const bool kNeedFree { false }; // NOLINT
        static const bool kRefCounted { false }; // NOLINT
    };
}
