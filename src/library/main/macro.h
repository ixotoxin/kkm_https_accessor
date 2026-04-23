// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cmake/options.h>

#define MAIN_CONST(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }

#ifdef EXTERNAL_MAIN_VARIABLES
#   define MAIN_CVARI(TYPE, NAME, VALUE) extern const TYPE NAME
#   define MAIN_CVARE(TYPE, NAME, VALUE) const TYPE NAME { VALUE }
#   define MAIN_MVARI(TYPE, NAME, VALUE) extern TYPE NAME
#   define MAIN_MVARE(TYPE, NAME, VALUE) TYPE NAME { VALUE }
#else
#   define MAIN_CVARI(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }
#   define MAIN_CVARE(TYPE, NAME, VALUE)
#   define MAIN_MVARI(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#   define MAIN_MVARE(TYPE, NAME, VALUE)
#endif

// CLEANUP

#ifdef EXTERNAL_MAIN_STRINGS
// #   define MAIN_CSTRI(NAME, VALUE) extern const char * const NAME
// #   define MAIN_CSTRE(NAME, VALUE) const char * const NAME { VALUE }
// #   define MAIN_WCSTRI(NAME, VALUE) extern const wchar_t * const NAME
// #   define MAIN_WCSTRE(NAME, VALUE) const wchar_t * const NAME { VALUE }
#   define MAIN_STRI(NAME, VALUE) extern const std::string_view NAME
#   define MAIN_STRE(NAME, VALUE) const std::string_view NAME { VALUE }
#   define MAIN_WSTRI(NAME, VALUE) extern const std::wstring_view NAME
#   define MAIN_WSTRE(NAME, VALUE) const std::wstring_view NAME { VALUE }
// #   define MAIN_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
// #   define MAIN_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
// #   define MAIN_CSTRI(NAME, VALUE) constexpr const char * NAME { VALUE }
// #   define MAIN_CSTRE(NAME, VALUE)
// #   define MAIN_WCSTRI(NAME, VALUE) constexpr const wchar_t * NAME { VALUE }
// #   define MAIN_WCSTRE(NAME, VALUE)
#   define MAIN_STRI(NAME, VALUE) constexpr std::string_view NAME { VALUE }
#   define MAIN_STRE(NAME, VALUE)
#   define MAIN_WSTRI(NAME, VALUE) constexpr std::wstring_view NAME { VALUE }
#   define MAIN_WSTRE(NAME, VALUE)
// #   define MAIN_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
// #   define MAIN_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#endif
