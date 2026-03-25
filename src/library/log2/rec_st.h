// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "record.h"
#include "state.h"
#include "writers.h"
#include <mutex>

namespace Log {
    class RecordAccessorSt : public RecordAccessor {
        Record & m_record;

    public:
        RecordAccessorSt() = delete;
        RecordAccessorSt(const RecordAccessorSt &) = delete;
        RecordAccessorSt(RecordAccessorSt &&) = delete;
        explicit RecordAccessorSt(Record & record) : RecordAccessor(), m_record { record } {}
        ~RecordAccessorSt() override = default;

        RecordAccessorSt & operator=(const RecordAccessorSt &) = delete;
        RecordAccessorSt & operator=(RecordAccessorSt &&) = delete;

        [[nodiscard]]
        Record * operator->() noexcept override {
            return std::addressof(m_record);
        }

        [[nodiscard]]
        Record & operator*() noexcept override {
            return m_record;
        }

        [[nodiscard]]
        explicit operator bool() noexcept override {
            return true;
        }

        void write() noexcept override {
            std::scoped_lock lock { s_mutex };
            ::Log::write(m_record);
        }
    };
}
