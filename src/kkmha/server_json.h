// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <json/object.h>
#include <json/inout.h>

namespace Server {
    using namespace Json::Literals;
    using JsonAlloc = Json::Allocator;
    using JsonDoc = Json::Wcs::Document;
    using JsonVal = Json::Wcs::Value;
    using JsonPtr = Json::PtrView;
    using JsonObj = Json::Object<Json::Wcs::Document>;
    using Json::kv;
}
