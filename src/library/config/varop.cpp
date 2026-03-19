// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "variables.h"
#include <iostream>

namespace Config {
    std::wostream & vars(std::wostream & stream) {
        stream
            << L"RTM: main.configDirectory = \"" << s_directory.wstring() << L"\"\n"
            L"RTM: main.configFile = \"" << s_file.wstring() << L"\"\n";

        return stream;
    }
}
