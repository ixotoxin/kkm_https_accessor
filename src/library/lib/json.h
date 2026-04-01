// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "strings.h"
#include "wconv.h"
#include "except.h"
#include "numeric.h"
#include "datetime.h"
#include "text.h"
#include "hexer.h"
#include <nlohmann/json.hpp>
#include <cassert>

namespace Nln {
    using Json = nlohmann::json;
    using Exception = Json::exception;
    constexpr auto EmptyJsonObject = Json::value_t::object;
}

inline std::wostream & operator<<(std::wostream & stream, const Nln::Json & json) {
    stream << Text::convert(json.dump(4));
    return stream;
}

namespace Json {
    using Basic::Failure;
    using Basic::DataError;
    using Handler = std::function<bool(const Nln::Json &, const std::wstring &)>;

    namespace Detail {
        void writeHex1(Meta::String auto & result, size_t & pos, const uint32_t code) {
            using Txt = Meta::TextTrait<decltype(result)>;
            Bin::Int2Hex hex { code };
            result[pos++] = Txt::c_letterU;
            result[pos++] = Txt::c_zero;
            result[pos++] = Txt::c_zero;
            hex.writeTo<1>(result, pos);
        }

        void writeHex2(Meta::String auto & result, size_t & pos, const uint32_t code) {
            using Txt = Meta::TextTrait<decltype(result)>;
            Bin::Int2Hex hex { code };
            result[pos++] = Txt::c_letterU;
            hex.writeTo<2>(result, pos);
        }

        void writeHex3(Meta::String auto & result, size_t & pos, const uint32_t code) {
            using Txt = Meta::TextTrait<decltype(result)>;
            Bin::Int2Hex hex { code };
            result[pos++] = Txt::c_letterU;
            result[pos++] = Txt::c_openingCurlyBrace;
            hex.writeTo<3>(result, pos);
            result[pos++] = Txt::c_closingCurlyBrace;
        }

        size_t basicExtraSpace(const Meta::View auto text) {
            using Txt = Meta::TextTrait<decltype(text)>;

            size_t result { 0 };
            auto it = text.begin();

            while (it != text.end()) {
                switch (const uint32_t high { static_cast<uint32_t>(*it) }) {
                    case Txt::c_quotationMark:
                    case Txt::c_solidus:
                    case Txt::c_reverseSolidus:
                    case Txt::c_backspace:
                    case Txt::c_formFeed:
                    case Txt::c_newLine:
                    case Txt::c_carriageReturn:
                    case Txt::c_horizontalTab:
                        ++result;
                        break;
                    default:
                        if (high <= 0x1f || high == 0x7f) {
                            result += 5;
                        }
                        break;
                }
                ++it;
            }

            return result;
        }

        inline size_t fullExtraSpace(const std::wstring_view text) {
            size_t result { 0 };
            auto it = text.begin();

            while (it != text.end()) {
                switch (const uint32_t high { *it /*static_cast<uint32_t>(*it)*/ }) {
                    case L'"':
                    case L'/':
                    case L'\\':
                    case L'\b':
                    case L'\f':
                    case L'\n':
                    case L'\r':
                    case L'\t':
                        ++result;
                        break;
                    default:
                        if (high <= 0x001f) { // NOLINT(*-branch-clone)
                            result += 5;
                        } else if (high <= 0x007e) {
                            // NOP
                        } else if (high <= 0x00ff) {
                            result += 5;
                        } else if (high >= 0xd800 && high <= 0xdbff) {
                            ++it;
                            if (it == text.end()) {
                                result += 5;
                                break;
                            }
                            const uint32_t low { *it /*static_cast<uint32_t>(*it)*/ };
                            if (low >= 0xdc00 && low <= 0xdfff) {
                                result += 8;
                                break;
                            }
                            result += 10;
                        } else {
                            result += 5;
                        }
                        break;
                }
                ++it;
            }

            return result;
        }
    }

    template<Meta::View T>
    auto escapeBasic(const T text) -> Meta::TextTrait<T>::String {
        using Txt = Meta::TextTrait<T>;

        const auto extraSpace = Detail::basicExtraSpace(text);
        if (extraSpace == 0) {
            return { text.begin(), text.end() };
        }

        size_t size { text.size() };
        typename Txt::String result(size + extraSpace, Txt::c_reverseSolidus);
        size_t pos { 0 };
        auto it = text.begin();

        while (it != text.end()) {
            switch (const uint32_t codePoint { static_cast<uint32_t>(*it) }) {
                case Txt::c_quotationMark: result[++pos] = Txt::c_quotationMark; ++pos; break;
                case Txt::c_solidus: result[++pos] = Txt::c_solidus; ++pos; break;
                case Txt::c_reverseSolidus: pos += 2; break;
                case Txt::c_backspace: result[++pos] = Txt::c_backspaceLiteral; ++pos; break;
                case Txt::c_formFeed: result[++pos] = Txt::c_formFeedLiteral; ++pos; break;
                case Txt::c_newLine: result[++pos] = Txt::c_newLineLiteral; ++pos; break;
                case Txt::c_carriageReturn: result[++pos] = Txt::c_carriageReturnLiteral; ++pos; break;
                case Txt::c_horizontalTab: result[++pos] = Txt::c_horizontalTabLiteral; ++pos; break;
                default:
                    if (codePoint <= 0x1f || codePoint == 0x7f) {
                        Detail::writeHex1(result, ++pos, codePoint);
                    } else {
                        result[pos++] = static_cast<wchar_t>(codePoint);
                    }
                    break;
            }
            ++it;
        }

        return result;
    }

