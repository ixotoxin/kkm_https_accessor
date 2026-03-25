// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <main/variables.h>
#include <log2/variables.h>
#include <kkm/variables.h>
#include <config/variables.h>

namespace Main {
    MAIN_MVARE(std::filesystem::path, s_file, );
    MAIN_MVARE(std::filesystem::path, s_directory, );
}

namespace Config {
    CONF_CVARE(std::wstring_view, c_directoryEnv, c_directoryEnvDef);
    CONF_CVARE(std::wstring_view, c_directory, c_directoryDef);
    CONF_CVARE(std::wstring_view, c_file, c_fileDef);
    CONF_MVARE(std::filesystem::path, s_directory, );
    CONF_MVARE(std::filesystem::path, s_file, );
}

namespace Log {
    LOG_MVARE(bool, s_enableAsync, c_enableAsyncDef);
    LOG_MVARE(size_t, s_lineSize, c_lineSizeDef);
    LOG_MVARE(size_t, s_blocksNumber, c_blocksNumberDef);

#ifdef DEBUG
    bool s_appendLocation { true };
#else
    bool s_appendLocation { false };
#endif

    namespace Console {
        LOG_MVARE(bool, s_terse, c_terseDef);
        LOG_MVARE(LevelUnderlying, s_level, c_levelDebug);
    }

    namespace File {
        LOG_MVARE(std::filesystem::path, s_directory, );
        LOG_MVARE(LevelUnderlying, s_fgLevel, c_levelNone);
        LOG_MVARE(LevelUnderlying, s_bgLevel, c_levelInfo);
    }

    namespace EventLog {
        LOG_MVARE(LevelUnderlying, s_fgLevel, c_levelNone);
        LOG_MVARE(LevelUnderlying, s_bgLevel, c_levelInfo);
    }
}

namespace Kkm {
    KKM_MVARE(std::filesystem::path, s_dbDirectory, c_dbDirectoryDef);
    KKM_MVARE(std::wstring, s_defaultBaudRate, c_defaultBaudRateDef);
    KKM_MVARE(size_t, s_defaultLineLength, c_defaultLineLengthDef);
#if VERSION_LIMIT >= VERSION_10107
    KKM_MVARE(TimeZone, s_timeZone, TimeZone::Device);
    KKM_MVARE(bool, s_timeZoneConfigured, false);
#endif
    KKM_MVARE(FfdVersion, s_fallbackFfdVersion, c_fallbackFfdVersionDef);
    KKM_MVARE(FfdVersionDetect, s_ffdVersionDetect, c_ffdVersionDetectDef);
    KKM_MVARE(DateTime::SleepUnit, s_documentClosingTimeout, c_documentClosingTimeoutDef);
    KKM_MVARE(std::wstring, s_cliOperatorName, c_cliOperatorNameDef);
    KKM_MVARE(std::wstring, s_cliOperatorInn, );
    KKM_MVARE(std::wstring, s_customerAccountField, c_customerAccountFieldDef);
    KKM_MVARE(double, s_maxCashInOut, c_maxCashInOutDef);
    KKM_MVARE(double, s_maxPrice, c_maxPriceDef);
    KKM_MVARE(double, s_maxQuantity, c_maxQuantityDef);
}
