// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include "strings.h"
#include "wconv.h"
#include "except.h"
#include "text_traits.h"
#include "numeric.h"
#include <cassert>
#include <algorithm>

namespace Text {
    using Basic::DataError;

    constexpr size_t c_defaultStringSize { 112 };

    template<Meta::Char T>
    [[nodiscard, maybe_unused]]
    T * trimmedChars(T * text) {
        if (text && *text) {
            while (Trait<T>::trimPredicate(*text)) { ++text; }
            if (*text) {
                auto end = text + Trait<T>::length(text) - 1;
                while (Trait<T>::trimPredicate(*end)) { --end; }
                end[1] = Trait<T>::c_terminator;
            }
        }
        return text;
    }

    template<Meta::String T>
    [[maybe_unused]]
    void trim(T & text) {
        text.erase(text.begin(), std::find_if(text.begin(), text.end(), Trait<T>::noTrimPredicate));
        text.erase(std::find_if(text.rbegin(), text.rend(), Trait<T>::noTrimPredicate).base(), text.end());
    }

    template<Meta::View T>
    [[nodiscard, maybe_unused]]
    String<T> trimmed(const T text) {
        auto end = std::find_if(text.rbegin(), text.rend(), Trait<T>::noTrimPredicate).base();
        return { std::find_if(text.begin(), end, Trait<T>::noTrimPredicate), end };
    }

