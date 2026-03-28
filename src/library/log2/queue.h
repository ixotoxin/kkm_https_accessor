// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "record.h"
#include "variables.h"
#include <lib/queue.h>

namespace Log {
    class LoggerQueue : public Ccy::Queue<Record, void, c_blockSize, true, c_acquireAttempts> {
    public:
        LoggerQueue() : Queue(static_cast<int>(s_blocksNumber)) {}
        LoggerQueue(const LoggerQueue &) = delete;
        LoggerQueue(LoggerQueue &&) = delete;
        ~LoggerQueue() = default;

        LoggerQueue & operator=(const LoggerQueue &) = delete;
        LoggerQueue & operator=(LoggerQueue &&) = delete;
    };
}
