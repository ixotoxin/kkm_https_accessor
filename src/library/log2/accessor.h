// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "record.h"
#include "queue.h"
#include "state.h"
#include "writers.h"
#include <variant>

namespace Log {
    class RecordAccessor {
    protected:
        Record & m_record;
        std::unique_lock<std::recursive_mutex> m_lock;

    public:
        RecordAccessor() = delete;
        RecordAccessor(const RecordAccessor &) = delete;
        RecordAccessor(RecordAccessor &&) noexcept = default;
        explicit RecordAccessor(Record & record) noexcept : m_record { record }, m_lock { s_mutex } {}
        ~RecordAccessor() = default;

        RecordAccessor & operator=(const RecordAccessor &) = delete;
        RecordAccessor & operator=(RecordAccessor &&) = delete;

        /*[[nodiscard, maybe_unused]] Record * operator->() const noexcept {
            return std::addressof(m_record);
        }*/

        [[nodiscard, maybe_unused]] Record & operator*() const noexcept {
            return m_record;
        }

        [[nodiscard, maybe_unused]] explicit operator bool() const noexcept {
            return true;
        }

        void complete() const noexcept {
            write(m_record);
        }
    };

    using RecordVariant = std::variant<RecordAccessor, LoggerQueue::ProducerAccessor>;

    inline auto RecordReady = [] (auto & accessor) -> bool { return static_cast<bool>(accessor); };
    inline auto RecordRef = [] (auto & accessor) -> Record & { return *accessor; };
    inline auto RecordWrite = [] (auto & accessor) -> void { accessor.complete(); };
}
