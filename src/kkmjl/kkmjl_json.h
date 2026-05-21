// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <json/object.h>

namespace KkmJsonLoader {
    using namespace Json::Literals;
    using JsonAlloc = Json::Allocator;
    using JsonDoc = Json::Wcs::Document;
    using JsonVal = Json::Wcs::Value;
    using JsonStr = Json::Wcs::StrRef;
    using JsonPtr = Json::PtrView;
    using JsonObj = Json::Object<Json::Wcs::Document>;
    using Json::kv;

    namespace Wcs {
        inline auto c_id = L"!id"_key;
        inline auto c_query = L"!query"_key;
    }
}
