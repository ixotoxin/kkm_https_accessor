// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "stream.h"
#include "allocator.h"
#include <lib/text.h>
#include <rapidjson/encodings.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <functional>

namespace Json {
    using rapidjson::SizeType;
    using Ptr = Meta::WcsText::String;
    using PtrView = Meta::WcsText::View;
}

namespace Meta {
    struct MbsJson;
    struct WcsJson;

    struct WcsJson : WcsText {
        using Opposite = MbsJson;
        using Encoding = rapidjson::UTF16<>;
        using Value = rapidjson::GenericValue<Encoding, Json::Allocator>;
        using Document = rapidjson::GenericDocument<Encoding, Json::Allocator>;
        using StringRef = rapidjson::GenericStringRef<Char>;
        using Key = StringRef;
        using StringBuffer = rapidjson::GenericStringBuffer<Encoding, Json::Allocator>;
        using StringInputStream = Json::StringInputStream<Wcs>;
        using StringOutputStream = Json::StringOutputStream<Wcs>;
        using Handler = std::function<bool(const Value &, Json::PtrView)>;
    };

    struct MbsJson : MbsText {
        using Opposite = WcsJson;
        using Encoding = rapidjson::UTF8<>;
        using Value = rapidjson::GenericValue<Encoding, Json::Allocator>;
        using Document = rapidjson::GenericDocument<Encoding, Json::Allocator>;
        using StringRef = rapidjson::GenericStringRef<Char>;
        using Key = StringRef;
        using StringBuffer = rapidjson::GenericStringBuffer<Encoding, Json::Allocator>;
        using StringInputStream = Json::StringInputStream<Mbs>;
        using StringOutputStream = Json::StringOutputStream<Mbs>;
        using Handler = std::function<bool(const Value &, Json::PtrView)>;
    };

    template<>
    struct WideTypes<WcsJson::Value> : std::true_type {};

    template<>
    struct WideTypes<WcsJson::Document> : std::true_type {};

    template<>
    struct WideTypes<WcsJson::Key> : std::true_type {};

    template<typename> struct JsonTrait {};
    template<> struct JsonTrait<Wcs> : WcsJson {};
    template<> struct JsonTrait<Mbs> : MbsJson {};
    template<> struct JsonTrait<WcsJson::Value> : WcsJson {};
    template<> struct JsonTrait<MbsJson::Value> : MbsJson {};
    template<> struct JsonTrait<WcsJson::Document> : WcsJson {};
    template<> struct JsonTrait<MbsJson::Document> : MbsJson {};
    template<> struct JsonTrait<WcsJson::Key> : WcsJson {};
    template<> struct JsonTrait<MbsJson::Key> : MbsJson {};
    template<> struct JsonTrait<WcsJson::Char> : WcsJson {};
    template<> struct JsonTrait<MbsJson::Char> : MbsJson {};

    template<typename T>
    requires Meta::Char<ValueType<T>>
    struct JsonTrait<T> : JsonTrait<ValueType<T>> {};

    template<typename T>
    constexpr bool isJsonValue = Meta::fromTemplate<T, rapidjson::GenericValue>;

    template<typename T>
    concept JsonValue = isJsonValue<UnderlyingType<T>>;

    template<class T>
    constexpr bool isJsonDocument = Meta::fromTemplate<T, rapidjson::GenericDocument>;

    template<class T>
    concept JsonDocument = isJsonDocument<UnderlyingType<T>>;

    /*template<typename T>
    constexpr bool isJsonKey = std::is_same_v<T, WcsJson::Key> || std::is_same_v<T, MbsJson::Key>;*/

    template<typename T>
    constexpr bool isJsonKey = Meta::fromTemplate<T, rapidjson::GenericStringRef>;

    template<typename T>
    concept JsonKey = isJsonKey<UnderlyingType<T>>;

    template<typename T>
    concept Json = JsonDocument<T> || JsonValue<T>;
}

namespace Json {
    template<typename T>
    using Trait = Meta::JsonTrait<Meta::UnderlyingType<T>>;
}
