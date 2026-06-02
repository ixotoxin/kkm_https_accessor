// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <log2/variables.h>

namespace Log {
    LOG_MVARE(size_t, s_lineSize, c_lineSizeDef);
#ifndef SINGLE_THREAD
    LOG_MVARE(bool, s_enableAsync, c_enableAsyncDef);
    LOG_MVARE(size_t, s_blocksNumber, c_blocksNumberDef);
#endif

#ifdef DEBUG
    bool s_appendLocation { true };
#else
    bool s_appendLocation { false };
#endif

#ifndef DISABLE_CONSOLE_LOGGING
    namespace Console {
        LOG_MVARE(bool, s_terse, c_terseDef);
        LOG_MVARE(Output, s_output, c_outputDef);
        LOG_MVARE(bool, s_flushEveryWrite, c_flushEveryWriteDef);
        LOG_MVARE(LevelUnderlying, s_level, c_levelDef);
    }
#endif

    namespace File {
        LOG_MVARE(std::filesystem::path, s_directory, );
        LOG_MVARE(bool, s_flushEveryWrite, c_flushEveryWriteDef);
        LOG_MVARE(LevelUnderlying, s_fgLevel, c_fgLevelDef);
        LOG_MVARE(LevelUnderlying, s_bgLevel, c_bgLevelDef);
    }

    namespace EventLog {
        LOG_MVARE(LevelUnderlying, s_fgLevel, c_fgLevelDef);
        LOG_MVARE(LevelUnderlying, s_bgLevel, c_bgLevelDef);
    }
}