    template<Meta::String T>
    auto escapeBasic(const T & text) -> Meta::TextTrait<T>::String {
        return escapeBasic<typename Meta::TextTrait<T>::View>(text);
    }

    template<Meta::Char T>
    auto escapeBasic(const T * text) -> Meta::TextTrait<T>::String {
        return escapeBasic<typename Meta::TextTrait<T>::View>(text);
    }

    inline std::wstring escapeFull(const std::wstring_view text) {
        const auto extraSpace = Detail::fullExtraSpace(text);
        if (extraSpace == 0) {
            return { text.begin(), text.end() };
        }

        const size_t size { text.size() };
        std::wstring result(size + extraSpace, Meta::Wcs::c_reverseSolidus);
        size_t pos { 0 };
        auto it = text.begin();

        while (it != text.end()) {
            switch (const uint32_t high { *it /*static_cast<uint32_t>(*it)*/ }) {
                case L'"': result[++pos] = '"'; ++pos; break;
                case L'/': result[++pos] = '/'; ++pos; break;
                case L'\\': pos += 2; break;
                case L'\b': result[++pos] = L'b'; ++pos; break;
                case L'\f': result[++pos] = L'f'; ++pos; break;
                case L'\n': result[++pos] = L'n'; ++pos; break;
                case L'\r': result[++pos] = L'r'; ++pos; break;
                case L'\t': result[++pos] = L't'; ++pos; break;
                default:
                    if (high <= 0x001f) { // NOLINT(*-branch-clone)
                        Detail::writeHex1(result, ++pos, high);
                    } else if (high <= 0x007e) {
                        result[pos++] = static_cast<wchar_t>(high);
                    } else if (high <= 0x00ff) {
                        Detail::writeHex1(result, ++pos, high);
                    } else if (high >= 0xd800 && high <= 0xdbff) {
                        ++it;
                        if (it == text.end()) {
                            Detail::writeHex2(result, ++pos, high);
                            break;
                        }
                        const uint32_t low { *it /*static_cast<uint32_t>(*it)*/ };
                        if (low >= 0xdc00 && low <= 0xdfff) {
                            const uint32_t codePoint = 0x01'0000 + ((high & 0x03ff) << 10) | (low & 0x03ff);
                            Detail::writeHex3(result, ++pos, codePoint);
                            break;
                        }
                        Detail::writeHex2(result, ++pos, high);
                        Detail::writeHex2(result, ++pos, low);
                    } else {
                        Detail::writeHex2(result, ++pos, high);
                    }
                    break;
            }
            ++it;
        }

        return result;
    }

