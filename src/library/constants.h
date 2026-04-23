// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

constexpr size_t c_xsStrSize { 80 };
constexpr size_t c_sStrSize { 4 * c_xsStrSize };
constexpr size_t c_mStrSize { 4 * c_sStrSize };
constexpr size_t c_lStrSize { 4 * c_mStrSize };
constexpr size_t c_xlStrSize { 4 * c_lStrSize };

namespace Log {
    namespace File {
        constexpr std::wstring_view c_kkmHttpAccessorFilenameFormat { L"kkmha-{:04d}-{:02d}.log" };
        constexpr std::wstring_view c_kkmOperatorFilenameFormat { L"kkmop-{:04d}-{:02d}.log" };
        constexpr std::wstring_view c_kkmJsonLoaderFilenameFormat { L"kkmjl-{:04d}-{:02d}.log" };
    }

    namespace EventLog {
        constexpr const wchar_t * c_kkmHttpAccessor { L"KKM HTTPS Accessor" };
        constexpr const wchar_t * c_kkmOperator { L"KKM Operator" };
        constexpr const wchar_t * c_kkmJsonLoader { L"KKM JSON Loader" };
    }
}
