// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "variables.h"
#include <string>

namespace Log {
    struct Record {
        std::wstring m_message {};
        std::wstring_view m_terse {};
        Level m_level { Level::Debug };
        bool m_toConsole { false };
        bool m_toFile { false };
        bool m_toEventLog { false };

        Record() noexcept { m_message.reserve(s_lineSize); }
        Record(const Record &) = delete;
        Record(Record &&) = delete;
        virtual ~Record() = default;

        Record & operator=(const Record &) = delete;
        Record & operator=(Record &&) = delete;
    };

    class RecordAccessor {
    public:
        RecordAccessor() = default;
        RecordAccessor(const RecordAccessor &) = delete;
        RecordAccessor(RecordAccessor &&) = delete;
        virtual ~RecordAccessor() = default;

        RecordAccessor & operator=(const RecordAccessor &) = delete;
        RecordAccessor & operator=(RecordAccessor &&) = delete;

        [[nodiscard]] virtual Record * operator->() noexcept = 0;
        [[nodiscard]] virtual Record & operator*() noexcept = 0;
        [[nodiscard]] virtual explicit operator bool() noexcept = 0;

        virtual void write() noexcept = 0;
    };
}
