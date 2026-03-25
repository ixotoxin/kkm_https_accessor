// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include <string_view>

namespace Config::Wcs {
    CONF_WSTRI(c_cantReadConfig, L"Не удалось прочитать конфигурационный файл '{}'");
    CONF_WSTRI(c_invalidConfig, L"Ошибка чтения конфигурации");
}
