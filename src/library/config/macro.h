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

#ifdef EXTERNAL_CONF_STRINGS
#   define CONF_STRI(NAME, VALUE) extern const std::string_view NAME
#   define CONF_STRE(NAME, VALUE) const std::string_view NAME { VALUE }
#   define CONF_WSTRI(NAME, VALUE) extern const std::wstring_view NAME
#   define CONF_WSTRE(NAME, VALUE) const std::wstring_view NAME { VALUE }
#else
#   define CONF_STRI(NAME, VALUE) constexpr std::string_view NAME { VALUE }
#   define CONF_STRE(NAME, VALUE)
#   define CONF_WSTRI(NAME, VALUE) constexpr std::wstring_view NAME { VALUE }
#   define CONF_WSTRE(NAME, VALUE)
#endif
