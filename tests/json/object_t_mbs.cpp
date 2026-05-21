// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_mbs.h"
#include <json/object.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Mbs {
    TEST_CASE("Handle MBS JSON object wrapper's predicates #1", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison { jsonDoc };

        REQUIRE_FALSE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 26);

        REQUIRE(ison[nullKey].rjExists());
        REQUIRE(ison[nullKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjExists());
        REQUIRE(ison[emptyObjectKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjIsObject());
        REQUIRE_FALSE(ison[emptyObjectKey].rjIsArray());
        REQUIRE(ison[emptyArrayKey].rjExists());
        REQUIRE(ison[emptyArrayKey].rjEmpty());
        REQUIRE_FALSE(ison[emptyArrayKey].rjIsObject());
        REQUIRE(ison[emptyArrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjExists());
        REQUIRE_FALSE(ison[arrayKey].rjEmpty());
        REQUIRE(ison[arrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjCount() == 5);
        REQUIRE(ison[trueKey].rjExists());
        REQUIRE_FALSE(ison[trueKey].rjEmpty());
        REQUIRE(ison[trueKey].rjIsBool());
        REQUIRE(ison[trueKey].rjIsTrue());
        REQUIRE_FALSE(ison[trueKey].rjIsFalse());
        REQUIRE(ison[trueStr1Key].rjIsString());
        REQUIRE(ison[trueStr2Key].rjIsString());
        REQUIRE(ison[trueStr3Key].rjIsString());
        REQUIRE(ison[falseKey].rjExists());
        REQUIRE_FALSE(ison[falseKey].rjEmpty());
        REQUIRE(ison[falseKey].rjIsBool());
        REQUIRE_FALSE(ison[falseKey].rjIsTrue());
        REQUIRE(ison[falseKey].rjIsFalse());
        REQUIRE(ison[falseStr1Key].rjIsString());
        REQUIRE(ison[falseStr2Key].rjIsString());
        REQUIRE(ison[falseStr3Key].rjIsString());
        REQUIRE(ison[zeroNumKey].rjIsNumber());
        REQUIRE(ison[zeroStrKey].rjIsString());
        REQUIRE(ison[fiveNumKey].rjIsNumber());
        REQUIRE(ison[sevenStrKey].rjIsString());
        REQUIRE(ison[pintNumKey].rjIsNumber());
        REQUIRE(ison[pintNumKey].rjIsInt());
        REQUIRE(ison[pintNumKey].rjIsUint());
        REQUIRE(ison[pintNumKey].rjIsInt64());
        REQUIRE(ison[pintNumKey].rjIsUint64());
        REQUIRE(ison[pintStrKey].rjIsString());
        REQUIRE(ison[nintNumKey].rjIsNumber());
        REQUIRE(ison[nintNumKey].rjIsInt());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint());
        REQUIRE(ison[nintNumKey].rjIsInt64());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint64());
        REQUIRE(ison[nintStrKey].rjIsString());
        REQUIRE(ison[pdblNumKey].rjIsNumber());
        REQUIRE(ison[pdblNumKey].rjIsDouble());
        REQUIRE(ison[pdblStrKey].rjIsString());
        REQUIRE(ison[ndblNumKey].rjIsNumber());
        REQUIRE(ison[ndblNumKey].rjIsDouble());
        REQUIRE(ison[ndblStrKey].rjIsString());
        REQUIRE(ison[strKey].rjExists());
        REQUIRE_FALSE(ison[strKey].rjEmpty());
        REQUIRE(ison[strKey].rjIsString());
        REQUIRE(ison[subKey].rjExists());
        REQUIRE_FALSE(ison[subKey].rjEmpty());
        REQUIRE(ison[subKey].rjIsObject());
        REQUIRE(ison[subKey].rjCount() == 25);
        REQUIRE_FALSE(ison[nepKey].rjExists());
        REQUIRE(ison[nepKey].rjEmpty());

        REQUIRE_NOTHROW(ison.clear());
        REQUIRE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 0);
    }

    TEST_CASE("Handle MBS JSON object wrapper's predicates #2", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison { jsonDoc, subKey };

        REQUIRE_FALSE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 25);

        REQUIRE(ison[nullKey].rjExists());
        REQUIRE(ison[nullKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjExists());
        REQUIRE(ison[emptyObjectKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjIsObject());
        REQUIRE_FALSE(ison[emptyObjectKey].rjIsArray());
        REQUIRE(ison[emptyArrayKey].rjExists());
        REQUIRE(ison[emptyArrayKey].rjEmpty());
        REQUIRE_FALSE(ison[emptyArrayKey].rjIsObject());
        REQUIRE(ison[emptyArrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjExists());
        REQUIRE_FALSE(ison[arrayKey].rjEmpty());
        REQUIRE(ison[arrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjCount() == 5);
        REQUIRE(ison[trueKey].rjExists());
        REQUIRE_FALSE(ison[trueKey].rjEmpty());
        REQUIRE(ison[trueKey].rjIsBool());
        REQUIRE(ison[trueKey].rjIsTrue());
        REQUIRE_FALSE(ison[trueKey].rjIsFalse());
        REQUIRE(ison[trueStr1Key].rjIsString());
        REQUIRE(ison[trueStr2Key].rjIsString());
        REQUIRE(ison[trueStr3Key].rjIsString());
        REQUIRE(ison[falseKey].rjExists());
        REQUIRE_FALSE(ison[falseKey].rjEmpty());
        REQUIRE(ison[falseKey].rjIsBool());
        REQUIRE_FALSE(ison[falseKey].rjIsTrue());
        REQUIRE(ison[falseKey].rjIsFalse());
        REQUIRE(ison[falseStr1Key].rjIsString());
        REQUIRE(ison[falseStr2Key].rjIsString());
        REQUIRE(ison[falseStr3Key].rjIsString());
        REQUIRE(ison[zeroNumKey].rjIsNumber());
        REQUIRE(ison[zeroStrKey].rjIsString());
        REQUIRE(ison[fiveNumKey].rjIsNumber());
        REQUIRE(ison[sevenStrKey].rjIsString());
        REQUIRE(ison[pintNumKey].rjIsNumber());
        REQUIRE(ison[pintNumKey].rjIsInt());
        REQUIRE(ison[pintNumKey].rjIsUint());
        REQUIRE(ison[pintNumKey].rjIsInt64());
        REQUIRE(ison[pintNumKey].rjIsUint64());
        REQUIRE(ison[pintStrKey].rjIsString());
        REQUIRE(ison[nintNumKey].rjIsNumber());
        REQUIRE(ison[nintNumKey].rjIsInt());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint());
        REQUIRE(ison[nintNumKey].rjIsInt64());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint64());
        REQUIRE(ison[nintStrKey].rjIsString());
        REQUIRE(ison[pdblNumKey].rjIsNumber());
        REQUIRE(ison[pdblNumKey].rjIsDouble());
        REQUIRE(ison[pdblStrKey].rjIsString());
        REQUIRE(ison[ndblNumKey].rjIsNumber());
        REQUIRE(ison[ndblNumKey].rjIsDouble());
        REQUIRE(ison[ndblStrKey].rjIsString());
        REQUIRE(ison[strKey].rjExists());
        REQUIRE_FALSE(ison[strKey].rjEmpty());
        REQUIRE(ison[strKey].rjIsString());
        REQUIRE_FALSE(ison[subKey].rjExists());
        REQUIRE(ison[subKey].rjEmpty());
        REQUIRE_FALSE(ison[subKey].rjIsObject());
        REQUIRE_FALSE(ison[nepKey].rjExists());
        REQUIRE(ison[nepKey].rjEmpty());

        REQUIRE_NOTHROW(ison.clear());
        REQUIRE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 0);
    }

    TEST_CASE("Handle MBS JSON object wrapper's predicates #3", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison { jsonDoc[kv(subKey)], jsonDoc };

        REQUIRE_FALSE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 25);

        REQUIRE(ison[nullKey].rjExists());
        REQUIRE(ison[nullKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjExists());
        REQUIRE(ison[emptyObjectKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjIsObject());
        REQUIRE_FALSE(ison[emptyObjectKey].rjIsArray());
        REQUIRE(ison[emptyArrayKey].rjExists());
        REQUIRE(ison[emptyArrayKey].rjEmpty());
        REQUIRE_FALSE(ison[emptyArrayKey].rjIsObject());
        REQUIRE(ison[emptyArrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjExists());
        REQUIRE_FALSE(ison[arrayKey].rjEmpty());
        REQUIRE(ison[arrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjCount() == 5);
        REQUIRE(ison[trueKey].rjExists());
        REQUIRE_FALSE(ison[trueKey].rjEmpty());
        REQUIRE(ison[trueKey].rjIsBool());
        REQUIRE(ison[trueKey].rjIsTrue());
        REQUIRE_FALSE(ison[trueKey].rjIsFalse());
        REQUIRE(ison[trueStr1Key].rjIsString());
        REQUIRE(ison[trueStr2Key].rjIsString());
        REQUIRE(ison[trueStr3Key].rjIsString());
        REQUIRE(ison[falseKey].rjExists());
        REQUIRE_FALSE(ison[falseKey].rjEmpty());
        REQUIRE(ison[falseKey].rjIsBool());
        REQUIRE_FALSE(ison[falseKey].rjIsTrue());
        REQUIRE(ison[falseKey].rjIsFalse());
        REQUIRE(ison[falseStr1Key].rjIsString());
        REQUIRE(ison[falseStr2Key].rjIsString());
        REQUIRE(ison[falseStr3Key].rjIsString());
        REQUIRE(ison[zeroNumKey].rjIsNumber());
        REQUIRE(ison[zeroStrKey].rjIsString());
        REQUIRE(ison[fiveNumKey].rjIsNumber());
        REQUIRE(ison[sevenStrKey].rjIsString());
        REQUIRE(ison[pintNumKey].rjIsNumber());
        REQUIRE(ison[pintNumKey].rjIsInt());
        REQUIRE(ison[pintNumKey].rjIsUint());
        REQUIRE(ison[pintNumKey].rjIsInt64());
        REQUIRE(ison[pintNumKey].rjIsUint64());
        REQUIRE(ison[pintStrKey].rjIsString());
        REQUIRE(ison[nintNumKey].rjIsNumber());
        REQUIRE(ison[nintNumKey].rjIsInt());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint());
        REQUIRE(ison[nintNumKey].rjIsInt64());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint64());
        REQUIRE(ison[nintStrKey].rjIsString());
        REQUIRE(ison[pdblNumKey].rjIsNumber());
        REQUIRE(ison[pdblNumKey].rjIsDouble());
        REQUIRE(ison[pdblStrKey].rjIsString());
        REQUIRE(ison[ndblNumKey].rjIsNumber());
        REQUIRE(ison[ndblNumKey].rjIsDouble());
        REQUIRE(ison[ndblStrKey].rjIsString());
        REQUIRE(ison[strKey].rjExists());
        REQUIRE_FALSE(ison[strKey].rjEmpty());
        REQUIRE(ison[strKey].rjIsString());
        REQUIRE_FALSE(ison[subKey].rjExists());
        REQUIRE(ison[subKey].rjEmpty());
        REQUIRE_FALSE(ison[subKey].rjIsObject());
        REQUIRE_FALSE(ison[nepKey].rjExists());
        REQUIRE(ison[nepKey].rjEmpty());

        REQUIRE_NOTHROW(ison.clear());
        REQUIRE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 0);
    }

    TEST_CASE("Handle MBS JSON object wrapper's predicates #4", "[json][mbs]") {
        Json::Allocator allocator {};
        Json::Mbs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        Json::Object ison0 { jsonDoc };
        Json::Object ison { ison0, subKey };

        REQUIRE_FALSE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 25);

        REQUIRE(ison[nullKey].rjExists());
        REQUIRE(ison[nullKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjExists());
        REQUIRE(ison[emptyObjectKey].rjEmpty());
        REQUIRE(ison[emptyObjectKey].rjIsObject());
        REQUIRE_FALSE(ison[emptyObjectKey].rjIsArray());
        REQUIRE(ison[emptyArrayKey].rjExists());
        REQUIRE(ison[emptyArrayKey].rjEmpty());
        REQUIRE_FALSE(ison[emptyArrayKey].rjIsObject());
        REQUIRE(ison[emptyArrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjExists());
        REQUIRE_FALSE(ison[arrayKey].rjEmpty());
        REQUIRE(ison[arrayKey].rjIsArray());
        REQUIRE(ison[arrayKey].rjCount() == 5);
        REQUIRE(ison[trueKey].rjExists());
        REQUIRE_FALSE(ison[trueKey].rjEmpty());
        REQUIRE(ison[trueKey].rjIsBool());
        REQUIRE(ison[trueKey].rjIsTrue());
        REQUIRE_FALSE(ison[trueKey].rjIsFalse());
        REQUIRE(ison[trueStr1Key].rjIsString());
        REQUIRE(ison[trueStr2Key].rjIsString());
        REQUIRE(ison[trueStr3Key].rjIsString());
        REQUIRE(ison[falseKey].rjExists());
        REQUIRE_FALSE(ison[falseKey].rjEmpty());
        REQUIRE(ison[falseKey].rjIsBool());
        REQUIRE_FALSE(ison[falseKey].rjIsTrue());
        REQUIRE(ison[falseKey].rjIsFalse());
        REQUIRE(ison[falseStr1Key].rjIsString());
        REQUIRE(ison[falseStr2Key].rjIsString());
        REQUIRE(ison[falseStr3Key].rjIsString());
        REQUIRE(ison[zeroNumKey].rjIsNumber());
        REQUIRE(ison[zeroStrKey].rjIsString());
        REQUIRE(ison[fiveNumKey].rjIsNumber());
        REQUIRE(ison[sevenStrKey].rjIsString());
        REQUIRE(ison[pintNumKey].rjIsNumber());
        REQUIRE(ison[pintNumKey].rjIsInt());
        REQUIRE(ison[pintNumKey].rjIsUint());
        REQUIRE(ison[pintNumKey].rjIsInt64());
        REQUIRE(ison[pintNumKey].rjIsUint64());
        REQUIRE(ison[pintStrKey].rjIsString());
        REQUIRE(ison[nintNumKey].rjIsNumber());
        REQUIRE(ison[nintNumKey].rjIsInt());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint());
        REQUIRE(ison[nintNumKey].rjIsInt64());
        REQUIRE_FALSE(ison[nintNumKey].rjIsUint64());
        REQUIRE(ison[nintStrKey].rjIsString());
        REQUIRE(ison[pdblNumKey].rjIsNumber());
        REQUIRE(ison[pdblNumKey].rjIsDouble());
        REQUIRE(ison[pdblStrKey].rjIsString());
        REQUIRE(ison[ndblNumKey].rjIsNumber());
        REQUIRE(ison[ndblNumKey].rjIsDouble());
        REQUIRE(ison[ndblStrKey].rjIsString());
        REQUIRE(ison[strKey].rjExists());
        REQUIRE_FALSE(ison[strKey].rjEmpty());
        REQUIRE(ison[strKey].rjIsString());
        REQUIRE_FALSE(ison[subKey].rjExists());
        REQUIRE(ison[subKey].rjEmpty());
        REQUIRE_FALSE(ison[subKey].rjIsObject());
        REQUIRE_FALSE(ison[nepKey].rjExists());
        REQUIRE(ison[nepKey].rjEmpty());

        REQUIRE_NOTHROW(ison.clear());
        REQUIRE(ison.rjEmpty());
        REQUIRE(ison.rjCount() == 0);
    }
}
