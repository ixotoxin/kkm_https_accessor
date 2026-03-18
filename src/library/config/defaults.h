// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <string_view>

namespace Config {
    CONF_CONST(std::wstring_view, c_directoryEnv, L"kkmha_conf_dir");
    CONF_CONST(std::wstring_view, c_directory, L"conf");
    CONF_CONST(std::wstring_view, c_file, L"kkmha.json");
}
