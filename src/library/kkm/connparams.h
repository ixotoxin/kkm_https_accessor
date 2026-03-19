// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <lib/json.h>
#include <memory>
#include <string>
#include <vector>

namespace Kkm {
    using ConnParamString = std::wstring;
    using ConnParamVector = std::vector<std::wstring>;
    using ConnParamJson = Nln::Json;

    class BaseConnParams {
    public:
        BaseConnParams() = default;
        BaseConnParams(const BaseConnParams &) = default;
        BaseConnParams(BaseConnParams &&) noexcept = default;
        virtual ~BaseConnParams() = default;

        BaseConnParams & operator=(const BaseConnParams &) = default;
        BaseConnParams & operator=(BaseConnParams &&) noexcept = default;

        virtual void apply(Atol::Fptr &) const = 0;
        [[nodiscard]] FfdVersion storedFfdVersion() const;
        virtual explicit operator ConnParamString() const = 0;
        virtual explicit operator ConnParamJson() const = 0;

    protected:
        FfdVersion m_ffdVersion { FfdVersion::Unknown };
    };

    using ConnParams = std::shared_ptr<BaseConnParams>;

    class ComConnParams final : public BaseConnParams {
    public:
        explicit ComConnParams(const ConnParamVector &);
        explicit ComConnParams(const ConnParamJson &);
        ComConnParams(const ComConnParams &) = default;
        ComConnParams(ComConnParams &&) noexcept = default;
        ~ComConnParams() override = default;

        ComConnParams & operator=(const ComConnParams &) = default;
        ComConnParams & operator=(ComConnParams &&) noexcept = default;

        void apply(Atol::Fptr &) const override;
        explicit operator ConnParamString() const override;
        explicit operator ConnParamJson() const override;

    protected:
        std::wstring m_port {};
        std::wstring m_baudRate {};

        ComConnParams() = default;
    };

    class UsbConnParams final : public BaseConnParams {
    public:
        explicit UsbConnParams(const ConnParamVector &);
        explicit UsbConnParams(const ConnParamJson &);
        UsbConnParams(const UsbConnParams &) = default;
        UsbConnParams(UsbConnParams &&) noexcept = default;
        ~UsbConnParams() override = default;

        UsbConnParams & operator=(const UsbConnParams &) = default;
        UsbConnParams & operator=(UsbConnParams &&) noexcept = default;

        void apply(Atol::Fptr &) const override;
        explicit operator ConnParamString() const override;
        explicit operator ConnParamJson() const override;

    protected:
        UsbConnParams() = default;
    };

    class TcpIpConnParams final : public BaseConnParams {
    public:
        explicit TcpIpConnParams(const ConnParamVector &);
        explicit TcpIpConnParams(const ConnParamJson &);
        TcpIpConnParams(const TcpIpConnParams &) = default;
        TcpIpConnParams(TcpIpConnParams &&) noexcept = default;
        ~TcpIpConnParams() override = default;

        TcpIpConnParams & operator=(const TcpIpConnParams &) = default;
        TcpIpConnParams & operator=(TcpIpConnParams &&) noexcept = default;

        void apply(Atol::Fptr &) const override;
        explicit operator ConnParamString() const override;
        explicit operator ConnParamJson() const override;

    protected:
        TcpIpConnParams() = default;
    };

    class BluetoothConnParams final : public BaseConnParams {
    public:
        explicit BluetoothConnParams(const ConnParamVector &);
        explicit BluetoothConnParams(const ConnParamJson &);
        BluetoothConnParams(const BluetoothConnParams &) = default;
        BluetoothConnParams(BluetoothConnParams &&) noexcept = default;
        ~BluetoothConnParams() override = default;

        BluetoothConnParams & operator=(const BluetoothConnParams &) = default;
        BluetoothConnParams & operator=(BluetoothConnParams &&) noexcept = default;

        void apply(Atol::Fptr &) const override;
        explicit operator ConnParamString() const override;
        explicit operator ConnParamJson() const override;

    protected:
        BluetoothConnParams() = default;
    };
}
