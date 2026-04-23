// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifndef EXECUTE_BEFORE_MAIN
#   ifndef _MSC_VER
#       define EXECUTE_BEFORE_MAIN(FUNC) static void __attribute__((constructor)) FUNC(void)
#   else
#       define EXECUTE_BEFORE_MAIN(FUNC) \
            inline void FUNC##___f(void); \
            struct FUNC##___t { FUNC##___t(void) { FUNC##___f(); } }; \
            inline FUNC##___t FUNC##___s; \
            inline void FUNC##___f(void)
#   endif
#endif

#define LIB_CONST(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }

#ifdef EXTERNAL_LIB_VARIABLES
#   define LIB_CVARI(TYPE, NAME, VALUE) extern const TYPE NAME
#   define LIB_CVARE(TYPE, NAME, VALUE) const TYPE NAME { VALUE }
#   define LIB_MVARI(TYPE, NAME, VALUE) extern TYPE NAME
#   define LIB_MVARE(TYPE, NAME, VALUE) TYPE NAME { VALUE }
#else
#   define LIB_CVARI(TYPE, NAME, VALUE) constexpr TYPE NAME { VALUE }
#   define LIB_CVARE(TYPE, NAME, VALUE)
#   define LIB_MVARI(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#   define LIB_MVARE(TYPE, NAME, VALUE)
#endif

// CLEANUP

#ifdef EXTERNAL_LIB_STRINGS
// #   define LIB_CSTRI(NAME, VALUE) extern const char * const NAME
// #   define LIB_CSTRE(NAME, VALUE) const char * const NAME { VALUE }
// #   define LIB_WCSTRI(NAME, VALUE) extern const wchar_t * const NAME
// #   define LIB_WCSTRE(NAME, VALUE) const wchar_t * const NAME { VALUE }
#   define LIB_STRI(NAME, VALUE) extern const std::string_view NAME
#   define LIB_STRE(NAME, VALUE) const std::string_view NAME { VALUE }
#   define LIB_WSTRI(NAME, VALUE) extern const std::wstring_view NAME
#   define LIB_WSTRE(NAME, VALUE) const std::wstring_view NAME { VALUE }
// #   define LIB_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
// #   define LIB_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
// #   define LIB_FMT2(R, F, A, ...) std::vformat_to(std::back_inserter(R),F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
// #   define LIB_WFMT2(R, F, A, ...) std::vformat_to(std::back_inserter(R),F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
// #   define LIB_CSTRI(NAME, VALUE) constexpr const char * NAME { VALUE }
// #   define LIB_CSTRE(NAME, VALUE)
// #   define LIB_WCSTRI(NAME, VALUE) constexpr const wchar_t * NAME { VALUE }
// #   define LIB_WCSTRE(NAME, VALUE)
#   define LIB_STRI(NAME, VALUE) constexpr std::string_view NAME { VALUE }
#   define LIB_STRE(NAME, VALUE)
#   define LIB_WSTRI(NAME, VALUE) constexpr std::wstring_view NAME { VALUE }
#   define LIB_WSTRE(NAME, VALUE)
// #   define LIB_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
// #   define LIB_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
// #   define LIB_FMT2(R, F, A, ...) std::format_to(std::back_inserter(R),F, A __VA_OPT__(,) __VA_ARGS__)
// #   define LIB_WFMT2(R, F, A, ...) std::format_to(std::back_inserter(R),F, A __VA_OPT__(,) __VA_ARGS__)
#endif
