// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <filesystem>

namespace Config {
    CONF_VAR(std::filesystem::path, s_directory, );
    CONF_VAR(std::filesystem::path, s_file, );
}
