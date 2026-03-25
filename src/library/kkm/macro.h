// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <cmake/options.h>

#define KKM_CONST(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }

#ifdef EXTERNAL_KKM_VARIABLES
#   define KKM_CVARI(TYPE, NAME, VALUE) extern const TYPE NAME
#   define KKM_CVARE(TYPE, NAME, VALUE) const TYPE NAME { VALUE }
#   define KKM_MVARI(TYPE, NAME, VALUE) extern TYPE NAME
#   define KKM_MVARE(TYPE, NAME, VALUE) TYPE NAME { VALUE }
#else
#   define KKM_CVARI(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }
#   define KKM_CVARE(TYPE, NAME, VALUE)
#   define KKM_MVARI(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#   define KKM_MVARE(TYPE, NAME, VALUE)
#endif

#ifdef EXTERNAL_KKM_STRINGS
#   define KKM_CSTRI(NAME, VALUE) extern const char * const NAME
#   define KKM_CSTRE(NAME, VALUE) const char * const NAME { VALUE }
#   define KKM_WCSTRI(NAME, VALUE) extern const wchar_t * const NAME
#   define KKM_WCSTRE(NAME, VALUE) const wchar_t * const NAME { VALUE }
#   define KKM_STRI(NAME, VALUE) extern const std::string_view NAME
#   define KKM_STRE(NAME, VALUE) const std::string_view NAME { VALUE }
#   define KKM_WSTRI(NAME, VALUE) extern const std::wstring_view NAME
#   define KKM_WSTRE(NAME, VALUE) const std::wstring_view NAME { VALUE }
#   define KKM_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
#   define KKM_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
#   define KKM_CSTRI(NAME, VALUE) constexpr const char * NAME { VALUE }
#   define KKM_CSTRE(NAME, VALUE)
#   define KKM_WCSTRI(NAME, VALUE) constexpr const wchar_t * NAME { VALUE }
#   define KKM_WCSTRE(NAME, VALUE)
#   define KKM_STRI(NAME, VALUE) constexpr std::string_view NAME { VALUE }
#   define KKM_STRE(NAME, VALUE)
#   define KKM_WSTRI(NAME, VALUE) constexpr std::wstring_view NAME { VALUE }
#   define KKM_WSTRE(NAME, VALUE)
#   define KKM_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#   define KKM_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#endif

#define VERSION_10108 10'010'008
#define VERSION_10107 10'010'007
#define VERSION_10106 10'010'006
#define FALLBACK_VERSION VERSION_10106

#if defined(WITH_10108) && WITH_10108
#   define VERSION_LIMIT VERSION_10108
#elif defined(WITH_10107) && WITH_10107
#   define VERSION_LIMIT VERSION_10107
#else
#   define VERSION_LIMIT VERSION_10106
#endif
