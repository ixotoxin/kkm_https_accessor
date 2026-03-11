// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include "connparams.h"
#include "callparams.h"

namespace Kkm {
    class Device {
    public:
        Device() = delete;
        Device(const Device &) = delete;
        Device(Device &&) = delete;
        explicit Device(ConnParams, std::wstring_view = {});
        ~Device();

        Device & operator=(const Device &) = delete;
        Device & operator=(Device &&) = delete;

        [[nodiscard]] const std::wstring & serialNumber() const;
        [[nodiscard]] FfdVersion ffdVersion(bool = false);

        void getStatus(StatusResult &);
        void getShiftState(ShiftStateResult &);
        void getReceiptState(ReceiptStateResult &);
        void getCashStat(CashStatResult &);
        void getFndtOfdExchangeStatus(FndtOfdExchangeStatusResult &);
        void getFndtFnInfo(FndtFnInfoResult &);
        void getFndtRegistrationInfo(FndtRegistrationInfoResult &);
        void getFndtLastRegistration(FndtLastRegistrationResult &);
        void getFndtLastReceipt(FndtLastReceiptResult &);
        void getFndtLastDocument(FndtLastDocumentResult &);
        void getFndtErrors(FndtErrorsResult &);
        void getFfdVersions(FfdVersionsResult &);
        void getFwVersions(FwVersionsResult &);
        void printHello();
        void printDemo(Result &);
        void printNonFiscalDocument(const PrintDetails &, Result &);
        void printInfo(Result &);
        void printFnRegistrations(Result &);
        void printOfdExchangeStatus(Result &);
        void printOfdTest(Result &);
        void printCloseShiftReports(Result &);
        void printLastDocument(Result &);
        void registerCashIn(const CashDetails &, Result &);
        void registerCashOut(const CashDetails &, Result &);
        void registerSell(const ReceiptDetails &, Result &);
        void registerSellReturn(const ReceiptDetails &, Result &);
        void reportX(const CloseDetails &, Result &);
        void closeShift(const CloseDetails &, Result &);
        void resetState(const CloseDetails &, Result &);

    protected:
        explicit Device(std::wstring_view);

        void connect(ConnParams);

    private:
        Atol::Fptr m_kkm {};
        FfdVersionsResult m_ffdVersions {};
        std::wstring m_serialNumber {};
        std::wstring m_logPrefix;
        unsigned int m_lineLength { 0 };
        FfdVersion m_storedFfdVersion { FfdVersion::Unknown };
        bool m_needToCancelReceipt { false };

        void fail(Result &, std::wstring_view, const SrcLoc::Point & = SrcLoc::Point::current());
        void fail(Result &, const std::wstring &, const SrcLoc::Point & = SrcLoc::Point::current());
        void fail(Result &, std::wstring &&, const SrcLoc::Point & = SrcLoc::Point::current());
        void fail(Result &, const SrcLoc::Point & = SrcLoc::Point::current());

        void detectFfdVersions();

        [[nodiscard, maybe_unused]] static std::wstring addMargins(std::wstring_view, int = 1, int = -1);
        [[maybe_unused]] void addSeparator(std::wstring &, int = 0, int = -1) const;
        [[nodiscard, maybe_unused]] std::wstring addSeparators(std::wstring_view, int = 0, int = -1) const;
        [[maybe_unused]] void subPrintSeparator(int = 0, int = -1);

        void subPrintText(
            std::wstring_view, bool = false, bool = false, bool = false,
            int = 1, int = 1, TextPosition = TextPosition::Auto
        );

        void subPrintText(const PrintableText &, TextPosition = TextPosition::Auto);
        void subCheckDocumentClosed(Result &);
        void subSetOperator(const OperatorDetails &);
        void subSetCustomer(const ReceiptDetails &);
        void subSetSeller(const ReceiptDetails &);
        void subRegisterReceipt(ReceiptType, const ReceiptDetails &, Result &);
        void subCashOut(const OperatorDetails &, Result &);
        void subCloseShift(const OperatorDetails &, Result &);
    };

    class NewDevice : public Device {
    public:
        NewDevice() = delete;
        NewDevice(const NewDevice &) = delete;
        NewDevice(NewDevice &&) = delete;
        explicit NewDevice(ConnParams, std::wstring_view = {});
        ~NewDevice() = default;

        NewDevice & operator=(const NewDevice &) = delete;
        NewDevice & operator=(NewDevice &&) = delete;
    };
}
