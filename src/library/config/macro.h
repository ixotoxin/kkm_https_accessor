// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#define CONF_CONST(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }

#ifdef EXTERNAL_CONF_VARIABLES
#   define CONF_CVARI(TYPE, NAME, VALUE) extern const TYPE NAME
#   define CONF_CVARE(TYPE, NAME, VALUE) const TYPE NAME { VALUE }
#   define CONF_MVARI(TYPE, NAME, VALUE) extern TYPE NAME
#   define CONF_MVARE(TYPE, NAME, VALUE) TYPE NAME { VALUE }
#else
#   define CONF_CVARI(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }
#   define CONF_CVARE(TYPE, NAME, VALUE)
#   define CONF_MVARI(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#   define CONF_MVARE(TYPE, NAME, VALUE)
#endif

// CLEANUP

#ifdef EXTERNAL_CONF_STRINGS
// #   define CONF_CSTRI(NAME, VALUE) extern const char * const NAME
// #   define CONF_CSTRE(NAME, VALUE) const char * const NAME { VALUE }
// #   define CONF_WCSTRI(NAME, VALUE) extern const wchar_t * const NAME
// #   define CONF_WCSTRE(NAME, VALUE) const wchar_t * const NAME { VALUE }
#   define CONF_STRI(NAME, VALUE) extern const std::string_view NAME
#   define CONF_STRE(NAME, VALUE) const std::string_view NAME { VALUE }
#   define CONF_WSTRI(NAME, VALUE) extern const std::wstring_view NAME
#   define CONF_WSTRE(NAME, VALUE) const std::wstring_view NAME { VALUE }
// #   define CONF_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
// #   define CONF_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
// #   define CONF_CSTRI(NAME, VALUE) constexpr const char * NAME { VALUE }
// #   define CONF_CSTRE(NAME, VALUE)
// #   define CONF_WCSTRI(NAME, VALUE) constexpr const wchar_t * NAME { VALUE }
// #   define CONF_WCSTRE(NAME, VALUE)
#   define CONF_STRI(NAME, VALUE) constexpr std::string_view NAME { VALUE }
#   define CONF_STRE(NAME, VALUE)
#   define CONF_WSTRI(NAME, VALUE) constexpr std::wstring_view NAME { VALUE }
#   define CONF_WSTRE(NAME, VALUE)
// #   define CONF_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
// #   define CONF_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#endif
