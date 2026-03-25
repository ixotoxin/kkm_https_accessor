// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <filesystem>

namespace Main {
    MAIN_MVARI(std::filesystem::path, s_file, );
    MAIN_MVARI(std::filesystem::path, s_directory, );
}
