// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "registry.h"
#include "strings.h"
#include "except.h"
#include "json.h"
#include <filesystem>

namespace Kkm::Registry {
    [[nodiscard]]
    std::wstring serialNumber(const std::filesystem::path & filePath) {
        return filePath.stem().wstring();
    }

    [[nodiscard]]
    std::filesystem::path filePath(const std::wstring & serialNumber, const bool touchDir) {
        std::filesystem::path path { s_dbDirectory };
        if (touchDir) {
            if (!std::filesystem::is_directory(path)) {
                std::filesystem::create_directories(path);
                if (!std::filesystem::is_directory(path)) {
                    throw Failure(Wcs::c_registryError);
                }
            }
        }
        path /= serialNumber + L".json"s;
        return path;
    }

    [[nodiscard]]
    const std::wstring & filterSerialNumber(const std::wstring & serialNumber) {
        if (serialNumber.empty() || std::string::npos != serialNumber.find_first_not_of(c_serialNumberChars)) {
            throw Failure(Wcs::c_invalidSerialNumber);
        }
        return serialNumber;
    }

    template<class T>
    requires std::is_same_v<T, ConnParamVector> || std::is_same_v<T, ConnParamJson>
    [[nodiscard]]
    ConnParams make(std::wstring type, const T & params) {
        Text::lower(type);
        if (type == L"com"s) {
            return std::make_shared<ComConnParams>(params);
        }
        if (type == L"usb"s) {
            return std::make_shared<UsbConnParams>(params);
        }
        if (type == L"tcpip"s || type == L"ip"s) {
            return std::make_shared<TcpIpConnParams>(params);
        }
        if (type == L"bluetooth"s || type == L"bt"s) {
            return std::make_shared<BluetoothConnParams>(params);
        }
        throw Failure(Wcs::c_invalidConnParams);
    }

    [[nodiscard]]
    ConnParams make(const std::wstring & paramString) {
        ConnParamVector paramVector {};
        Text::splitTo(paramVector, paramString, c_connParamsSeparator);
        if (paramVector.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams);
        }
        return make(paramVector[0], paramVector);
    }

    [[nodiscard]]
    ConnParams load(const std::wstring & serialNumber) {
        return read(filePath(filterSerialNumber(serialNumber)));
    }

    [[nodiscard]]
    ConnParams read(const std::filesystem::path & path) {
        JsonDoc paramJson { Json::Type::Object };
        paramJson <<= path;

        if (paramJson.IsArray()) {
            ConnParamVector paramVector {};
            Json::handle(paramJson, paramVector);
            if (paramVector.size() < 2) {
                throw Failure(Wcs::c_invalidConnParams);
            }
            return make(paramVector[0], paramVector);
        }
        if (!paramJson.IsObject()) {
            throw Failure(Wcs::c_invalidConnParams);
        }
        if (!paramJson.HasMember(L"type"_key)) {
            throw Failure(Wcs::c_invalidConnParams);
        }
        return make(Json::cast<std::wstring>(paramJson[kv(L"type"_key)]), paramJson);
    }

    void save(const ConnParams & params, Device & kkm) {
        const auto serialNumber = kkm.serialNumber();
        const auto path = filePath(filterSerialNumber(serialNumber), true);
        auto paramJson = static_cast<ConnParamJson>(*params);
        JsonObj paramObj { paramJson };
        paramObj[L"serialNumber"_key] <<= serialNumber;
        paramObj[L"ffdVersion"_key] <<= kkm.ffdVersion(true);
        paramJson >>= path;
        if (!std::filesystem::is_regular_file(path)) {
            throw Failure(Fmt<c_sStrSize>(Wcs::c_savingError, serialNumber));
        }
    }
}
