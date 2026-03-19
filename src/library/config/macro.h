// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#ifdef EXTERNAL_CONF_VARIABLES
#   define CONF_DEF(TYPE, NAME, VALUE)
#   define CONF_CONST(TYPE, NAME, VALUE) extern const TYPE NAME
#   define CONF_VAR(TYPE, NAME, VALUE) extern TYPE NAME
#else
#   define CONF_DEF(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define CONF_CONST(TYPE, NAME, VALUE) constexpr const TYPE NAME { VALUE }
#   define CONF_VAR(TYPE, NAME, VALUE) inline TYPE NAME { VALUE }
#endif

#ifdef EXTERNAL_CONF_STRINGS
#   define CONF_STR(NAME, VALUE) extern const std::string_view NAME
#   define CONF_FMT(F, A, ...) std::vformat(F, std::make_format_args(A __VA_OPT__(,) __VA_ARGS__))
#   define CONF_WSTR(NAME, VALUE) extern const std::wstring_view NAME
#   define CONF_WFMT(F, A, ...) std::vformat(F, std::make_wformat_args(A __VA_OPT__(,) __VA_ARGS__))
#else
#   define CONF_STR(NAME, VALUE) constexpr const std::string_view NAME { VALUE }
#   define CONF_FMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#   define CONF_WSTR(NAME, VALUE) constexpr const std::wstring_view NAME { VALUE }
#   define CONF_WFMT(F, A, ...) std::format(F, A __VA_OPT__(,) __VA_ARGS__)
#endif
