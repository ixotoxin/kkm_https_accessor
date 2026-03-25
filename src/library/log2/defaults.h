// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <lib/winapi.h>
#include <string_view>

namespace Log {
    LOG_CONST(bool, c_enableAsyncDef, true);
    LOG_CONST(size_t, c_lineSizeDef, 384);
    LOG_CONST(size_t, c_lineSizeMin, 64);
    LOG_CONST(size_t, c_lineSizeMax, 1024);
    LOG_CONST(size_t, c_blockSize, 1024);
    LOG_CONST(size_t, c_blocksNumberDef, 16);
    LOG_CONST(size_t, c_blocksNumberMin, 1);
    LOG_CONST(size_t, c_blocksNumberMax, 64);
    LOG_CONST(size_t, c_acquireAttempts, 3);

    namespace Console {
        LOG_CONST(bool, c_terseDef, true);
    }

    namespace File {
        LOG_CONST(std::wstring_view, c_directoryDef, L"logs");
        LOG_CONST(std::wstring_view, c_filenameFormatDef, L"kkmha-{:04d}-{:02d}.log");
    }

    namespace EventLog {
        LOG_CONST(const wchar_t *, c_eventSourceDef, L"KKM HTTPS Accessor");
        LOG_CONST(::DWORD, c_eventId, 0);
        LOG_CONST(::WORD, c_eventCategory, 0);
    }
}
