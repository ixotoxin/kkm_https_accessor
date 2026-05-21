// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "meta.h"
#include "except.h"
#include <limits>
#include <algorithm>

namespace Numeric {
    using Basic::DataError;

    template<std::integral T, std::integral U>
    [[maybe_unused]]
    bool fits(U value) {
        if constexpr (std::is_signed_v<T>) {
            if constexpr (std::is_signed_v<U>) {
                if constexpr (sizeof(T) >= sizeof(U)) {
                    return true;
                } else {
                    /** int => int **/
                    return value >= static_cast<U>(std::numeric_limits<T>::min())
                        && value <= static_cast<U>(std::numeric_limits<T>::max());
                }
            } else {
                /** uint => int **/
                return value <= static_cast<U>(std::numeric_limits<T>::max());
            }
        } else {
            if constexpr (std::is_unsigned_v<U>) {
                if constexpr (sizeof(T) >= sizeof(U)) {
                    return true;
                } else {
                    /** uint => uint **/
                    return value <= static_cast<U>(std::numeric_limits<T>::max());
                }
            } else {
                /** int => uint **/
                return value >= 0
                    && static_cast<T>(value) <= std::numeric_limits<T>::max();
            }
        }
    }

    template<std::integral T>
    [[maybe_unused]]
    T safeCast(std::integral auto value) {
        if (Numeric::fits<T>(value)) {
            return static_cast<T>(value);
        }
        throw DataError(Basic::Wcs::c_invalidValue);
    }

    template<std::signed_integral T>
    [[maybe_unused]]
    void doubleClamp(T & a, T & b, const T min, const T max) {
        if (a < min) {
            a = min;
        } else if (a > max) {
            a = max;
        }
        if (b < min || b > max) {
            b = a;
        }
    }

    template<Meta::String T, Meta::Numeric U>
    [[nodiscard, maybe_unused]]
    T cast(U num) {
        if constexpr (std::is_same_v<T, std::wstring>) {
            return std::to_wstring(num);
        } else {
            return std::to_string(num);
        }
    }

    template<Meta::View T, Meta::BoolTag U>
    [[nodiscard, maybe_unused]]
    T boolCast(auto && value) {
        return static_cast<bool>(value) ? Meta::BoolLabels<T, U>::c_true : Meta::BoolLabels<T, U>::c_false;
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    auto clamp(T min, T max) {
        return [min, max] (const T value) -> T { return std::clamp(value, min, max); };
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    auto min(T min) {
        return
            [min] (const T value) -> T {
                if (value < min) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return value;
            };
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    auto max(T max) {
        return
            [max] (const T value) -> T {
                if (value > max) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return value;
            };
    }

    template<Meta::Numeric T>
    [[nodiscard, maybe_unused]]
    auto between(T min, T max) {
        return
            [min, max] (const T value) -> T {
                if (value < min || value > max) {
                    throw DataError(Basic::Wcs::c_rangeError);
                }
                return value;
            };
    }
}
