// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "except.h"
#include <lib/meta.h>
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/dict.h>
#include <chrono>
#include <filesystem>

namespace Json {
    template<Meta::Bool T, Meta::Json J>
    [[nodiscard, maybe_unused]]
    T cast(const J & json) {
        if (json.IsBool()) {
            return json.GetBool();
        }
        /*if (json.IsInt()) {
            return static_cast<T>(json.GetInt());
        }
        if (json.IsUint()) {
            return static_cast<T>(json.GetUint());
        }
        if (json.IsInt64()) {
            return static_cast<T>(json.GetInt64());
        }
        if (json.IsUint64()) {
            return static_cast<T>(json.GetUint64());
        }*/
        if (json.IsString()) {
            return Text::cast<T>(View<J> { json.GetString() });
        }
        /*if (json.IsNull()) {
            return false;
        }*/
        throw DataError(Basic::Wcs::c_invalidValue);
    }

    template<Meta::Integral T, Meta::Json J>
    [[nodiscard, maybe_unused]]
    T cast(const J & json) {
        if (json.IsInt()) {
            return Numeric::safeCast<T>(json.GetInt());
        }
        if (json.IsUint()) {
            return Numeric::safeCast<T>(json.GetUint());
        }
        if (json.IsInt64()) {
            return Numeric::safeCast<T>(json.GetInt64());
        }
        if (json.IsUint64()) {
            return Numeric::safeCast<T>(json.GetUint64());
        }
        if (json.IsString()) {
            return Text::cast<T>(View<J> { json.GetString() });
        }
        /*if (json.IsBool()) {
            return static_cast<T>(json.GetBool());
        }*/
        /*if (json.IsNull()) {
            return 0;
        }*/
        throw DataError(Basic::Wcs::c_invalidValue);
    }

    template<Meta::FloatingPoint T, Meta::Json J>
    [[nodiscard, maybe_unused]]
    T cast(const J & json) {
        if (json.IsInt()) {
            return static_cast<T>(json.GetInt());
        }
        if (json.IsUint()) {
            return static_cast<T>(json.GetUint());
        }
        if (json.IsInt64()) {
            return static_cast<T>(json.GetInt64());
        }
        if (json.IsUint64()) {
            return static_cast<T>(json.GetUint64());
        }
        if (json.IsDouble()) {
            return static_cast<T>(json.GetDouble());
        }
        if (json.IsString()) {
            return Text::cast<T>(View<J> { json.GetString() });
        }
        /*if (json.IsBool()) {
            return static_cast<T>(json.GetBool());
        }*/
        /*if (json.IsNull()) {
            return 0;
        }*/
        throw DataError(Basic::Wcs::c_invalidValue);
    }

    template<Meta::fromTemplate<std::chrono::duration> T>
    [[nodiscard, maybe_unused]]
    T cast(const Meta::Json auto & json) {
        return T(cast<typename T::rep>(json));
    }

    template<Meta::String T, Meta::Json J>
    [[nodiscard, maybe_unused]]
    T cast(const J & json) {
        if (json.IsString()) {
            if constexpr (std::is_same_v<Wideness<T>, Wideness<J>>) {
                return json.GetString();
            } else {
                return Text::convert(json.GetString());
            }
        }
        if (json.IsInt()) {
            return Numeric::cast<T>(json.GetInt());
        }
        if (json.IsUint()) {
            return Numeric::cast<T>(json.GetUint());
        }
        if (json.IsInt64()) {
            return Numeric::cast<T>(json.GetInt64());
        }
        if (json.IsUint64()) {
            return Numeric::cast<T>(json.GetUint64());
        }
        if (json.IsDouble()) {
            return Numeric::cast<T>(json.GetDouble());
        }
        if (json.IsBool()) {
            auto value = Text::yesNo<Wideness<T>>(json.GetBool());
            return { value.data(), value.size() };
        }
        /*if (json.IsNull()) {
            return {};
        }*/
        throw DataError(Basic::Wcs::c_invalidValue);
    }

    template<std::same_as<std::filesystem::path> T, Meta::Json J>
    [[nodiscard, maybe_unused]]
    T cast(const J & json) {
        if (json.IsString()) {
            if constexpr (Meta::isWide<J>) {
                return json.GetString();
            } else {
                return Text::convert(json.GetString());
            }
        }
        throw DataError(Basic::Wcs::c_invalidValue);
    }

    template<Meta::NotText T, Meta::Domain<T> D>
    [[maybe_unused]]
    T cast(const Meta::Json auto & json, const D & domain) {
        return Domain::cast<T>(cast<Meta::ValueType<D>>(json), domain);
    }

    template<Meta::String T, Meta::TextDomain<T> D>
    [[maybe_unused]]
    T cast(const Value<T> & json, const D & domain) {
        return Domain::cast<T>(cast<T>(json), domain);
    }

    template<Meta::NotText T, Meta::CastTo<T> M>
    requires Meta::isNotText<Meta::KeyType<M>>
    [[maybe_unused]]
    T cast(const Meta::Json auto & json, const M & castMap) {
        return Dictionary::cast<T>(cast<Meta::KeyType<M>>(json), castMap, true);
    }

    template<Meta::NotText T, Meta::Json J, Meta::CastTo<T> M>
    requires Meta::TextClass<Meta::KeyType<M>, Wideness<J>>
    [[maybe_unused]]
    T cast(const J & json, const M & castMap) {
        return Dictionary::cast<T>(cast<String<J>>(json), castMap, true);
    }

    template<Meta::String T, Meta::CastToText<T> M>
    requires Meta::isNotText<Meta::KeyType<M>>
    [[maybe_unused]]
    T cast(const Meta::Json auto & json, const M & castMap) {
        return Dictionary::cast<T>(cast<Meta::KeyType<M>>(json), castMap, true);
    }

    template<Meta::String T, Meta::Json J, Meta::CastToText<T> M>
    requires Meta::TextClass<Meta::KeyType<M>, Wideness<J>>
    [[maybe_unused]]
    T cast(const J & json, const M & castMap) {
        return Dictionary::cast<T>(cast<String<J>>(json), castMap, true);
    }
}
