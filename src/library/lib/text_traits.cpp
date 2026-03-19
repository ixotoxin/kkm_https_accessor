// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "text_traits.h"
// #include <cctype> // CLEANUP
#include <cwctype>

[[nodiscard, maybe_unused]]
bool Meta::Wcs::trimPredicate(const wchar_t c) {
    // return std::iswspace(c);
    return c == L' ' || c == L'\n' || c == L'\r' || c == L'\t' || c == L'\v' || c == L'\f';
}

[[nodiscard, maybe_unused]]
bool Meta::Wcs::noTrimPredicate(const wchar_t c) {
    // return !std::iswspace(c);
    return c != L' ' && c != L'\n' && c != L'\r' && c != L'\t' && c != L'\v' && c != L'\f';
}

[[nodiscard, maybe_unused]]
wchar_t Meta::Wcs::toLower(const wchar_t c) {
    // ISSUE: Не совсем корректный способ, но для данного проекта достаточен.
    return static_cast<wchar_t>(std::towlower(c));
}

template<>
signed long Meta::Wcs::toNumeric<signed long>(const Char *str, Char **end) {
    return std::wcstol(str, end, 10);
}

template<>
unsigned long Meta::Wcs::toNumeric<unsigned long>(const Char *str, Char **end) {
    return std::wcstoul(str, end, 10);
}

template<>
[[nodiscard, maybe_unused]]
signed long long Meta::Wcs::toNumeric<signed long long>(const Char * str, Char ** end) {
    return std::wcstoll(str, end, 10);
}

template<>
[[nodiscard, maybe_unused]]
unsigned long long Meta::Wcs::toNumeric<unsigned long long>(const Char * str, Char ** end) {
    return std::wcstoull(str, end, 10);
}

template<>
[[nodiscard, maybe_unused]]
double Meta::Wcs::toNumeric<double>(const Char * str, Char ** end) {
    return std::wcstod(str, end);
}

template<>
[[nodiscard, maybe_unused]]
long double Meta::Wcs::toNumeric<long double>(const Char * str, Char ** end) {
    return std::wcstold(str, end);
}

[[nodiscard, maybe_unused]]
bool Meta::Mbs::trimPredicate(const unsigned char c) {
    // return std::isspace(c);
    // return c <= 0x20u || c == 0x7fu;
    return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f';
}

[[nodiscard, maybe_unused]]
bool Meta::Mbs::noTrimPredicate(const unsigned char c) {
    // return !std::isspace(c);
    // return c > 0x20u && c != 0x7fu;
    return c != ' ' && c != '\n' && c != '\r' && c != '\t' && c != '\v' && c != '\f';
}

[[nodiscard, maybe_unused]]
char Meta::Mbs::toLower(const unsigned char c) {
    // ISSUE: Не совсем корректный способ, но для данного проекта достаточен.
    return static_cast<Char>(std::tolower(c));
}

template<>
[[nodiscard, maybe_unused]]
signed long Meta::Mbs::toNumeric<signed long>(const Char * str, Char ** end) {
    return std::strtol(str, end, 10);
}

template<>
[[nodiscard, maybe_unused]]
unsigned long Meta::Mbs::toNumeric<unsigned long>(const Char * str, Char ** end) {
    return std::strtoul(str, end, 10);
}

template<>
[[nodiscard, maybe_unused]]
signed long long Meta::Mbs::toNumeric<signed long long>(const Char * str, Char ** end) {
    return std::strtoll(str, end, 10);
}

template<>
[[nodiscard, maybe_unused]]
unsigned long long Meta::Mbs::toNumeric<unsigned long long>(const Char * str, Char ** end) {
    return std::strtoull(str, end, 10);
}

template<>
[[nodiscard, maybe_unused]]
double Meta::Mbs::toNumeric<double>(const Char * str, Char ** end) {
    return std::strtod(str, end);
}

template<>
[[nodiscard, maybe_unused]]
long double Meta::Mbs::toNumeric<long double>(const Char * str, Char ** end) {
    return std::strtold(str, end);
}
