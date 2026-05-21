// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/object.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Set property of WCS JSON object #1", "[json][wcs]") {
        using Basic::RefPair;
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };
        prepareObject(jsonDoc);
        Json::Object ison { jsonDoc, subKey };

        REQUIRE_NOTHROW(ison[L"prop1"_key].set(Json::Wcs::Value { 12345 }));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetInt() == 12345);

        REQUIRE_NOTHROW(ison[L"prop2"_key] <<= Json::Wcs::Value { L"Cookie" });
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetString() == L"Cookie"s);

        REQUIRE_NOTHROW(ison[L"prop3"_key].set(Json::Wcs::Value(Json::Type::Null)));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop3"_key)].IsNull());

        REQUIRE_NOTHROW(ison[L"prop4"_key] <<= Json::Wcs::Value(Json::Type::Object));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop4"_key)].IsObject());

        REQUIRE_NOTHROW(ison[L"prop5"_key].set(true));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop5"_key)].IsBool());

        REQUIRE_NOTHROW(ison[L"prop6"_key].set(trueVal));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop6"_key)].IsTrue());

        REQUIRE_NOTHROW(ison[L"prop7"_key] <<= false);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop7"_key)].IsBool());

        REQUIRE_NOTHROW(ison[L"prop8"_key] <<= falseVal);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop8"_key)].IsFalse());

        REQUIRE_NOTHROW(ison[L"prop1"_key].set(12345));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetInt() == 12345);

        REQUIRE_NOTHROW(ison[L"prop2"_key].set(pintNumVal));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetInt() == pintNumVal);

        REQUIRE_NOTHROW(ison[L"prop3"_key] <<= -12345);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop3"_key)].GetInt() == -12345);

        REQUIRE_NOTHROW(ison[L"prop4"_key] <<= nintNumVal);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop4"_key)].GetInt() == nintNumVal);

        REQUIRE_NOTHROW(ison[L"prop5"_key].set(12345.67));
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv(L"prop5"_key)].GetDouble(), WithinRel(12345.67, 0.000001));

        REQUIRE_NOTHROW(ison[L"prop6"_key].set(pdblNumVal));
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv(L"prop6"_key)].GetDouble(), WithinRel(pdblNumVal, 0.000001));

        REQUIRE_NOTHROW(ison[L"prop7"_key] <<= -12345.67);
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv(L"prop7"_key)].GetDouble(), WithinRel(-12345.67, 0.000001));

        REQUIRE_NOTHROW(ison[L"prop8"_key] <<= ndblNumVal);
        REQUIRE_THAT(jsonDoc[kv(subKey)][kv(L"prop8"_key)].GetDouble(), WithinRel(ndblNumVal, 0.000001));

        REQUIRE_NOTHROW(ison[L"prop1"_key].set(L"Cookie"));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetString() == L"Cookie"s);

        REQUIRE_NOTHROW(ison[L"prop2"_key].set(L"Cookie"s));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetString() == L"Cookie"s);

        REQUIRE_NOTHROW(ison[L"prop3"_key].set(L"Cookie"sv));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop3"_key)].GetString() == L"Cookie"s);

        REQUIRE_NOTHROW(ison[L"prop4"_key].set(strVal));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop4"_key)].GetString() == strVal);

        REQUIRE_NOTHROW(ison[L"prop5"_key] <<= L"Cookie");
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop5"_key)].GetString() == L"Cookie"s);

        REQUIRE_NOTHROW(ison[L"prop6"_key] <<= L"Cookie"s);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop6"_key)].GetString() == L"Cookie"s);

        REQUIRE_NOTHROW(ison[L"prop7"_key] <<= L"Cookie"sv);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop7"_key)].GetString() == L"Cookie"s);

        REQUIRE_NOTHROW(ison[L"prop8"_key] <<= strVal);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop8"_key)].GetString() == strVal);

        REQUIRE_NOTHROW(ison[L"prop1"_key].set(tmVal));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetString() == tmStrVal);

        REQUIRE_NOTHROW(ison[L"prop2"_key] <<= tmVal);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetString() == tmStrVal);
    }

    TEST_CASE("Set property of WCS JSON object #2", "[json][wcs]") {
        using Basic::RefPair;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };
        prepareObject(jsonDoc);
        Json::Object ison { jsonDoc, subKey };

        REQUIRE_NOTHROW(ison[L"prop1"_key].set(1, intDomain));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetInt() == 1);

        REQUIRE_NOTHROW(ison[L"prop2"_key].set(pintNumVal, intDomain));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetInt() == pintNumVal);

        REQUIRE_NOTHROW(ison[L"prop3"_key] <<= RefPair { nintNumVal, intDomain });
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop3"_key)].GetInt() == nintNumVal);

        REQUIRE_THROWS_AS(ison[L"prop3"_key].set(zeroStrVal, s2vCastMap), Basic::DataError);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop3"_key)].GetInt() == nintNumVal);

        REQUIRE_NOTHROW(ison[L"prop4"_key].set(L"3", strDomain));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop4"_key)].GetString() == L"3"s);

        REQUIRE_NOTHROW(ison[L"prop5"_key].set(pintStrVal, strDomain));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop5"_key)].GetString() == pintStrVal);

        REQUIRE_NOTHROW(ison[L"prop6"_key] <<= RefPair { nintStrVal, strDomain });
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop6"_key)].GetString() == nintStrVal);

        REQUIRE_THROWS_AS(ison[L"prop6"_key].set(zeroStrVal, strDomain), Basic::DataError);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop6"_key)].GetString() == nintStrVal);

        REQUIRE_NOTHROW(ison[L"prop7"_key].set(L"5", svwDomain));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop7"_key)].GetString() == L"5"s);

        REQUIRE_NOTHROW(ison[L"prop8"_key].set(pintStrVal, svwDomain));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop8"_key)].GetString() == pintStrVal);

        REQUIRE_NOTHROW(ison[L"prop9"_key] <<= RefPair { nintStrVal, svwDomain });
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop9"_key)].GetString() == nintStrVal);

        REQUIRE_THROWS_AS(ison[L"prop9"_key].set(zeroStrVal, s2vCastMap), Basic::DataError);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop9"_key)].GetString() == nintStrVal);
    }

    TEST_CASE("Set property of WCS JSON object #3", "[json][wcs]") {
        using Basic::RefPair;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };
        prepareObject(jsonDoc);
        Json::Object ison { jsonDoc, subKey };

        REQUIRE_NOTHROW(ison[L"prop1"_key].set(L"\t One \t", v2iCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetInt() == oneNumVal);

        REQUIRE_NOTHROW(ison[L"prop2"_key].set(L"\t thrEE \t"s, v2iCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetInt() == threeNumVal);

        REQUIRE_NOTHROW(ison[L"prop3"_key].set(L"fIvE"sv, v2iCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop3"_key)].GetInt() == fiveNumVal);

        REQUIRE_NOTHROW(ison[L"prop4"_key].set(sevenStrVal, v2iCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop4"_key)].GetInt() == sevenNumVal);

        REQUIRE_NOTHROW(ison[L"prop5"_key] <<= RefPair { nineStrVal, v2iCastMap });
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop5"_key)].GetInt() == nineNumVal);

        REQUIRE_THROWS_AS(ison[L"prop5"_key].set(zeroStrVal, v2iCastMap), Basic::DataError);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop5"_key)].GetInt() == nineNumVal);

        REQUIRE_NOTHROW(ison[L"prop6"_key].set(1, i2sCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop6"_key)].GetString() == oneStrVal);

        REQUIRE_NOTHROW(ison[L"prop7"_key].set(threeNumVal, i2sCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop7"_key)].GetString() == threeStrVal);

        REQUIRE_NOTHROW(ison[L"prop8"_key] <<= RefPair { fiveNumVal, i2sCastMap });
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop8"_key)].GetString() == fiveStrVal);

        REQUIRE_THROWS_AS(ison[L"prop8"_key].set(zeroStrVal, s2vCastMap), Basic::DataError);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop8"_key)].GetString() == fiveStrVal);

        REQUIRE_NOTHROW(ison[L"prop9"_key].set(L"\t One \t", s2vCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop9"_key)].GetString() == oneNumStrVal);

        REQUIRE_NOTHROW(ison[L"prop1"_key].set(L"\t thrEE \t"s, s2vCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop1"_key)].GetString() == threeNumStrVal);

        REQUIRE_NOTHROW(ison[L"prop2"_key].set(L"fIvE"sv, s2vCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop2"_key)].GetString() == fiveNumStrVal);

        REQUIRE_NOTHROW(ison[L"prop3"_key].set(sevenStrVal, s2vCastMap));
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop3"_key)].GetString() == sevenNumStrVal);

        REQUIRE_NOTHROW(ison[L"prop4"_key] <<= RefPair { nineStrVal, s2vCastMap });
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop4"_key)].GetString() == nineNumStrVal);

        REQUIRE_THROWS_AS(ison[L"prop4"_key].set(zeroStrVal, s2vCastMap), Basic::DataError);
        REQUIRE(jsonDoc[kv(subKey)][kv(L"prop4"_key)].GetString() == nineNumStrVal);
    }
}
