// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "types.h"
#include <string_view>

namespace Log {
    LOG_CONST(size_t, c_lineSizeDef, 384);
    LOG_CONST(size_t, c_lineSizeMin, 64);
    LOG_CONST(size_t, c_lineSizeMax, 1024);
#ifndef SINGLE_THREAD
    LOG_CONST(bool, c_enableAsyncDef, true);
    LOG_CONST(size_t, c_blockSize, 1024);
    LOG_CONST(size_t, c_blocksNumberDef, 16);
    LOG_CONST(size_t, c_blocksNumberMin, 1);
    LOG_CONST(size_t, c_blocksNumberMax, 64);
    LOG_CONST(size_t, c_acquireAttempts, 3);
#endif

    namespace Console {
        LOG_CONST(bool, c_terseDef, true);
        LOG_CONST(Output, c_outputDef, Output::Separated);
        LOG_CONST(bool, c_flushEveryWriteDef, true);
        LOG_CONST(LevelUnderlying, c_levelDef, c_levelInfo);
    }

    namespace File {
        LOG_CONST(std::wstring_view, c_directoryDef, L"logs");
        LOG_CONST(bool, c_flushEveryWriteDef, true);
        LOG_CONST(LevelUnderlying, c_fgLevelDef, c_levelNone);
        LOG_CONST(LevelUnderlying, c_bgLevelDef, c_levelInfo);
    }

    namespace EventLog {
        LOG_CONST(LevelUnderlying, c_fgLevelDef, c_levelNone);
        LOG_CONST(LevelUnderlying, c_bgLevelDef, c_levelWarning);
    }
}