    template<Meta::Char T>
    [[nodiscard, maybe_unused]]
    auto trimmed(const T * text) {
        return trimmed<View<T>>(View<T> { text });
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto trimmed(const T & text) {
        return trimmed<View<T>>(View<T> { text });
    }

    template<Meta::Char T>
    [[nodiscard, maybe_unused]]
    T * loweredChars(T * text) {
        if (text && *text) {
            std::transform(text, text + Trait<T>::length(text), text, Trait<T>::toLower);
        }
        return text;
    }

    template<Meta::String T>
    [[maybe_unused]]
    void lower(T & text) {
        std::transform(text.begin(), text.end(), text.begin(), Trait<T>::toLower);
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    T lowered(T text) {
        std::transform(text.begin(), text.end(), text.begin(), Trait<T>::toLower);
        return text;
    }

    template<Meta::Char T>
    [[nodiscard, maybe_unused]]
    auto lowered(const T * text) {
        return lowered<String<T>>(String<T> { text });
    }

    template<Meta::View T>
    [[nodiscard, maybe_unused]]
    auto lowered(T text) {
        return lowered<String<T>>(String<T> { text.data(), text.length() });
    }

    template<Meta::Bool T, Meta::String U>
    [[nodiscard, maybe_unused]]
    T cast(U text) {
        if (text.empty()) {
            throw DataError(Basic::Wcs::c_invalidValue);
        }
        std::transform(text.begin(), text.end(), text.begin(), Trait<U>::toLower);
        if (Trait<U>::c_trueValueStrings.contains(text)) {
            return true;
        }
        if (Trait<U>::c_falseValueStrings.contains(text)) {
            return false;
        }
        throw DataError(Basic::Wcs::c_invalidValue);
    }

    template<Meta::Bool T, Meta::Char U>
    [[nodiscard, maybe_unused]]
    T cast(const U * text) {
        return cast<T, String<U>>(String<U> { text });
    }

    template<Meta::Bool T, Meta::View U>
    [[nodiscard, maybe_unused]]
    T cast(const U text) {
        return cast<T, String<U>>(String<U> { text });
    }

    template<Meta::Integral T, Meta::Char U>
    [[nodiscard, maybe_unused]]
    T cast(const U * text) {
        using Num = Meta::CastTrait<T>::Type;
        if constexpr (std::is_signed_v<T>) {
            if (Trait<U>::length(text) == 0) {
                throw DataError(Basic::Wcs::c_invalidValue);
            }
        } else {
            if (Trait<U>::length(text) == 0 || text[0] == Trait<U>::c_minus) {
                throw DataError(Basic::Wcs::c_invalidValue);
            }
        }
        Char<U> * end {};
        Num value = Trait<U>::template toNumeric<Num>(text, &end);
        if (end == text || *end) {
            throw DataError(Basic::Wcs::c_invalidValue);
        }
        return Numeric::safeCast<T>(value);
    }

    template<Meta::FloatingPoint T, Meta::Char U>
    [[nodiscard, maybe_unused]]
    T cast(const U * text) {
        if (Trait<U>::length(text) == 0) {
            throw DataError(Basic::Wcs::c_invalidValue);
        }
        Char<U> * end {};
        T value = Trait<U>::template toNumeric<T>(text, &end);
        if (end == text || *end) {
            throw DataError(Basic::Wcs::c_invalidValue);
        }
        return value;
    }

    template<Meta::Numeric T, Meta::String U>
    [[nodiscard, maybe_unused]]
    T cast(const U & text) {
        return cast<T, Char<U>>(text.c_str());
    }

    template<Meta::Numeric T, Meta::View U>
    [[nodiscard, maybe_unused]]
    T cast(const U text) {
        return cast<T, String<U>>(String<U> { text });
    }

    template<class T>
    requires (Meta::BackSideGrowingRange<T> && Meta::String<Meta::ValueType<T>>)
    [[maybe_unused]]
    Meta::Rebind<T, OppositeString<Meta::ValueType<T>>>
    convert(const T & container) {
        Meta::Rebind<T, OppositeString<Meta::ValueType<T>>> result {};
        if constexpr (Meta::isReservingRange<T>) {
            result.reserve(container.size());
        }
        for (const auto & item : container) {
            result.emplace_back(convert(item));
        }
        return result;
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto trim() {
        return [] (const T & value) -> T { return trimmed(value); };
    }

    template<Meta::String T, Meta::Filter<T> F>
    [[nodiscard, maybe_unused]]
    auto trim(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const T & value) -> T {
                return trimmed(subFilter(value));
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto lower() {
        return [] (const T & value) -> T { return lowered(value); };
    }

    template<Meta::String T, Meta::Filter<T> F>
    [[nodiscard, maybe_unused]]
    auto lower(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const T & value) -> T {
                return lowered(subFilter(value));
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto noEmpty() {
        return
            [] (const T & value) -> T {
                if (value.empty()) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return value;
            };
    }

    template<Meta::String T, Meta::Filter<T> F>
    [[nodiscard, maybe_unused]]
    auto noEmpty(F && subFilter0) {
        return
            [subFilter = std::forward<F>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (filtered.empty()) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return filtered;
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto length(const size_t min, const size_t max) {
        assert(max > min);
        return
            [min, max] (const T & value) -> T {
                if (value.length() < min || value.length() > max) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return value;
            };
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    auto maxLength(const size_t max) {
        assert(max > 0);
        return
            [max] (const T & value) -> T {
                if (value.length() > max) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return value;
            };
    }

    template<Meta::String T, Meta::Filter<T> F>
    [[nodiscard, maybe_unused]]
    auto length(const size_t min, const size_t max, F && subFilter0) {
        assert(max > min);
        return
            [min, max, subFilter = std::forward<F>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (filtered.length() < min || filtered.length() > max) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return filtered;
            };
    }

    template<Meta::String T, Meta::Filter<T> F>
    [[nodiscard, maybe_unused]]
    auto maxLength(const size_t max, F && subFilter0) {
        assert(max > 0);
        return
            [max, subFilter = std::forward<F>(subFilter0)]
            (const T & value) -> T {
                T filtered { subFilter(value) };
                if (filtered.length() > max) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return filtered;
            };
    }

    template<Meta::TextualContainer T>
    [[maybe_unused]]
    Meta::SizeType<T> splitTo(
        T & output,
        const View<Meta::ValueType<T>> text,
        const View<Meta::ValueType<T>> delims,
        const bool clear = false
    ) {
        if (clear && !output.empty()) {
            output.clear();
        }
        if (text.empty() || delims.empty()) {
            return 0;
        }
        auto first = text.find_first_not_of(delims);
        while (first != Meta::ValueType<T>::npos) {
            auto last = text.find_first_of(delims, first + 1);
            if (last == Meta::ValueType<T>::npos) {
                output.emplace_back(text, first, text.length() - first);
                break;
            }
            output.emplace_back(text, first, last - first);
            first = text.find_first_not_of(delims, last + 1);
        }
        return output.size();
    }

    template<Meta::String T>
    [[maybe_unused]]
    void splitVariable(
        const View<T> text,
        T & name,
        T & value,
        const bool lowerName = true,
        const bool lowerValue = false,
        const View<T> separator = Trait<T>::c_assignmentSigns
    ) {
        name.clear();
        value.clear();
        if (text.empty() || separator.empty()) {
            return;
        }
        auto position = text.find_first_of(separator);
        if (position == T::npos) {
            name.assign(trimmed(text));
            if (lowerName) {
                lower(name);
            }
            return;
        }
        name.assign(text, 0, position);
        if (position <= text.length()) {
            value.assign(text, position + 1, text.length());
        }
        trim(name);
        if (lowerName) {
            lower(name);
        }
        trim(value);
        if (lowerValue) {
            lower(value);
        }
    }

    template<Meta::TextualContainer T>
    [[maybe_unused]]
    Meta::ValueType<T>::size_type joinTo(
        String<Meta::ValueType<T>> & output,
        const T & container,
        const View<Meta::ValueType<T>> glue,
        const bool clear = false
    ) {
        if (clear && !output.empty()) {
            output.clear();
        }
        for (const auto & str : container) {
            if (!str.empty()) {
                if (!output.empty()) {
                    output.append(glue);
                }
                output.append(str);
            }
        }
        return output.length();
    }

    template<Meta::String T>
    [[maybe_unused]]
    Meta::SizeType<T> joinTo(
        T & output,
        T && text,
        const View<T> glue
    ) {
        if (output.empty()) {
            output.assign(std::forward<T>(text));
        } else {
            output.append(glue);
            output.append(text);
        }
        return output.length();
    }

    template<Meta::String T>
    [[maybe_unused]]
    Meta::SizeType<T> joinTo(
        T & output,
        const View<T> text,
        const View<T> glue
    ) {
        if (output.empty()) {
            output.assign(text);
        } else {
            output.append(glue);
            output.append(text);
        }
        return output.length();
    }

    /*template<typename ... T>
    requires (std::is_convertible_v<T, Meta::Wcs::View> && ...)
    [[nodiscard, maybe_unused]]
    Meta::Wcs::String concat(T && ... text) {
        Meta::Wcs::String result;
        (result.append(text), ...);
        return result;
    }*/

    /*template<typename ... T>
    requires (std::is_convertible_v<T, Meta::Mbs::View> && ...)
    [[nodiscard, maybe_unused]]
    Meta::Mbs::String concat(T && ... text) {
        Meta::Mbs::String result;
        (result.append(text), ...);
        return result;
    }*/

    template<size_t S = 0, typename ... T>
    requires (std::is_convertible_v<T, Meta::WcsText::View> && ...)
    [[nodiscard, maybe_unused]]
    Meta::WcsText::String concat(T && ... text) {
        Meta::WcsText::String result;
        if constexpr (S) {
            result.reserve(S);
        }
        (result.append(text), ...);
        return result;
    }

    template<size_t S = 0, typename ... T>
    requires (std::is_convertible_v<T, Meta::MbsText::View> && ...)
    [[nodiscard, maybe_unused]]
    Meta::MbsText::String concat(T && ... text) {
        Meta::MbsText::String result;
        if constexpr (S) {
            result.reserve(S);
        }
        (result.append(text), ...);
        return result;
    }

    template<Meta::String T, typename ... U>
    requires (std::is_convertible_v<U, View<T>> && ...)
    [[maybe_unused]]
    Meta::SizeType<T> concatTo(T & output, U && ... text) {
        (output.append(text), ...);
        return output.length();
    }

    template<Meta::Wideness T>
    [[nodiscard, maybe_unused]]
    View<T> daNet(const bool value) {
        return static_cast<bool>(value)
           ? Meta::BoolLabels<T, Meta::DaNet>::c_true
           : Meta::BoolLabels<T, Meta::DaNet>::c_false;
    }

    template<Meta::Wideness T>
    [[nodiscard, maybe_unused]]
    View<T> yesNo(const bool value) {
        return static_cast<bool>(value)
           ? Meta::BoolLabels<T, Meta::YesNo>::c_true
           : Meta::BoolLabels<T, Meta::YesNo>::c_false;
    }

    template<Meta::Wideness T>
    [[nodiscard, maybe_unused]]
    View<T> enaDis(const bool value) {
        return static_cast<bool>(value)
            ? Meta::BoolLabels<T, Meta::EnaDis>::c_true
            : Meta::BoolLabels<T, Meta::EnaDis>::c_false;
    }

    template<Meta::Wideness T>
    [[nodiscard, maybe_unused]]
    View<T> trueFalse(const bool value) {
        return static_cast<bool>(value)
           ? Meta::BoolLabels<T, Meta::TrueFalse>::c_true
           : Meta::BoolLabels<T, Meta::TrueFalse>::c_false;
    }

    namespace Wcs {
        [[nodiscard, maybe_unused]]
        inline auto trim() {
            return [] (const std::wstring & value) -> std::wstring { return trimmed(value); };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto trim(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    return trimmed(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto lower() {
            return [] (const std::wstring & value) -> std::wstring { return lowered(value); };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto lower(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    return lowered(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto noEmpty() {
            return
                [] (const std::wstring & value) -> std::wstring {
                    if (value.empty()) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto noEmpty(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (filtered.empty()) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto length(const size_t min, const size_t max) {
            assert(max > min);
            return
                [min, max] (const std::wstring & value) -> std::wstring {
                    if (value.length() < min || value.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto length(const size_t min, const size_t max, F && subFilter0) {
            assert(max > min);
            return
                [min, max, subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (filtered.length() < min || filtered.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
            };
        }

        [[nodiscard, maybe_unused]]
        inline auto maxLength(const size_t max) {
            assert(max > 0);
            return
                [max] (const std::wstring & value) -> std::wstring {
                    if (value.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto maxLength(const size_t max, F && subFilter0) {
            assert(max > 0);
            return
                [max, subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (filtered.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isInteger() {
            return
                [] (const std::wstring & value) -> std::wstring {
                    std::ignore = cast<int64_t>(value);
                    return value;
                };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto isInteger(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    std::ignore = cast<int64_t>(filtered);
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isFloatingPoint() {
            return
                [] (const std::wstring & value) -> std::wstring {
                    std::ignore = cast<long double>(value);
                    return value;
                };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto isFloatingPoint(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    std::ignore = cast<long double>(filtered);
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isIntBetween(const int64_t min, const int64_t max) {
            assert(min < max);
            return
                [min, max] (const std::wstring & value) -> std::wstring {
                    if (const int64_t numValue { cast<int64_t>(value) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto isIntBetween(const int64_t min, const int64_t max, F && subFilter0) {
            assert(min < max);
            return
                [min, max, subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (const int64_t numValue { cast<int64_t>(filtered) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isFpBetween(const long double min, const long double max) {
            assert(min < max);
            return
                [min, max] (const std::wstring & value) -> std::wstring {
                    if (const long double numValue { cast<long double>(value) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::wstring> F>
        [[nodiscard, maybe_unused]]
        auto isFpBetween(const long double min, const long double max, F && subFilter0) {
            assert(min < max);
            return
                [min, max, subFilter = std::forward<F>(subFilter0)]
                (const std::wstring & value) -> std::wstring {
                    std::wstring filtered { subFilter(value) };
                    if (const long double numValue { cast<long double>(filtered) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline std::wstring_view daNet(const bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<Meta::Wcs, Meta::DaNet>::c_true
                : Meta::BoolLabels<Meta::Wcs, Meta::DaNet>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::wstring_view yesNo(const bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<Meta::Wcs, Meta::YesNo>::c_true
                : Meta::BoolLabels<Meta::Wcs, Meta::YesNo>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::wstring_view enaDis(const bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<Meta::Wcs, Meta::EnaDis>::c_true
                : Meta::BoolLabels<Meta::Wcs, Meta::EnaDis>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::wstring_view trueFalse(const bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<Meta::Wcs, Meta::TrueFalse>::c_true
                : Meta::BoolLabels<Meta::Wcs, Meta::TrueFalse>::c_false;
        }
    }

    namespace Mbs {
        [[nodiscard, maybe_unused]]
        inline auto trim() {
            return [] (const std::string & value) -> std::string { return trimmed(value); };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto trim(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    return trimmed(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto lower() {
            return [] (const std::string & value) -> std::string { return lowered(value); };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto lower(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    return lowered(subFilter(value));
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto noEmpty() {
            return
                [] (const std::string & value) -> std::string {
                    if (value.empty()) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto noEmpty(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (filtered.empty()) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto length(const size_t min, const size_t max) {
            assert(max > min);
            return
                [min, max] (const std::string & value) -> std::string {
                    if (value.length() < min || value.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto length(const size_t min, const size_t max, F && subFilter0) {
            assert(max > min);
            return
                [min, max, subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (filtered.length() < min || filtered.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto maxLength(const size_t max) {
            assert(max > 0);
            return
                [max] (const std::string & value) -> std::string {
                    if (value.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto maxLength(const size_t max, F && subFilter0) {
            assert(max > 0);
            return
                [max, subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (filtered.length() > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isInteger() {
            return
                [] (const std::string & value) -> std::string {
                    std::ignore = cast<int64_t>(value);
                    return value;
                };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto isInteger(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    std::ignore = cast<int64_t>(filtered);
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isFloatingPoint() {
            return
                [] (const std::string & value) -> std::string {
                    std::ignore = cast<long double>(value);
                    return value;
                };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto isFloatingPoint(F && subFilter0) {
            return
                [subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    std::ignore = cast<long double>(filtered);
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isIntBetween(const int64_t min, const int64_t max) {
            assert(min < max);
            return
                [min, max] (const std::string & value) -> std::string {
                    if (const int64_t numValue { cast<int64_t>(value) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto isIntBetween(const int64_t min, const int64_t max, F && subFilter0) {
            assert(min < max);
            return
                [min, max, subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (const int64_t numValue { cast<int64_t>(filtered) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline auto isFpBetween(const long double min, const long double max) {
            assert(min < max);
            return
                [min, max] (const std::string & value) -> std::string {
                    if (const long double numValue { cast<long double>(value) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return value;
                };
        }

        template<Meta::Filter<std::string> F>
        [[nodiscard, maybe_unused]]
        auto isFpBetween(const long double min, const long double max, F && subFilter0) {
            assert(min < max);
            return
                [min, max, subFilter = std::forward<F>(subFilter0)]
                (const std::string & value) -> std::string {
                    std::string filtered { subFilter(value) };
                    if (const long double numValue { cast<long double>(filtered) }; numValue < min || numValue > max) {
                        throw DataError(Basic::Wcs::c_rangeError);
                    }
                    return filtered;
                };
        }

        [[nodiscard, maybe_unused]]
        inline std::string_view daNet(const bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<Meta::Mbs, Meta::DaNet>::c_true
                : Meta::BoolLabels<Meta::Mbs, Meta::DaNet>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::string_view yesNo(const bool value) {
            return static_cast<bool>(value)
               ? Meta::BoolLabels<Meta::Mbs, Meta::YesNo>::c_true
               : Meta::BoolLabels<Meta::Mbs, Meta::YesNo>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::string_view enaDis(const bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<Meta::Mbs, Meta::EnaDis>::c_true
                : Meta::BoolLabels<Meta::Mbs, Meta::EnaDis>::c_false;
        }

        [[nodiscard, maybe_unused]]
        inline std::string_view trueFalse(const bool value) {
            return static_cast<bool>(value)
                ? Meta::BoolLabels<Meta::Mbs, Meta::TrueFalse>::c_true
                : Meta::BoolLabels<Meta::Mbs, Meta::TrueFalse>::c_false;
        }
    }
}
