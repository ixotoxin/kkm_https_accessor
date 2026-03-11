// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "connparams.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"

namespace Kkm {
    void BaseConnParams::applyCommon(Atol::Fptr & kkm) {
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_MODEL, std::to_wstring(Atol::LIBFPTR_MODEL_ATOL_AUTO));
        if (s_timeZoneConfigured) {
            kkm.setSingleSetting(Atol::LIBFPTR_SETTING_TIME_ZONE, std::to_wstring(Meta::toUnderlying(s_timeZone)));
        }
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_OFD_CHANNEL, std::to_wstring(Atol::LIBFPTR_OFD_CHANNEL_AUTO));
        // ISSUE: Из документации не ясно, что передавать в качестве значения параметра.
        //  Но нам этот параметр не очень нужен, потому как при формировании чека единицы у нас
        //  всегда передаются и проблем с ФФД 1.2+ не должно возникнуть.
        // kkm.setSingleSetting(Atol::LIBFPTR_SETTING_AUTO_MEASUREMENT_UNIT, ???);
    }

    void BaseConnParams::apply(Atol::Fptr & kkm) const {
        this->applyDetail(kkm);
        applyCommon(kkm);
        if (kkm.applySingleSettings() < 0) {
            throw Failure(kkm); // NOLINT(*-exception-baseclass)
        }
    }

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

    void ComConnParams::applyDetail(Atol::Fptr & kkm) const {
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_PORT, std::to_wstring(Atol::LIBFPTR_PORT_COM));
        std::wstring port { L"COM" };
        port.append(m_port);
        kkm.setSingleSetting(Atol::LIBFPTR_SETTING_COM_FILE, port);
        if (!m_baudRate.empty()) {
            kkm.setSingleSetting(Atol::LIBFPTR_SETTING_BAUDRATE, m_baudRate);
        }
    }

    ComConnParams::operator ConnParamString() const {
        return L"COM"s + m_port;
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

    void UsbConnParams::applyDetail(Atol::Fptr &) const {
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

    void TcpIpConnParams::applyDetail(Atol::Fptr &) const {
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

    void BluetoothConnParams::applyDetail(Atol::Fptr &) const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    BluetoothConnParams::operator ConnParamString() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }

    BluetoothConnParams::operator ConnParamJson() const {
        throw Failure(Wcs::c_notImplemented); // NOLINT(*-exception-baseclass) // TODO: Реализовать.
    }
}
