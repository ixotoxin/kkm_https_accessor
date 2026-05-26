// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cmake/options.h>
#include <string_view>

#if !BUILD_SEPARATED
namespace KkmJsonLoader::Wcs {
    using Csv = const std::wstring_view;

    constexpr Csv c_usage1 {
        L"    exec {сн} {вф}      Выполнить операцию описанную в файле (json)\n"
    };

    constexpr Csv c_usage2 {
        L"    {вф}                Входной файл в JSON-формате\n"
        L"    (json)              Вывод в JSON-формате\n"
    };
}
#endif
