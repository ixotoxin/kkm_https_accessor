// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "record.h"
#include "variables.h"
#include <ccy/dynamique.h>

namespace Log {
    class LoggerQueue : public Ccy::DynamicMpmcQueue<Record, c_blockSize, true, true/*, c_acquireAttempts*/> {
    public:
        LoggerQueue(KeyTag, Ccy::ThrowingTag)
        : DynamicMpmcQueue(KeyTag {}, Ccy::ThrowingTag {}, static_cast<int>(s_blocksNumber)) {}

        // LoggerQueue(KeyTag, Ccy::NonThrowingTag) noexcept
        // : DynamicMpmcQueue(KeyTag {}, Ccy::NonThrowingTag {}, static_cast<int>(s_blocksNumber)) {}

        LoggerQueue(const LoggerQueue &) = delete;
        LoggerQueue(LoggerQueue &&) = delete;
        ~LoggerQueue() override = default;

        LoggerQueue & operator=(const LoggerQueue &) = delete;
        LoggerQueue & operator=(LoggerQueue &&) = delete;

    protected:
        // bool prepare(Payload &) noexcept(c_noExceptAccess) override { // NOLINT
        //     return true;
        // }

        bool clear(Payload & record) noexcept(c_noExceptAccess) override { // NOLINT
            record.m_message.clear();
            record.m_terseMsg1 = std::wstring_view {};
            record.m_terseMsg2 = std::wstring_view {};
            return true;
        }
    };
}
