// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "traits.h"

namespace Json {
    namespace Wcs {
        using Value = Meta::WcsJson::Value;
        using Document = Meta::WcsJson::Document;
        using Key = Meta::WcsJson::Key;
        using StrRef = Meta::WcsJson::StringRef;
    }

    namespace Mbs {
        using Value = Meta::MbsJson::Value;
        using Document = Meta::MbsJson::Document;
        using Key = Meta::MbsJson::Key;
        using StrRef = Meta::MbsJson::StringRef;
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
    using Value = Trait<T>::Value;

    template<typename T>
    using Document = Trait<T>::Document;

    template<typename T>
    using StringRef = Trait<T>::StringRef;

    template<typename T>
    using Key = Trait<T>::Key;

    template<typename T>
    using Handler = Trait<T>::Handler;

    namespace Type {
        static constexpr auto Null = rapidjson::kNullType;
        static constexpr auto Object = rapidjson::kObjectType;
        static constexpr auto Array = rapidjson::kArrayType;
    }
}
