// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include <cassert>
#include <cstdlib>
#include <cwchar>
#include <unordered_set>

namespace Meta {
    struct Mbs;

    struct Wcs {
        using Char = wchar_t;
        using String = std::wstring;
        using View = std::wstring_view;
        using Opposite = Mbs;

        static constexpr Char c_minus [[maybe_unused]] { L'-' };
        static constexpr Char c_terminator [[maybe_unused]] { L'\0' };
        static constexpr View c_assignmentSigns [[maybe_unused]] { L"=:" };
        static constexpr Char c_quotationMark [[maybe_unused]] { L'"' };
        static constexpr Char c_solidus [[maybe_unused]] { L'/' };
        static constexpr Char c_reverseSolidus [[maybe_unused]] { L'\\' };
        static constexpr Char c_backspace [[maybe_unused]] { L'\b' };
        static constexpr Char c_backspaceLiteral [[maybe_unused]] { L'b' };
        static constexpr Char c_formFeed [[maybe_unused]] { L'\f' };
        static constexpr Char c_formFeedLiteral [[maybe_unused]] { L'f' };
        static constexpr Char c_newLine [[maybe_unused]] { L'\n' };
        static constexpr Char c_newLineLiteral [[maybe_unused]] { L'n' };
        static constexpr Char c_carriageReturn [[maybe_unused]] { L'\r' };
        static constexpr Char c_carriageReturnLiteral [[maybe_unused]] { L'r' };
        static constexpr Char c_horizontalTab [[maybe_unused]] { L'\t' };
        static constexpr Char c_horizontalTabLiteral [[maybe_unused]] { L't' };
        static constexpr Char c_verticalTab [[maybe_unused]] { L'\v' };
        static constexpr Char c_verticalTabLiteral [[maybe_unused]] { L'v' };
        static constexpr Char c_openingCurlyBrace [[maybe_unused]] { L'{' };
        static constexpr Char c_closingCurlyBrace [[maybe_unused]] { L'}' };
        static constexpr Char c_letterU [[maybe_unused]] { L'u' };
        static constexpr Char c_zero [[maybe_unused]] { L'0' };
        static constexpr View c_hexDigits [[maybe_unused]] { L"0123456789ABCDEF" };
        static constexpr Char c_defaultPadding [[maybe_unused]] { L' ' };

        static inline const std::unordered_set<String> c_trueValueStrings [[maybe_unused]] {
            L"true", L"t", L"yes", L"y", L"on", L"enable", L"ena", L"en", L"e", L"allow", L"allowed", L"a", L"+"
        };

        static inline const std::unordered_set<String> c_falseValueStrings [[maybe_unused]] {
            L"false", L"f", L"no", L"n", L"off", L"disable", L"dis", L"d", L"deny", L"denied", L"-"
        };

        [[nodiscard, maybe_unused]]
        static bool trimPredicate(wchar_t);

        [[nodiscard, maybe_unused]]
        static bool noTrimPredicate(wchar_t);

        [[nodiscard, maybe_unused]]
        static wchar_t toLower(wchar_t);

        template<typename T>
        [[nodiscard, maybe_unused]]
        static T toNumeric(const Char * str, Char ** end);

