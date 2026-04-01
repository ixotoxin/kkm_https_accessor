// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "connparams.h"
#include "variables.h"
#include "strings.h"
#include "except.h"

namespace Kkm {
    [[nodiscard]]
    FfdVersion BaseConnParams::storedFfdVersion() const {
        return m_ffdVersion;
    }

    ComConnParams::ComConnParams(const ConnParamVector & paramVector) : BaseConnParams() {
        if (paramVector.size() < 2) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        m_port.assign(paramVector[1]);
        if (paramVector.size() >= 3) {
            if (std::ranges::find(Wcs::c_allowedBaudRate, paramVector[2]) != Wcs::c_allowedBaudRate.end()) {
                m_baudRate.assign(paramVector[2]);
            } else {
                throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
            }
        } else {
            m_baudRate.assign(s_defaultBaudRate);
        }
    }

    ComConnParams::ComConnParams(const ConnParamJson & paramJson) : BaseConnParams() {
        if (!paramJson.is_object()) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        if (!paramJson.contains("port")) {
            throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
        }
        m_port.assign(std::to_wstring(Json::cast<unsigned>(paramJson["port"])));
        if (paramJson.contains("baudRate")) {
            auto baudRate = std::to_wstring(Json::cast<unsigned>(paramJson["baudRate"]));
            if (std::ranges::find(Wcs::c_allowedBaudRate, baudRate) != Wcs::c_allowedBaudRate.end()) {
                m_baudRate.assign(std::move(baudRate));
            } else {
                throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
            }
        } else {
            m_baudRate.assign(s_defaultBaudRate);
        }
        if (/*s_ffdVersionDetect == FfdVersionDetect::Once &&*/ paramJson.contains("ffdVersion")) {
            m_ffdVersion
                = static_cast<FfdVersion>(Json::cast<std::underlying_type_t<FfdVersion>>(paramJson["ffdVersion"]));
            if (!Mbs::c_ffdVersions.contains(m_ffdVersion)) {
                throw Failure(Wcs::c_invalidConnParams); // NOLINT(*-exception-baseclass)
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
        return {
            { "type", "com" },
            { "port", Text::convert(m_port) },
            { "baudRate", Text::convert(m_baudRate) },
            { "ffdVersion", Meta::toUnderlying(m_ffdVersion) }
        };
    }

    UsbConnParams::UsbConnParams(const ConnParamVector &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    UsbConnParams::UsbConnParams(const ConnParamJson &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    void UsbConnParams::apply(Atol::Fptr &) const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    UsbConnParams::operator ConnParamString() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    UsbConnParams::operator ConnParamJson() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    TcpIpConnParams::TcpIpConnParams(const ConnParamVector &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    TcpIpConnParams::TcpIpConnParams(const ConnParamJson &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    void TcpIpConnParams::apply(Atol::Fptr &) const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    TcpIpConnParams::operator ConnParamString() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    TcpIpConnParams::operator ConnParamJson() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    BluetoothConnParams::BluetoothConnParams(const ConnParamVector &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    BluetoothConnParams::BluetoothConnParams(const ConnParamJson &) : BaseConnParams() {
        // TODO: Реализовать.
    }

    void BluetoothConnParams::apply(Atol::Fptr &) const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    BluetoothConnParams::operator ConnParamString() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    BluetoothConnParams::operator ConnParamJson() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }
}
