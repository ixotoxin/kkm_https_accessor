// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#define LOG_CONST(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }

#ifdef EXTERNAL_LOG_VARIABLES
#   define LOG_CVARI(TYPE, NAME, VALUE) extern const TYPE NAME
#   define LOG_CVARE(TYPE, NAME, VALUE) const TYPE NAME { VALUE }
#   define LOG_MVARI(TYPE, NAME, VALUE) extern TYPE NAME
#   define LOG_MVARE(TYPE, NAME, VALUE) TYPE NAME { VALUE }
#else
#   define LOG_CVARI(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define LOG_CVARE(TYPE, NAME, VALUE)
#   define LOG_MVARI(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#   define LOG_MVARE(TYPE, NAME, VALUE)
#endif

#ifdef EXTERNAL_LOG_STRINGS
#   define LOG_CSTRI(NAME, VALUE) extern const char * const NAME
#   define LOG_CSTRE(NAME, VALUE) const char * const NAME { VALUE }
#   define LOG_WCSTRI(NAME, VALUE) extern const wchar_t * const NAME
#   define LOG_WCSTRE(NAME, VALUE) const wchar_t * const NAME { VALUE }
#   define LOG_STRI(NAME, VALUE) extern const std::string_view NAME
#   define LOG_STRE(NAME, VALUE) const std::string_view NAME { VALUE }
#   define LOG_WSTRI(NAME, VALUE) extern const std::wstring_view NAME
#   define LOG_WSTRE(NAME, VALUE) const std::wstring_view NAME { VALUE }
#   define LOG_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
#   define LOG_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
#   define LOG_CSTRI(NAME, VALUE) constexpr const char * NAME { VALUE }
#   define LOG_CSTRE(NAME, VALUE)
#   define LOG_WCSTRI(NAME, VALUE) constexpr const wchar_t * NAME { VALUE }
#   define LOG_WCSTRE(NAME, VALUE)
#   define LOG_STRI(NAME, VALUE) constexpr std::string_view NAME { VALUE }
#   define LOG_STRE(NAME, VALUE)
#   define LOG_WSTRI(NAME, VALUE) constexpr std::wstring_view NAME { VALUE }
#   define LOG_WSTRE(NAME, VALUE)
#   define LOG_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#   define LOG_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#endif
