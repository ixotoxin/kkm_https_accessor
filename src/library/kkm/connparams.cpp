// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "connparams.h"
#include "variables.h"
#include "strings.h"
#include "except.h"

namespace Kkm {
    using namespace std::string_view_literals;

    [[nodiscard]]
    FfdVersion BaseConnParams::storedFfdVersion() const {
        return m_ffdVersion;
    }

    ComConnParams::ComConnParams(const ConnParamVector & paramVector) : BaseConnParams() {
        if (paramVector.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams);
        }
        m_port.assign(paramVector[1]);
        if (paramVector.size() >= 3) {
            if (std::ranges::find(Wcs::c_allowedBaudRate, paramVector[2]) != Wcs::c_allowedBaudRate.end()) {
                m_baudRate.assign(paramVector[2]);
            } else {
                throw Failure(Wcs::c_invalidConnParams);
            }
        } else {
            m_baudRate.assign(s_defaultBaudRate);
        }
    }

    ComConnParams::ComConnParams(const ConnParamJson & paramJson) : BaseConnParams() {
        if (!paramJson.IsObject()) {
            throw Failure(Wcs::c_invalidConnParams);
        }
        if (!Json::handleKey(paramJson, L"port"_key, m_port, Text::Wcs::isInteger())) {
            throw Failure(Wcs::c_invalidConnParams);
        }
        if (!Json::handleKey(paramJson, L"baudRate"_key, m_baudRate, Wcs::c_allowedBaudRate)) {
            m_baudRate.assign(s_defaultBaudRate);
        }
        if (/*s_ffdVersionDetect == FfdVersionDetect::Once &&*/ paramJson.HasMember(L"ffdVersion"_key)) {
            m_ffdVersion
                = static_cast<FfdVersion>(Json::cast<std::underlying_type_t<FfdVersion>>(paramJson[kv(L"ffdVersion"_key)]));
            if (!Wcs::c_ffdVersions.contains(m_ffdVersion)) {
                throw Failure(Wcs::c_invalidConnParams);
            }
        }
    }

    void ComConnParams::apply(Atol::Fptr & kkm) const {
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_COM));
        std::wstring port { L"COM" };
        port.append(m_port);
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_COM_FILE, port);
        if (!m_baudRate.empty()) {
            kkm.setSingleSetting(Atol::LIBFPTR_SETTING_BAUDRATE, m_baudRate);
        }
    }

    ComConnParams::operator ConnParamString() const {
        std::wstring result { L"COM" };
        result.append(c_connParamsSeparator);
        result.append(m_port);
        return result;
    }

    ComConnParams::operator ConnParamJson() const {
        JsonDoc jsonResult { Json::Type::Object };
        JsonObj result { jsonResult };
        result[L"type"_key] <<= L"com"sv;
        result[L"port"_key] <<= m_port;
        result[L"baudRate"_key] <<= m_baudRate;
        result[L"ffdVersion"_key] <<= m_ffdVersion;
        return jsonResult;
    }

    UsbConnParams::UsbConnParams(const ConnParamVector &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    UsbConnParams::UsbConnParams(const ConnParamJson &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    void UsbConnParams::apply(Atol::Fptr &) const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    UsbConnParams::operator ConnParamString() const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    UsbConnParams::operator ConnParamJson() const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    TcpIpConnParams::TcpIpConnParams(const ConnParamVector &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    TcpIpConnParams::TcpIpConnParams(const ConnParamJson &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    void TcpIpConnParams::apply(Atol::Fptr &) const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    TcpIpConnParams::operator ConnParamString() const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    TcpIpConnParams::operator ConnParamJson() const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    BluetoothConnParams::BluetoothConnParams(const ConnParamVector &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    BluetoothConnParams::BluetoothConnParams(const ConnParamJson &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    void BluetoothConnParams::apply(Atol::Fptr &) const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    BluetoothConnParams::operator ConnParamString() const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }

    BluetoothConnParams::operator ConnParamJson() const {
        throw Failure(Wcs::c_notImplemented); // TODO: Реализовать.
    }
}
