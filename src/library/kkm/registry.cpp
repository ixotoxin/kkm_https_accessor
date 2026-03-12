// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "registry.h"
#include "strings.h"
#include <filesystem>
#include <fstream>

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
                    throw Failure(Wcs::c_registryError); // NOLINT(*-exception-baseclass)
                }
            }
        }
        path /= serialNumber + L".json"s;
        return path;
    }

    [[nodiscard]]
    const std::wstring & filterSerialNumber(const std::wstring & serialNumber) {
        if (serialNumber.empty() || std::string::npos != serialNumber.find_first_not_of(c_serialNumberChars)) {
            throw Failure(Wcs::c_invalidSerialNumber); // NOLINT(*-exception-baseclass)
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
        throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
    }

    [[nodiscard]]
    ConnParams make(const std::wstring & paramString) {
        ConnParamVector paramVector {};
        Text::splitTo(paramVector, paramString, c_connParamsSeparator);
        if (paramVector.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        return make(paramVector[0], paramVector);
    }

    [[nodiscard]]
    ConnParams load(const std::wstring & serialNumber) {
        return read(filePath(filterSerialNumber(serialNumber)), serialNumber);
    }

    [[nodiscard]]
    ConnParams read(const std::filesystem::path & path, const std::wstring & serialNumber) {
        const std::ifstream file { path };
        if (!file.is_open() || !file.good()) {
            throw Failure(KKM_WFMT(Wcs::c_loadingError, serialNumber)); // NOLINT(*-exception-baseclass)
        }
        const Nln::Json paramJson(Nln::Json::parse(std::ifstream(path)));
        if (paramJson.is_array()) {
            ConnParamVector paramVector {};
            Json::handle(paramJson, paramVector);
            if (paramVector.size() < 2) {
                throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
            }
            return make(paramVector[0], paramVector);
        }
        if (!paramJson.is_object()) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        if (!paramJson.contains("type")) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        return make(Json::cast<std::wstring>(paramJson["type"]), paramJson);
    }

    void save(const ConnParams & params, Device & kkm) {
        const auto & path = filePath(filterSerialNumber(kkm.serialNumber()), true);
        std::ofstream file { path };
        auto paramJson = static_cast<ConnParamJson>(*params);
        paramJson["serialNumber"] = Text::convert(kkm.serialNumber());
        paramJson["ffdVersion"] = kkm.ffdVersion(true);
        file << paramJson.dump();
        file.close();
        if (!std::filesystem::is_regular_file(path)) {
            throw Failure(KKM_WFMT(Wcs::c_savingError, kkm.serialNumber())); // NOLINT(*-exception-baseclass)
        }
    }
}
