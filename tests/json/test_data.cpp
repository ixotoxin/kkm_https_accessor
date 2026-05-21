// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "test_data_wcs.h"
#include "test_data_mbs.h"

namespace UnitTests {
    int intAdd10(const int v) {
        return v + 10;
    }

    int intThrow(int) {
        throw Basic::DataError { Basic::Wcs::c_invalidValue };
    }

    std::filesystem::path tempFile() {
        return std::filesystem::temp_directory_path() / (L"kkmha_"s + std::to_wstring(DateTime::windows()) + L".json"s);
    }

    namespace Wcs {
        std::wstring strAddTilda(const std::wstring & v) {
            return L"~"s + v + L"~"s;
        }

        std::wstring strThrow(const std::wstring &) {
            throw Basic::DataError { Basic::Wcs::c_invalidValue };
        }

        void prepareObject(Json::Wcs::Document & json) {
            auto & allocator = json.GetAllocator();

            if (!json.IsObject()) {
                json.SetObject();
            }

            json.AddMember(enpKey, Json::Wcs::Value { 42 }, allocator);
            json.AddMember(etpKey, Json::Wcs::Value { L"Forty two", allocator }, allocator);

            json.AddMember(
                cookieKey,
                Json::Wcs::Value { cookieVal.data(), static_cast<SizeType>(cookieVal.size()), allocator },
                allocator
            );

            json.AddMember(subKey, Json::Wcs::Value { Json::Type::Object }, allocator);

            json[kv(subKey)].AddMember(
                cookieKey,
                Json::Wcs::Value { cookieVal.data(), static_cast<SizeType>(cookieVal.size()), allocator },
                allocator
            );
        }

        std::wstring removeWhiteSpace(std::wstring str) {
            str.erase(std::ranges::remove_if(str, Meta::WcsText::trimPredicate).begin(), str.end());
            return str;
        }

        std::wstring removeTrailingZeros(std::wstring str) {
            if (!str.empty()) {
                if (const auto pointPos = str.find('.'); pointPos != std::wstring::npos) {
                    if (const size_t lastNonZeroPos = str.find_last_not_of('0'); lastNonZeroPos != std::wstring::npos) {
                        str.resize(pointPos == lastNonZeroPos ? pointPos : lastNonZeroPos + 1);
            } } }
            return str;
        }
    }

    namespace Mbs {
        std::string strAddTilda(const std::string & v) {
            return "~"s + v + "~"s;
        }

        std::string strThrow(const std::string &) {
            throw Basic::DataError { Basic::Wcs::c_invalidValue };
        }

        void prepareObject(Json::Mbs::Document & json) {
            auto & allocator = json.GetAllocator();

            if (!json.IsObject()) {
                json.SetObject();
            }

            json.AddMember(enpKey, Json::Mbs::Value { 42 }, allocator);
            json.AddMember(etpKey, Json::Mbs::Value { "Forty two", allocator }, allocator);

            json.AddMember(
                cookieKey,
                Json::Mbs::Value { cookieVal.data(), static_cast<SizeType>(cookieVal.size()), allocator },
                allocator
            );

            json.AddMember(subKey, Json::Mbs::Value { Json::Type::Object }, allocator);

            json[kv(subKey)].AddMember(
                cookieKey,
                Json::Mbs::Value { cookieVal.data(), static_cast<SizeType>(cookieVal.size()), allocator },
                allocator
            );
        }

        std::string removeWhiteSpace(std::string str) {
            str.erase(std::ranges::remove_if(str, Meta::MbsText::trimPredicate).begin(), str.end());
            return str;
        }

        std::string removeTrailingZeros(std::string str) {
            if (!str.empty()) {
                if (const auto pointPos = str.find('.'); pointPos != std::wstring::npos) {
                    if (const size_t lastNonZeroPos = str.find_last_not_of('0'); lastNonZeroPos != std::wstring::npos) {
                        str.resize(pointPos == lastNonZeroPos ? pointPos : lastNonZeroPos + 1);
                    } } }
            return str;
        }
    }
}
