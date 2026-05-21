// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include <json/xetters.h>
#include <json/inout.h>
#include <catch2/catch_test_macros.hpp>

namespace UnitTests::Wcs {
    TEST_CASE("Obtain a reference to an array within the WCS JSON object", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(jsonDoc.HasMember(arrayKey));
        REQUIRE(jsonDoc[kv(arrayKey)].IsArray());
        REQUIRE(jsonDoc[kv(arrayKey)].Size() == 5);
        REQUIRE(jsonDoc.HasMember(subKey));
        REQUIRE(jsonDoc[kv(subKey)].HasMember(arrayKey));
        REQUIRE(jsonDoc[kv(subKey)][kv(arrayKey)].IsArray());
        REQUIRE(jsonDoc[kv(subKey)][kv(arrayKey)].Size() == 5);
        REQUIRE(jsonDoc.HasMember(zeroNumKey));
        REQUIRE_FALSE(jsonDoc[kv(zeroNumKey)].IsArray());
        REQUIRE_FALSE(jsonDoc.HasMember(nepKey));

        try {
            {
                auto & jsonArray { Json::requireArray(jsonDoc[kv(arrayKey)]) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
            {
                auto & jsonArray { Json::requireArray(jsonDoc[kv(arrayKey)], 2, 7) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
            {
                auto & jsonArray { Json::requireArray(jsonDoc[kv(subKey)][kv(arrayKey)]) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
            {
                auto & jsonArray { Json::requireArray(jsonDoc[kv(subKey)][kv(arrayKey)], 2, 7) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
        } catch (...) {
            FAIL("Json::requireArray() threw an exception");
        }

        try {
            {
                auto & jsonArray { Json::requireArray(jsonDoc, arrayKey) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
            {
                auto & jsonArray { Json::requireArray(jsonDoc, arrayKey, 2, 7) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
            {
                auto & jsonArray { Json::requireArray(jsonDoc[kv(subKey)], arrayKey) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
            {
                auto & jsonArray { Json::requireArray(jsonDoc[kv(subKey)], arrayKey, 2, 7) };
                REQUIRE(jsonArray.IsArray());
                REQUIRE(jsonArray.Size() == 5);
                for (SizeType i {}; i < 5; ++i) {
                    REQUIRE(jsonArray[i].IsInt());
                    REQUIRE(jsonArray[i].GetInt() == static_cast<int>(i + 1));
                }
            }
        } catch (...) {
            FAIL("Json::requireArray() threw an exception");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc[kv(zeroNumKey)]) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc[kv(arrayKey)], 7) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc[kv(arrayKey)], 0, 3) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc[kv(arrayKey)], 7, 10) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc, zeroNumKey) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc, nepKey) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc, arrayKey, 7) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc, arrayKey, 0, 3) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireArray(jsonDoc, arrayKey, 7, 10) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }
    }

    TEST_CASE("Obtain a reference to an object within the WCS JSON object", "[json][wcs]") {
        Json::Allocator allocator {};
        Json::Wcs::Document jsonDoc { &allocator };

        REQUIRE_NOTHROW(jsonDoc <<= jsonObjStr);

        REQUIRE(jsonDoc.HasMember(emptyObjectKey));
        REQUIRE(jsonDoc.HasMember(subKey));
        REQUIRE(jsonDoc[kv(subKey)].HasMember(emptyObjectKey));
        REQUIRE(jsonDoc.HasMember(zeroNumKey));
        REQUIRE_FALSE(jsonDoc[kv(zeroNumKey)].IsObject());
        REQUIRE_FALSE(jsonDoc.HasMember(nepKey));

        try {
            {
                auto & jsonObject { Json::requireObject(jsonDoc[kv(emptyObjectKey)]) };
                REQUIRE(jsonObject.IsObject());
                REQUIRE(jsonObject.ObjectEmpty());
            }
            {
                auto & jsonObject { Json::requireObject(jsonDoc[kv(subKey)][kv(emptyObjectKey)]) };
                REQUIRE(jsonObject.IsObject());
                REQUIRE(jsonObject.ObjectEmpty());
            }
        } catch (...) {
            FAIL("Json::requireObject() threw an exception");
        }

        try {
            {
                auto & jsonObject { Json::requireObject(jsonDoc, emptyObjectKey) };
                REQUIRE(jsonObject.IsObject());
                REQUIRE(jsonObject.ObjectEmpty());
            }
            {
                auto & jsonObject { Json::requireObject(jsonDoc[kv(subKey)], emptyObjectKey) };
                REQUIRE(jsonObject.IsObject());
                REQUIRE(jsonObject.ObjectEmpty());
            }
        } catch (...) {
            FAIL("Json::requireObject() threw an exception");
        }

        try {
            auto & jsonObject [[maybe_unused]] { Json::requireObject(jsonDoc[kv(zeroNumKey)]) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonObject [[maybe_unused]] { Json::requireObject(jsonDoc, zeroNumKey) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }

        try {
            auto & jsonArray [[maybe_unused]] { Json::requireObject(jsonDoc, nepKey) };
            FAIL("Expected an exception");
        } catch (...) {
            SUCCEED("Exception was thrown as expected");
        }
    }
}
