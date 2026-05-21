// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "server_static_defaults.h"
#include <unordered_map>
#include <filesystem>

namespace Server::Static {
    inline bool s_enable { c_enableStatic };
    inline std::filesystem::path s_directory { c_staticDirectory };
    inline std::string s_indexFile { c_indexFile };
    inline std::filesystem::path s_mimeMapFile { c_mimeMapFile };
    inline std::unordered_map<std::wstring, std::string> s_mimeMap {};
    inline bool s_enableUnknownType { c_enableUnknownType };
}
