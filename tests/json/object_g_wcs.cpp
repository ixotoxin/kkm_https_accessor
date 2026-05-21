// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/object.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Handle WCS JSON object with callback", "[json][wcs]") {
        using Json::Wcs::Value;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison { jsonDoc };

        REQUIRE_FALSE(ison[nullKey]([] (const Json::Wcs::Value &, const Json::PtrView) { return true; }));
        REQUIRE(ison[emptyObjectKey].handle([] (const Json::Wcs::Value &, const Json::PtrView) { return true; }));
        REQUIRE(ison[emptyArrayKey]([] (const Json::Wcs::Value &, const Json::PtrView) { return true; }));

        {
            SizeType testValue {};
            REQUIRE(
                ison[arrayKey](
                    [& testValue] (const Json::Wcs::Value & v, const Json::PtrView)
                    { testValue = v.Size(); return true; }
                )
            );
            REQUIRE(testValue == 5);
        }

        REQUIRE(ison[trueKey]([] (const Json::Wcs::Value & v, const Json::PtrView) { return v.GetBool(); }));
        REQUIRE_FALSE(ison[falseKey]([] (const Json::Wcs::Value & v, const Json::PtrView) { return v.GetBool(); }));

        REQUIRE_THROWS_AS(
            ison[zeroNumKey].handle(
                [] (const Json::Wcs::Value &, const Json::PtrView) -> bool {
                    throw Basic::DataError { Basic::Wcs::c_invalidValue };
                }
            ),
            Basic::DataError
        );

        REQUIRE(
            ison[strKey].handle(
                [] (const Json::Wcs::Value & v, const Json::PtrView) {
                    REQUIRE(v.IsString());
                    return v.GetString() == strVal;
                }
            )
        );

        REQUIRE(
            ison[subKey](
                [] (const Json::Wcs::Value & v, const Json::PtrView) {
                    return Json::handle(v, [] (const Json::Wcs::Value & v2, const Json::PtrView) {
                        REQUIRE(v2.IsObject());
                        REQUIRE(v2.MemberCount() == 25);
                        REQUIRE(v2.HasMember(trueKey));
                        REQUIRE(v2[kv(trueKey)].IsBool());
                        return v2[kv(trueKey)].GetBool();
                    });
                }
            )
        );

        REQUIRE_FALSE(
            ison[nepKey].handle(
                [] (const Json::Wcs::Value &, const Json::PtrView) -> bool {
                    throw Basic::DataError { Basic::Wcs::c_invalidValue };
                }
            )
        );
    }

    TEST_CASE("Get value by key from WCS JSON object", "[json][wcs]") {
        using Catch::Matchers::WithinRel;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison { jsonDoc, subKey };

        {
            bool testValue { false };
            REQUIRE_NOTHROW(ison[trueKey].get(testValue));
            REQUIRE(testValue);
            REQUIRE_NOTHROW(ison[falseKey] >>= testValue);
            REQUIRE_FALSE(testValue);
            REQUIRE_NOTHROW(ison[trueStr3Key].get(testValue));
            REQUIRE(testValue);
            REQUIRE_NOTHROW(ison[falseStr3Key] >>= testValue);
            REQUIRE_FALSE(testValue);
            REQUIRE_THROWS_AS(ison[strKey] >>= testValue, Basic::DataError);
            REQUIRE_THROWS_AS(ison[nepKey] >>= testValue, Basic::DataError);
        }

        {
            unsigned short testValue {};
            REQUIRE_NOTHROW(ison[pintNumKey].get(testValue));
            REQUIRE(testValue == pintNumVal);
            REQUIRE_NOTHROW(ison[pintStrKey] >>= testValue);
            REQUIRE(testValue == pintNumVal);
            REQUIRE_THROWS_AS(ison[nintNumKey].get(testValue), Basic::DataError);
            REQUIRE_THROWS_AS(ison[nintStrKey] >>= testValue, Basic::DataError);
            REQUIRE_THROWS_AS(ison[pdblNumKey].get(testValue), Basic::DataError);
            REQUIRE_THROWS_AS(ison[pdblStrKey] >>= testValue, Basic::DataError);
            REQUIRE_THROWS_AS(ison[strKey] >>= testValue, Basic::DataError);
            REQUIRE_THROWS_AS(ison[nepKey] >>= testValue, Basic::DataError);
        }

        {
            double testValue {};
            REQUIRE_NOTHROW(ison[pdblNumKey].get(testValue));
            REQUIRE_THAT(testValue, WithinRel(pdblNumVal, 0.000001));
            REQUIRE_NOTHROW(ison[pdblStrKey].get(testValue));
            REQUIRE_THAT(testValue, WithinRel(pdblNumVal, 0.000001));
            REQUIRE_NOTHROW(ison[ndblNumKey] >>= testValue);
            REQUIRE_THAT(testValue, WithinRel(ndblNumVal, 0.000001));
            REQUIRE_NOTHROW(ison[ndblStrKey] >>= testValue);
            REQUIRE_THAT(testValue, WithinRel(ndblNumVal, 0.000001));
            REQUIRE_NOTHROW(ison[pintNumKey].get(testValue));
            REQUIRE_THAT(testValue, WithinRel(pintNumVal, 0.000001));
            REQUIRE_NOTHROW(ison[nintNumKey] >>= testValue);
            REQUIRE_THAT(testValue, WithinRel(nintNumVal, 0.000001));
            REQUIRE_THROWS_AS(ison[strKey] >>= testValue, Basic::DataError);
            REQUIRE_THROWS_AS(ison[nepKey] >>= testValue, Basic::DataError);
        }

        {
            std::wstring testValue { false };
            REQUIRE_NOTHROW(ison[trueKey].get(testValue));
            REQUIRE(testValue == Text::yesNo<Meta::Wcs>(trueVal));
            REQUIRE_NOTHROW(ison[falseKey] >>= testValue);
            REQUIRE(testValue == Text::yesNo<Meta::Wcs>(falseVal));
            REQUIRE_NOTHROW(ison[trueStr1Key].get(testValue));
            REQUIRE(testValue == trueStr1Val);
            REQUIRE_NOTHROW(ison[falseStr1Key] >>= testValue);
            REQUIRE(testValue == falseStr1Val);
            REQUIRE_NOTHROW(ison[strKey] >>= testValue);
            REQUIRE(testValue == strVal);
            REQUIRE_THROWS_AS(ison[nepKey].get(testValue), Basic::DataError);
        }
    }

    TEST_CASE("Get value by key from WCS JSON object within domain", "[json][wcs]") {
        using Basic::RefPair;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison { jsonDoc[kv(subKey)], jsonDoc };

        {
            unsigned short testValue {};
            REQUIRE_THROWS_AS(ison[zeroNumKey].get(testValue, intDomain), Basic::DataError);
            REQUIRE_NOTHROW(ison[pintNumKey].get(testValue, intDomain));
            REQUIRE(testValue == pintNumVal);
            REQUIRE_NOTHROW(ison[pintStrKey] >>= RefPair { testValue, intDomain });
            REQUIRE(testValue == pintNumVal);
            REQUIRE_THROWS_AS(ison[nintNumKey].get(testValue, intDomain), Basic::DataError);
            REQUIRE_THROWS_AS(ison[nintStrKey].get(testValue, intDomain), Basic::DataError);
            REQUIRE_THROWS_AS(ison[strKey].get(testValue, intDomain), Basic::DataError);
            REQUIRE_THROWS_AS(ison[nepKey].get(testValue, intDomain), Basic::DataError);
        }

        {
            std::wstring testValue {};
            REQUIRE_THROWS_AS(ison[zeroNumKey].get(testValue, strDomain), Basic::DataError);
            REQUIRE_NOTHROW(ison[pintNumKey].get(testValue, strDomain));
            REQUIRE(testValue == pintStrVal);
            REQUIRE_NOTHROW(ison[pintStrKey] >>= RefPair { testValue, strDomain });
            REQUIRE(testValue == pintStrVal);
            REQUIRE_NOTHROW(ison[nintNumKey].get(testValue, strDomain));
            REQUIRE(testValue == nintStrVal);
            REQUIRE_NOTHROW(ison[nintStrKey].get(testValue, strDomain));
            REQUIRE(testValue == nintStrVal);
            REQUIRE_NOTHROW(ison[strKey].get(testValue, strDomain));
            REQUIRE_FALSE(testValue == strVal);
            REQUIRE(testValue == loweredStrVal);
            REQUIRE_THROWS_AS(ison[nepKey].get(testValue, strDomain), Basic::DataError);
        }

        {
            std::wstring testValue {};
            REQUIRE_THROWS_AS(ison[zeroNumKey].get(testValue, svwDomain), Basic::DataError);
            REQUIRE_NOTHROW(ison[pintNumKey].get(testValue, svwDomain));
            REQUIRE(testValue == pintStrVal);
            REQUIRE_NOTHROW(ison[pintStrKey] >>= RefPair { testValue, svwDomain });
            REQUIRE(testValue == pintStrVal);
            REQUIRE_NOTHROW(ison[nintNumKey].get(testValue, svwDomain));
            REQUIRE(testValue == nintStrVal);
            REQUIRE_NOTHROW(ison[nintStrKey].get(testValue, svwDomain));
            REQUIRE(testValue == nintStrVal);
            REQUIRE_NOTHROW(ison[strKey].get(testValue, svwDomain));
            REQUIRE_FALSE(testValue == strVal);
            REQUIRE(testValue == loweredStrVal);
            REQUIRE_THROWS_AS(ison[nepKey].get(testValue, svwDomain), Basic::DataError);
        }
    }

    TEST_CASE("Get value by key from WCS JSON object with cast map", "[json][wcs]") {
        using Basic::RefPair;

        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison0 { jsonDoc };
        Json::Object ison { ison0, subKey };

        {
            unsigned short testValue {};
            REQUIRE_THROWS_AS(ison[zeroNumKey].get(testValue, v2iCastMap), Basic::DataError);
            REQUIRE_THROWS_AS(ison[pintNumKey].get(testValue, v2iCastMap), Basic::DataError);
            REQUIRE_THROWS_AS(ison[pintStrKey].get(testValue, v2iCastMap), Basic::DataError);
            REQUIRE_NOTHROW(ison[nintNumKey].get(testValue, v2iCastMap));
            REQUIRE(testValue == pintNumVal);
            REQUIRE_NOTHROW(ison[nintStrKey] >>= RefPair { testValue, v2iCastMap });
            REQUIRE(testValue == pintNumVal);
            REQUIRE_THROWS_AS(ison[strKey].get(testValue, v2iCastMap), Basic::DataError);
            REQUIRE_THROWS_AS(ison[nepKey].get(testValue, v2iCastMap), Basic::DataError);
        }

        {
            std::wstring testValue {};
            REQUIRE_THROWS_AS(ison[zeroNumKey].get(testValue, i2sCastMap), Basic::DataError);
            REQUIRE_NOTHROW(ison[pintNumKey].get(testValue, i2sCastMap));
            REQUIRE(testValue == cookieVal);
            REQUIRE_NOTHROW(ison[pintStrKey] >>= RefPair { testValue, i2sCastMap });
            REQUIRE(testValue == cookieVal);
            REQUIRE_NOTHROW(ison[nintNumKey].get(testValue, i2sCastMap));
            REQUIRE(testValue == coalVal);
            REQUIRE_NOTHROW(ison[nintStrKey].get(testValue, i2sCastMap));
            REQUIRE(testValue == coalVal);
            REQUIRE_THROWS_AS(ison[strKey].get(testValue, i2sCastMap), Basic::DataError);
            REQUIRE_THROWS_AS(ison[nepKey].get(testValue, i2sCastMap), Basic::DataError);
        }

        {
            std::wstring testValue {};
            REQUIRE_THROWS_AS(ison[zeroNumKey].get(testValue, s2vCastMap), Basic::DataError);
            REQUIRE_NOTHROW(ison[pintNumKey].get(testValue, s2vCastMap));
            REQUIRE(testValue == cookieVal);
            REQUIRE_NOTHROW(ison[pintStrKey] >>= RefPair { testValue, s2vCastMap });
            REQUIRE(testValue == cookieVal);
            REQUIRE_NOTHROW(ison[nintNumKey].get(testValue, s2vCastMap));
            REQUIRE(testValue == coalVal);
            REQUIRE_NOTHROW(ison[nintStrKey].get(testValue, s2vCastMap));
            REQUIRE(testValue == coalVal);
            REQUIRE_THROWS_AS(ison[strKey].get(testValue, s2vCastMap), Basic::DataError);
            REQUIRE_THROWS_AS(ison[nepKey].get(testValue, s2vCastMap), Basic::DataError);
        }
    }
}
