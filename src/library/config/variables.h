// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "defaults.h"
#include <filesystem>

namespace Config {
    CONF_CVARI(std::wstring_view, c_directoryEnv, c_directoryEnvDef);
    CONF_CVARI(std::wstring_view, c_directory, c_directoryDef);
    CONF_CVARI(std::wstring_view, c_file, c_fileDef);
    CONF_MVARI(std::filesystem::path, s_directory, );
    CONF_MVARI(std::filesystem::path, s_file, );
}
