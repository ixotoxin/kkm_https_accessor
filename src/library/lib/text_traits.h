// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include <cassert>
#include <cwchar>
#include <unordered_set>

namespace Meta {
    struct MbsText;

    struct WcsText {
        using Wideness = Wcs;
        using Char = wchar_t;
        using String = std::wstring;
        using View = std::wstring_view;
        using Opposite = MbsText;

        static constexpr View c_emptyStr [[maybe_unused]] { L"." };
        static constexpr Char c_dot [[maybe_unused]] { L'.' };
        static constexpr View c_dotStr [[maybe_unused]] { L"." };
        static constexpr Char c_minus [[maybe_unused]] { L'-' };
        static constexpr Char c_terminator [[maybe_unused]] { L'\0' };
        static constexpr View c_assignmentSigns [[maybe_unused]] { L"=:" };
        static constexpr Char c_quotationMark [[maybe_unused]] { L'"' };
        static constexpr Char c_solidus [[maybe_unused]] { L'/' };
        static constexpr View c_solidusStr [[maybe_unused]] { L"/" };
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

    template<>
    [[nodiscard, maybe_unused]]
    signed long WcsText::toNumeric<signed long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    unsigned long WcsText::toNumeric<unsigned long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    signed long long WcsText::toNumeric<signed long long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    unsigned long long WcsText::toNumeric<unsigned long long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    double WcsText::toNumeric<double>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    long double WcsText::toNumeric<long double>(const Char * str, Char ** end);

    struct MbsText {
        using Wideness = Mbs;
        using Char = char;
        using String = std::string;
        using View = std::string_view;
        using Opposite = WcsText;

        static constexpr View c_emptyStr [[maybe_unused]] { "." };
        static constexpr Char c_dot [[maybe_unused]] { '.' };
        static constexpr View c_dotStr [[maybe_unused]] { "." };
        static constexpr Char c_minus [[maybe_unused]] { '-' };
        static constexpr Char c_terminator [[maybe_unused]] { '\0' };
        static constexpr View c_assignmentSigns [[maybe_unused]] { "=:" };
        static constexpr Char c_quotationMark [[maybe_unused]] { '"' };
        static constexpr Char c_solidus [[maybe_unused]] { '/' };
        static constexpr View c_solidusStr [[maybe_unused]] { "/" };
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
    [[nodiscard, maybe_unused]]
    signed long MbsText::toNumeric<signed long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    unsigned long MbsText::toNumeric<unsigned long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    signed long long MbsText::toNumeric<signed long long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    unsigned long long MbsText::toNumeric<unsigned long long>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    double MbsText::toNumeric<double>(const Char * str, Char ** end);

    template<>
    [[nodiscard, maybe_unused]]
    long double MbsText::toNumeric<long double>(const Char * str, Char ** end);

    template<typename> struct TextTrait {};
    template<> struct TextTrait<Wcs> : WcsText {};
    template<> struct TextTrait<Mbs> : MbsText {};
    template<> struct TextTrait<WcsText::Char> : WcsText {};
    template<> struct TextTrait<MbsText::Char> : MbsText {};

    template<typename T>
    requires isChar<ValueType<T>>
    struct TextTrait<T> : TextTrait<ValueType<T>> {};

    template<typename T>
    concept AnyText = std::convertible_to<T, WcsText::View> || std::convertible_to<T, MbsText::View>;

    template<typename T>
    concept AnyTextClass = AnyText<T> && !std::is_pointer_v<T> && !std::is_array_v<T>;

    template<typename T, typename W>
    concept Text = std::convertible_to<T, typename TextTrait<W>::View>;

    template<typename T, typename W>
    concept TextClass = Text<T, W> && !std::is_pointer_v<T> && !std::is_array_v<T>;

    template<typename T>
    concept TextualContainer = BackSideGrowingRange<T> && AnyText<ValueType<T>>;

    template<typename T, typename W>
    concept TextDomain = isSetOf<T, const typename TextTrait<W>::Char *> && TextClass<ValueType<T>, W>; // NOLINT

    template<typename T, typename W, typename V>
    concept FromTextCastMap = isMapOf<T, const typename TextTrait<W>::Char *, V> && TextClass<KeyType<T>, W>; // NOLINT

    template<typename T, typename W>
    concept CastFromText = FromTextCastMap<T, W, ValueType<T>>;

    template<typename T, typename K, typename W>
    concept ToTextCastMap = isMapOf<T, K, typename TextTrait<W>::View>;

    template<typename T, typename W>
    concept CastToText = ToTextCastMap<T, KeyType<T>, W>;
}

namespace Text {
    template<typename T>
    using Trait = Meta::TextTrait<Meta::UnderlyingType<T>>;

    namespace Wcs {
        using Char = Meta::WcsText::Char;
        using String = Meta::WcsText::String;
        using View = Meta::WcsText::View;
    }

    namespace Mbs {
        using Char = Meta::MbsText::Char;
        using String = Meta::MbsText::String;
        using View = Meta::MbsText::View;
    }

    template<typename T>
    using Wideness = Trait<T>::Wideness;

    template<typename T>
    using Char = Trait<T>::Char;

    template<typename T>
    using String = Trait<T>::String;

    template<typename T>
    using View = Trait<T>::View;

    template<typename T>
    using OppositeChar = Trait<T>::Opposite::Char;

    template<typename T>
    using OppositeString = Trait<T>::Opposite::String;

    template<typename T>
    using OppositeView = Trait<T>::Opposite::View;
}