        template<>
        [[nodiscard, maybe_unused]]
        signed long toNumeric<signed long>(const Char * str, Char ** end) {
            return std::wcstol(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        unsigned long toNumeric<unsigned long>(const Char * str, Char ** end) {
            return std::wcstoul(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        signed long long toNumeric<signed long long>(const Char * str, Char ** end) {
            return std::wcstoll(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        unsigned long long toNumeric<unsigned long long>(const Char * str, Char ** end) {
            return std::wcstoull(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        double toNumeric<double>(const Char * str, Char ** end) {
            return std::wcstod(str, end);
        }

        template<>
        [[nodiscard, maybe_unused]]
        long double toNumeric<long double>(const Char * str, Char ** end) {
            return std::wcstold(str, end);
        }

        [[nodiscard, maybe_unused]]
        static bool contains(const Char * str, const Char ch) {
            return std::wcschr(str, ch) != nullptr;
        }

        [[nodiscard, maybe_unused]]
        static auto length(const Char * str) {
            return std::wcslen(str);
        }

        [[nodiscard, maybe_unused]]
        static auto length(const String & str) {
            return str.length();
        }

        [[nodiscard, maybe_unused]]
        static auto length(const View str) {
            return str.length();
        }
    };

    struct Mbs {
        using Char = char;
        using String = std::string;
        using View = std::string_view;
        using Opposite = Wcs;

        static constexpr Char c_minus [[maybe_unused]] { '-' };
        static constexpr Char c_terminator [[maybe_unused]] { '\0' };
        static constexpr View c_assignmentSigns [[maybe_unused]] { "=:" };
        static constexpr Char c_quotationMark [[maybe_unused]] { '"' };
        static constexpr Char c_solidus [[maybe_unused]] { '/' };
        static constexpr Char c_reverseSolidus [[maybe_unused]] { '\\' };
        static constexpr Char c_backspace [[maybe_unused]] { '\b' };
        static constexpr Char c_backspaceLiteral [[maybe_unused]] { 'b' };
        static constexpr Char c_formFeed [[maybe_unused]] { '\f' };
        static constexpr Char c_formFeedLiteral [[maybe_unused]] { 'f' };
        static constexpr Char c_newLine [[maybe_unused]] { '\n' };
        static constexpr Char c_newLineLiteral [[maybe_unused]] { 'n' };
        static constexpr Char c_carriageReturn [[maybe_unused]] { '\r' };
        static constexpr Char c_carriageReturnLiteral [[maybe_unused]] { 'r' };
        static constexpr Char c_horizontalTab [[maybe_unused]] { '\t' };
        static constexpr Char c_horizontalTabLiteral [[maybe_unused]] { 't' };
        static constexpr Char c_verticalTab [[maybe_unused]] { '\v' };
        static constexpr Char c_verticalTabLiteral [[maybe_unused]] { 'v' };
        static constexpr Char c_openingCurlyBrace [[maybe_unused]] { '{' };
        static constexpr Char c_closingCurlyBrace [[maybe_unused]] { '}' };
        static constexpr Char c_letterU [[maybe_unused]] { 'u' };
        static constexpr Char c_zero [[maybe_unused]] { '0' };
        static constexpr View c_hexDigits [[maybe_unused]] { "0123456789ABCDEF" };
        static constexpr Char c_defaultPadding [[maybe_unused]] { ' ' };

        static inline const std::unordered_set<String> c_trueValueStrings {
            "true", "t", "yes", "y", "on", "enable", "ena", "en", "e", "allow", "allowed", "a", "+"
        };

        static inline const std::unordered_set<String> c_falseValueStrings {
            "false", "f", "no", "n", "off", "disable", "dis", "d", "deny", "denied", "-"
        };

        [[nodiscard, maybe_unused]]
        static bool trimPredicate(unsigned char);

        [[nodiscard, maybe_unused]]
        static bool noTrimPredicate(unsigned char);

        [[nodiscard, maybe_unused]]
        static char toLower(unsigned char);

        template<typename T>
        [[nodiscard, maybe_unused]]
        static T toNumeric(const Char * str, Char ** end);

        template<>
        [[nodiscard, maybe_unused]]
        signed long toNumeric<signed long>(const Char * str, Char ** end) {
            return std::strtol(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        unsigned long toNumeric<unsigned long>(const Char * str, Char ** end) {
            return std::strtoul(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        signed long long toNumeric<signed long long>(const Char * str, Char ** end) {
            return std::strtoll(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        unsigned long long toNumeric<unsigned long long>(const Char * str, Char ** end) {
            return std::strtoull(str, end, 10);
        }

        template<>
        [[nodiscard, maybe_unused]]
        double toNumeric<double>(const Char * str, Char ** end) {
            return std::strtod(str, end);
        }

        template<>
        [[nodiscard, maybe_unused]]
        long double toNumeric<long double>(const Char * str, Char ** end) {
            return std::strtold(str, end);
        }

        [[nodiscard, maybe_unused]]
        static bool contains(const Char * str, const Char ch) {
            return std::strchr(str, ch) != nullptr;
        }

        [[nodiscard, maybe_unused]]
        static auto length(const Char * str) {
            return std::strlen(str);
        }

        [[nodiscard, maybe_unused]]
        static auto length(const String & str) {
            return str.length();
        }

        [[nodiscard, maybe_unused]]
        static auto length(const View str) {
            return str.length();
        }
    };

    template<>
    struct WideTypes<Wcs> : std::true_type {};

    template<typename T>
    concept Wideness = std::is_same_v<T, Wcs> || std::is_same_v<T, Mbs>;

    template<typename>
    struct TextTrait {};

    template<>
    struct TextTrait<Wcs::Char> : Wcs { using Wideness = Wcs; };

    template<>
    struct TextTrait<Mbs::Char> : Mbs { using Wideness = Mbs; };

    template<typename T>
    requires std::is_same_v<ArrayElementType<T>, Wcs::Char> || std::is_same_v<ArrayElementType<T>, Mbs::Char>
    struct TextTrait<T> : TextTrait<ArrayElementType<T>> {};

    template<typename T>
    requires String<std::remove_cvref_t<T>> || View<std::remove_cvref_t<T>>
    struct TextTrait<T> : TextTrait<ContainerElementType<T>> {};

    template<typename T>
    concept TextualContainer
        = BackSideGrowingRange<T>
          && (String<typename T::value_type> || View<typename T::value_type>)
          && requires (
                T t,
                typename T::value_type u,
                typename TextTrait<typename T::value_type>::View v,
                typename TextTrait<typename T::value_type>::View::size_type z
             ) {
                t.emplace_back(u, z, z);
                t.emplace_back(v, z, z);
             };
}