    template<Meta::Bool T>
    [[nodiscard, maybe_unused]]
    T cast(const Nln::Json & json) try {
        if (json.is_boolean()) {
            return json.get<T>();
        }
        if (json.is_string()) {
            return Text::cast<T>(static_cast<std::string>(json));
        }
        if (json.is_number()) {
            return static_cast<T>(json.get<int64_t>());
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what())); // NOLINT(*-exception-baseclass)
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    T cast(const Nln::Json & json) try {
        if (json.is_number/*_integer*/()) {
            return json.get<T>();
        }
        if (json.is_string()) {
            return Text::cast<T>(static_cast<std::string>(json));
        }
        if (json.is_boolean()) {
            return static_cast<T>(json.get<bool>());
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what())); // NOLINT(*-exception-baseclass)
    }

    template<Meta::fromTemplate<std::chrono::duration> T>
    [[nodiscard, maybe_unused]]
    T cast(const Nln::Json & json) try {
        if (json.is_number/*_integer*/()) {
            return T(json.get<typename T::rep>());
        }
        if (json.is_string()) {
            return T(Text::cast<typename T::rep>(static_cast<std::string>(json)));
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what())); // NOLINT(*-exception-baseclass)
    }

    template<Meta::FloatingPoint T>
    [[nodiscard, maybe_unused]]
    T cast(const Nln::Json & json) try {
        if (json.is_number()) {
            return json.get<T>();
        }
        if (json.is_string()) {
            return Text::cast<T>(static_cast<std::string>(json));
        }
        if (json.is_boolean()) {
            return static_cast<T>(json.get<bool>());
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what())); // NOLINT(*-exception-baseclass)
    }

    template<Meta::String T>
    [[nodiscard, maybe_unused]]
    T cast(const Nln::Json & json) try {
        if (json.is_string()) {
            if constexpr (Meta::isWide<T>) {
                return Text::convert(json.get<std::string>());
            } else {
                return json.get<std::string>();
            }
        }
        if (json.is_number_integer()) {
            return Numeric::cast<T>(json.get<int64_t>());
        }
        if (json.is_number_float()) {
            return Numeric::cast<T>(json.get<long double>());
        }
        if (json.is_boolean()) {
            auto value = Text::yesNo<typename Meta::TextTrait<T>::Wideness>(json.get<bool>());
            // auto value = Numeric::boolCast<typename Meta::TextTrait<T>::View, Meta::YesNo>(json.get<bool>());
            return { value.begin(), value.end() };
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what())); // NOLINT(*-exception-baseclass)
    }

    template<std::same_as<std::filesystem::path> T>
    [[nodiscard, maybe_unused]]
    T cast(const Nln::Json & json) try {
        if (json.is_string()) {
            return T(Text::convert(static_cast<std::string>(json)));
        }
        throw DataError(Basic::Wcs::c_invalidValue); // NOLINT(*-exception-baseclass)
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what())); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    inline bool handle(
        const Nln::Json & json,
        const Handler & handler,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        return handler(json, jsonPath);
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    inline bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        const Handler & handler,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle(json[key], handler, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    requires (!Meta::BackSideGrowingRange<T>)
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        variable = cast<T>(json);
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    requires (!Meta::BackSideGrowingRange<T>)
    [[maybe_unused]]
    bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::Filter<T> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        variable = filter(cast<T>(json));
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::Filter<T> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, filter, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T, Meta::EnumCastMap<T> U>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const U & castMap,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        auto text = cast<typename U::key_type>(json);
        Text::trim(text);
        Text::lower(text);
        auto it = castMap.find(text);
        if (it == castMap.end()) {
            throw DataError(Basic::Wcs::c_rangeError, jsonPath); // NOLINT(*-exception-baseclass)
        }
        variable = it->second;
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, castMap, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const Meta::Filter<T> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        if (T value {}; handle(json, value, castMap, jsonPath)) {
            variable = filter(value);
            return true;
        }
        return false;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumCastMap<T> auto & castMap,
        const Meta::Filter<T> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, castMap, filter, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T, Meta::EnumDomain<T> U>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const U & domain,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        auto value = cast<T>(json);
        if constexpr (Meta::String<T>) {
            Text::trim(value);
            Text::lower(value);
        }
        if (std::ranges::find(domain, value) == domain.end()) {
            throw DataError(Basic::Wcs::c_rangeError, jsonPath); // NOLINT(*-exception-baseclass)
        }
        variable = value;
        return true;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, domain, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const Meta::Filter<T> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_null()) {
            return false;
        }
        if (T value {}; handle(json, value, domain, jsonPath)) {
            variable = filter(value);
            return true;
        }
        return false;
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<typename T>
    [[maybe_unused]]
    bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::EnumDomain<T> auto & domain,
        const Meta::Filter<T> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, domain, filter, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_array()) {
            for (const auto & j : json) {
                variable.push_back(cast<typename T::value_type>(j));
            }
            return true;
        }
        if (json.is_null()) {
            return false;
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    [[maybe_unused]]
    bool handleKey(
        const Nln::Json & json,
        const std::string_view key,
        Meta::BackSideGrowingRange auto & variable,
        const std::wstring & baseJsonPath = {}
    ) {
        assert(json.is_object());
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle(json[key], variable, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        T & variable,
        const Meta::Filter<typename T::value_type> auto & filter,
        const std::wstring & jsonPath = {}
    ) try {
        if (json.is_array()) {
            for (const auto & j : json) {
                variable.push_back(filter(cast<typename T::value_type>(j)));
            }
            return true;
        }
        if (json.is_null()) {
            return false;
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    } catch (DataError & e) {
        e.variable(jsonPath);
        throw;
    } catch (const Nln::Exception & e) {
        throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
    }

    template<Meta::BackSideGrowingRange T>
    [[maybe_unused]]
    bool handle(
        const Nln::Json & json,
        const std::string_view key,
        T & variable,
        const Meta::Filter<typename T::value_type> auto & filter,
        const std::wstring & baseJsonPath = {}
    ) {
        std::wstring jsonPath { baseJsonPath };
        Text::joinTo(jsonPath, Text::convert(key), L".");
        try {
            if (json.is_object()) {
                if (json.contains(key)) {
                    return handle<T>(json[key], variable, filter, jsonPath);
                }
                return false;
            }
            if (json.is_null()) {
                return false;
            }
        } catch (DataError & e) {
            e.variable(jsonPath);
            throw;
        } catch (const Nln::Exception & e) {
            throw DataError(Text::convert(e.what()), jsonPath); // NOLINT(*-exception-baseclass)
        }
        throw DataError(Basic::Wcs::c_invalidValue, jsonPath); // NOLINT(*-exception-baseclass)
    }
}
