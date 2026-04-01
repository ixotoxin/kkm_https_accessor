// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifdef SINGLE_THREAD
#   error Not required
#endif

#include "record.h"
#include "writers.h"
#include <lib/concurrency.h>

namespace Log {
    class RecordAccessor {
        Record & m_record;

    public:
        RecordAccessor() = delete;
        RecordAccessor(const RecordAccessor &) = delete;
        RecordAccessor(RecordAccessor &&) noexcept = default;
        explicit RecordAccessor(Record & record) noexcept : m_record { record } {}
        ~RecordAccessor() = default;

        RecordAccessor & operator=(const RecordAccessor &) = delete;
        RecordAccessor & operator=(RecordAccessor &&) = delete;

        [[nodiscard, maybe_unused]]
        Record * operator->() const noexcept {
            return std::addressof(m_record);
        }

        [[nodiscard, maybe_unused]]
        Record & operator*() const noexcept {
            return m_record;
        }

        [[nodiscard, maybe_unused]]
        explicit operator bool() const noexcept {
            return true;
        }

        void complete() const noexcept {
            write(m_record);
        }
    };

    template<Ccy::Locker T>
    class ExclusiveAccessor : public RecordAccessor {
        std::unique_lock<T> m_lock;

    public:
        ExclusiveAccessor() = delete;
        ExclusiveAccessor(const ExclusiveAccessor &) = delete;
        ExclusiveAccessor(ExclusiveAccessor &&) noexcept = default;
        ExclusiveAccessor(Record & record, T & mutex) noexcept : RecordAccessor(record), m_lock { mutex } {}
        ~ExclusiveAccessor() = default;

        ExclusiveAccessor & operator=(const ExclusiveAccessor &) = delete;
        ExclusiveAccessor & operator=(ExclusiveAccessor &&) = delete;
    };
}
