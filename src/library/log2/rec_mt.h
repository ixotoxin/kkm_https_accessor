// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "record.h"
#include "queue.h"

namespace Log {
    class RecordAccessorMt : public RecordAccessor {
        LoggerQueue::ProducerAccessor m_slotAccessor;

    public:
        RecordAccessorMt() = delete;
        RecordAccessorMt(const RecordAccessorMt &) = delete;
        RecordAccessorMt(RecordAccessorMt &&) = delete;
        explicit RecordAccessorMt(LoggerQueue & queue) : RecordAccessor(), m_slotAccessor { queue.producerSlot() } {}
        ~RecordAccessorMt() override = default;

        RecordAccessorMt & operator=(const RecordAccessorMt &) = delete;
        RecordAccessorMt & operator=(RecordAccessorMt &&) = delete;

        [[nodiscard]]
        Record * operator->() noexcept override {
            return std::addressof(*m_slotAccessor);
        }

        [[nodiscard]]
        Record & operator*() noexcept override {
            return *std::addressof(*m_slotAccessor);
        }

        [[nodiscard]]
        explicit operator bool() noexcept override {
            return static_cast<bool>(m_slotAccessor);
        }

        void write() noexcept override {
            m_slotAccessor.complete();
        }
    };
}
