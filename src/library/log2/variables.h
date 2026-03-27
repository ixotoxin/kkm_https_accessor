// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "defaults.h"
#include <filesystem>

namespace Log {
    LOG_MVARI(bool, s_enableAsync, c_enableAsyncDef);
    LOG_MVARI(size_t, s_lineSize, c_lineSizeDef);
    LOG_MVARI(size_t, s_blocksNumber, c_blocksNumberDef);

#ifdef EXTERNAL_LOG_VARIABLES
    extern bool s_appendLocation;
#else
#   ifdef DEBUG
    inline bool s_appendLocation { true };
#   else
    inline bool s_appendLocation { false };
#   endif
#endif

    namespace Console {
        LOG_MVARI(bool, s_terse, c_terseDef);
        LOG_MVARI(Output, s_output, c_outputDef);
        LOG_MVARI(bool, s_flushEveryWrite, c_flushEveryWrite);
        LOG_MVARI(LevelUnderlying, s_level, c_level);
    }

    namespace File {
        LOG_CVARI(std::wstring_view, c_filenameFormat, c_filenameFormatDef);
        LOG_MVARI(std::filesystem::path, s_directory, );
        LOG_MVARI(bool, s_flushEveryWrite, c_flushEveryWrite);
        LOG_MVARI(LevelUnderlying, s_fgLevel, c_fgLevel);
        LOG_MVARI(LevelUnderlying, s_bgLevel, c_bgLevel);
    }

    namespace EventLog {
        LOG_CVARI(wchar_t *, c_eventSource, c_eventSourceDef);
        LOG_MVARI(LevelUnderlying, s_fgLevel, c_fgLevel);
        LOG_MVARI(LevelUnderlying, s_bgLevel, c_bgLevel);
    }
}
