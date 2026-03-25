// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <string_view>

namespace Config {
    CONF_CONST(std::wstring_view, c_directoryEnvDef, L"kkmha_conf_dir");
    CONF_CONST(std::wstring_view, c_directoryDef, L"conf");
    CONF_CONST(std::wstring_view, c_fileDef, L"kkmha.json");
}
