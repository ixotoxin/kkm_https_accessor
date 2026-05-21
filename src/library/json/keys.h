// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "escape.h"

namespace Json {
    constexpr size_t c_initialKeySize { 112 };

    namespace Literals {
        inline Meta::WcsJson::StringRef operator""_key(const wchar_t * str, const std::size_t len) noexcept {
            return { str, static_cast<SizeType>(len) };
        }
        inline Meta::MbsJson::StringRef operator""_key(const char * str, const std::size_t len) noexcept {
            return { str, static_cast<SizeType>(len) };
        }
    }

    namespace Wcs {
        using namespace Literals;
        inline const auto c_successKey = L"!success"_key;
        inline const auto c_successKeyEsc = escapeBasic(c_successKey);
        inline const auto c_messageKey = L"!message"_key;
        inline const auto c_messageKeyEsc = escapeBasic(c_messageKey);
    }

    namespace Mbs {
        using namespace Literals;
        inline const auto c_successKey = "!success"_key;
        inline const auto c_successKeyEsc = escapeBasic(c_successKey);
        inline const auto c_messageKey = "!message"_key;
        inline const auto c_messageKeyEsc = escapeBasic(c_messageKey);
    }

    template<Meta::JsonKey T>
    Value<T> kv(const T key) { // NOLINT
        return Value<T> { key };
    }

    template<Meta::JsonKey T>
    Ptr makePointer(const PtrView base, const T key) {
        assert(key.length > 0);
        Ptr result {};
        result.reserve(c_initialKeySize);
        result.assign(base);
        result.append(Meta::WcsText::c_solidusStr);
        if constexpr (Meta::isWide<T>) {
            result.append(key.s, key.length);
        } else {
            result.append(Text::convert(Meta::MbsText::View { key.s, key.length }));
        }
        return result;
    }
}
