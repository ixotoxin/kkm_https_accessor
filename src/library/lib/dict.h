// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include "except.h"
#include "text.h"

namespace Domain {
    template<typename T, Meta::NotText I, Meta::Domain<T> D>
    requires std::convertible_to<I, T>
    [[maybe_unused]]
    T cast(I && value, const D & domain) {
        if (std::ranges::find(domain, value) == domain.end()) {
            throw Basic::DataError(Basic::Wcs::c_rangeError);
        }
        if constexpr (std::is_scalar_v<T>) {
            if constexpr (Meta::Integral<T>) {
                return Numeric::safeCast<T>(value);
            } else {
                return static_cast<T>(value);
            }
        } else {
            return { std::forward<I>(value) };
        }
    }

    template<Meta::String T, Meta::Text<T> I, Meta::TextDomain<T> D>
    [[maybe_unused]]
    T cast(I && value, const D & domain) {
        Text::String<T> temp { std::forward<I>(value) };
        Text::trim(temp);
        Text::lower(temp);
        if (std::ranges::find(domain, temp) == domain.end()) {
            throw Basic::DataError(Basic::Wcs::c_rangeError);
        }
        return temp;
    }
}

namespace Dictionary {
    template<typename T, Meta::NotText K, Meta::CastMap<K, T> M>
    [[maybe_unused]]
    T cast(K && value, const M & castMap, const bool noMissing /*= true*/) {
        if (auto it = castMap.find(value); it != castMap.end()) {
            if constexpr (std::is_scalar_v<T>) {
                if constexpr (Meta::Integral<T>) {
                    return Numeric::safeCast<T>(it->second);
                } else {
                    return static_cast<T>(it->second);
                }
            } else {
                return T { it->second };
            }
        }
        if (noMissing) {
            throw Basic::DataError(Basic::Wcs::c_rangeError);
        }
        return {};
    }

    template<typename T, Meta::AnyText K, Meta::FromTextCastMap<K, T> M>
    [[maybe_unused]]
    T cast(K && value, const M & castMap, const bool noMissing /*= true*/) {
        Text::String<K> temp { std::forward<K>(value) };
        Text::trim(temp);
        Text::lower(temp);
        if (auto it = castMap.find(temp); it != castMap.end()) {
            if constexpr (std::is_scalar_v<T>) {
                if constexpr (Meta::Integral<T>) {
                    return Numeric::safeCast<T>(it->second);
                } else {
                    return static_cast<T>(it->second);
                }
            } else {
                return T { it->second };
            }
        }
        if (noMissing) {
            throw Basic::DataError(Basic::Wcs::c_rangeError);
        }
        return {};
    }
}
