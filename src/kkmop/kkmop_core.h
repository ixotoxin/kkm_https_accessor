// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "kkmop_strings.h"
#include <main/shortcut.h>
#include <kkm/variables.h>
#include <kkm/strings.h>
#include <kkm/logger.h>
#include <kkm/device.h>
#include <kkm/registry.h>
#include <kkm/callhelpers.h>
#include <cassert>
#include <cstdlib>
#include <optional>

namespace KkmOperator {
    using namespace Kkm;

    template<std::derived_from<Result> R, typename T>
    void logResult(R & result, const std::wstring_view name, T && value) noexcept {
        if (result.m_success) {
            log(Log::Level::Info, name, std::forward<T>(value));
        } else {
            log(Log::Level::Error, name, Wcs::c_error);
        }
    }

    [[nodiscard]]
    inline int learn(const int connParamCount, wchar_t ** connParamItems) {
        assert(connParamCount > 0);
        Log::Console::ScopedSolo solo {};
        Log::Console::ScopedLevelDown scopeLevel { Log::Level::Info };

        for (int i = 0, n = 1; i < connParamCount; ++i, ++n) {
            Log::CategoryLogger genLogger {
                Log::Category::Generic,
                Basic::Wcs::Fmt<c_xsStrSize>(Wcs::c_commandPrefix, n)
            };
            try {
                auto kkmLogger { std::make_shared<Logger>(genLogger) };
                const auto connParams = Registry::make(connParamItems[i]);
                NewDevice kkm { connParams, kkmLogger };
                std::wstring serialNumber { kkm.serialNumber() };
                kkmLogger->debug(Wcs::c_getKkmInfo);
                Registry::save(connParams, kkm);
                kkm.printHello();
                genLogger.info(Wcs::c_connParamsSaved, serialNumber);
            } catch (const Basic::Failure & e) {
                genLogger.warning(e);
            }
        }

        return EXIT_SUCCESS;
    }

    template<class R, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, UndetailedMethod<R> method) {
        Log::Console::ScopedSolo solo {};
        Log::Console::ScopedLevelDown scopeLevel { Log::Level::Info };
        R result {};
        (kkm.*method)(result);
        if (result.m_success) {
            log(Log::Level::Info, L"Done");
        } else {
            throw Basic::Failure(result.m_message); // NOLINT(*-exception-baseclass)
        }
    }

    template<class R, class D, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, DetailedMethod<R, D> method, D && details) {
        Log::Console::ScopedSolo solo {};
        Log::Console::ScopedLevelDown scopeLevel { Log::Level::Info };
        R result {};
        (kkm.*method)(details, result);
        if (result.m_success) {
            log(Log::Level::Info, L"Done");
        } else {
            throw Basic::Failure(result.m_message); // NOLINT(*-exception-baseclass)
        }
    }

    [[nodiscard]]
    inline std::optional<int> exec(const std::wstring & command, wchar_t * serialNumber) {
        std::wstring serial { serialNumber };

        if (KKM_CMD_EQ(command, L"status", L"v")) {
            Device kkm { Registry::load(serial) };

            StatusResult status {};
            kkm.getStatus(status);

            ShiftStateResult shift {};
            kkm.getShiftState(shift);

            CashStatResult cash {};
            kkm.getCashStat(cash);

            FndtOfdExchangeStatusResult exchange {};
            kkm.getFndtOfdExchangeStatus(exchange);

            FndtErrorsResult errors {};
            kkm.getFndtErrors(errors);

            FfdVersionsResult ffdVers {};
            kkm.getFfdVersions(ffdVers);

            FwVersionsResult fwVers {};
            kkm.getFwVersions(fwVers);

            {
                Log::Console::ScopedSolo solo {};
                Log::Console::ScopedLevelDown scopeLevel { Log::Level::Info };

                // KKM_LOG(status, Wcs::c_fmtModel, wcsSafeGet(Mbs::c_models, status.m_model));
                logResult(status, Wcs::c_fmtModel, status.m_modelName);
                logResult(status, Wcs::c_fmtSerialNumber, status.m_serialNumber);
                logResult(status, Wcs::c_fmtBlocked, Text::Wcs::daNet(status.m_blocked));
                logResult(status, Wcs::c_fmtLogicalNumber, status.m_logicalNumber);
                logResult(status, Wcs::c_fmtDateTime, DateTime::cast<std::wstring>(status.m_dateTime));
                logResult(status, Wcs::c_fmtFnPresent, Text::Wcs::daNet(status.m_fnPresent));
                logResult(status, Wcs::c_fmtFnFiscal, Text::Wcs::daNet(status.m_fnFiscal));
                logResult(status, Wcs::c_fmtInvalidFn, Text::Wcs::daNet(status.m_invalidFn));
                logResult(status, Wcs::c_fmtFiscal, Text::Wcs::daNet(status.m_fiscal));
                logResult(status, Wcs::c_fmtCashDrawerOpened, Text::Wcs::daNet(status.m_cashDrawerOpened));
                logResult(status, Wcs::c_fmtCoverOpened, Text::Wcs::daNet(status.m_coverOpened));
                logResult(status, Wcs::c_fmtReceiptPaperPresent, Text::Wcs::daNet(status.m_receiptPaperPresent));
                logResult(status, Wcs::c_fmtPaperNearEnd, Text::Wcs::daNet(status.m_paperNearEnd));
                logResult(status, Wcs::c_fmtCutError, Text::Wcs::daNet(status.m_cutError));
                logResult(status, Wcs::c_fmtPrinterOverheat, Text::Wcs::daNet(status.m_printerOverheat));
                logResult(status, Wcs::c_fmtReceiptLineLength, status.m_receiptLineLength);
                logResult(status, Wcs::c_fmtReceiptLineLengthPix, status.m_receiptLineLengthPix);
                logResult(shift, Wcs::c_fmtShiftState, wcsSafeGet(Mbs::c_shiftStateLabels, shift.m_shiftState));
                logResult(shift, Wcs::c_fmtShiftExpiration, DateTime::cast<std::wstring>(shift.m_expirationDateTime));
                logResult(status, Wcs::c_fmtReceiptType, wcsSafeGet(Mbs::c_receiptTypeLabels, status.m_receiptType));
                logResult(status, Wcs::c_fmtDocumentType, wcsSafeGet(Mbs::c_documentTypeLabels, status.m_documentType));

                logResult(cash, Wcs::c_fmtCashInCount, cash.m_cashInCount);
                logResult(cash, Wcs::c_fmtCashInSum, cash.m_cashInSum);
                logResult(cash, Wcs::c_fmtCashOutCount, cash.m_cashOutCount);
                logResult(cash, Wcs::c_fmtCashOutSum, cash.m_cashOutSum);
                logResult(cash, Wcs::c_fmtSellCashSum, cash.m_sellCashSum);
                logResult(cash, Wcs::c_fmtSellReturnCashSum, cash.m_sellReturnCashSum);
                logResult(cash, Wcs::c_fmtCashSum, cash.m_cashSum);

                if (exchange.m_success) {
                    log(Log::Level::Info, Wcs::c_ofdExchangeStatus);
                    log(Log::Level::Info, Wcs::c_fmtOfdExSBit0, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'0001));
                    log(Log::Level::Info, Wcs::c_fmtOfdExSBit1, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'0010));
                    Kkm::log(Log::Level::Info, Wcs::c_fmtOfdExSBit2, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'0100));
                    Kkm::log(Log::Level::Info, Wcs::c_fmtOfdExSBit3, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'1000));
                    Kkm::log(Log::Level::Info, Wcs::c_fmtOfdExSBit4, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0001'0000));
                    Kkm::log(Log::Level::Info, Wcs::c_fmtOfdExSBit5, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0010'0000));
                } else {
                    Kkm::log(Log::Level::Error, Wcs::c_ofdExchangeError);
                }
                logResult(exchange, Wcs::c_fmtUnsentCount, exchange.m_unsentCount);
                logResult(exchange, Wcs::c_fmtFirstUnsentNumber, exchange.m_firstUnsentNumber);
                logResult(
                    exchange,
                    Wcs::c_fmtFirstUnsentDateTime,
                    DateTime::cast<std::wstring>(exchange.m_firstUnsentDateTime)
                );
                logResult(exchange, Wcs::c_fmtOkpDateTime, DateTime::cast<std::wstring>(exchange.m_okpDateTime));
                logResult(exchange, Wcs::c_fmtOfdMessageRead, Text::Wcs::daNet(exchange.m_ofdMessageRead));
                logResult(exchange, Wcs::c_fmtLastSentDateTime, DateTime::cast<std::wstring>(exchange.m_lastSentDateTime));

                logResult(errors, Wcs::c_fmtSuccessDateTime, DateTime::cast<std::wstring>(errors.m_successDateTime));
                logResult(errors, Wcs::c_fmtNetworkError, errors.m_networkError);
                logResult(errors, Wcs::c_fmtNetworkErrorText, errors.m_networkErrorText);
                logResult(errors, Wcs::c_fmtOfdError, errors.m_ofdError);
                logResult(errors, Wcs::c_fmtOfdErrorText, errors.m_ofdErrorText);
                logResult(errors, Wcs::c_fmtFnError, errors.m_fnError);
                logResult(errors, Wcs::c_fmtFnErrorText, errors.m_fnErrorText);
                logResult(errors, Wcs::c_fmtDocumentNumber, errors.m_documentNumber);
                logResult(errors, Wcs::c_fmtCommandCode, errors.m_commandCode);
                logResult(errors, Wcs::c_fmtDataForSendIsEmpty, Text::Wcs::daNet(errors.m_dataForSendIsEmpty));

                logResult(ffdVers, Wcs::c_fmtDeviceFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_deviceFfdVersion));
                logResult(ffdVers, Wcs::c_fmtDevMinFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_devMinFfdVersion));
                logResult(ffdVers, Wcs::c_fmtDevMaxFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_devMaxFfdVersion));
                logResult(ffdVers, Wcs::c_fmtFnFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_fnFfdVersion));
                logResult(ffdVers, Wcs::c_fmtFnMaxFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_fnMaxFfdVersion));
                logResult(ffdVers, Wcs::c_fmtFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_ffdVersion));

                logResult(fwVers, Wcs::c_fmtFirmwareVersion, fwVers.m_firmwareVersion);
                logResult(fwVers, Wcs::c_fmtConfigurationVersion, fwVers.m_configurationVersion);
                logResult(fwVers, Wcs::c_fmtReleaseVersion, fwVers.m_releaseVersion);
                logResult(fwVers, Wcs::c_fmtTemplatesVersion, fwVers.m_templatesVersion);
                logResult(fwVers, Wcs::c_fmtControlUnitVersion, fwVers.m_controlUnitVersion);
                logResult(fwVers, Wcs::c_fmtBootVersion, fwVers.m_bootVersion);
            }
        } else if (KKM_CMD_EQ(command, L"demo-print", L"d")) {
            callMethod(Device { Registry::load(serial) }, &Device::printDemo);
        } else if (KKM_CMD_EQ(command, L"ofd-test", L"t")) {
            callMethod(Device { Registry::load(serial) }, &Device::printOfdTest);
        } else if (KKM_CMD_EQ(command, L"shift-reports", L"j")) {
            callMethod(Device { Registry::load(serial) }, &Device::printCloseShiftReports);
        } else if (KKM_CMD_EQ(command, L"last-document", L"m")) {
            callMethod(Device { Registry::load(serial) }, &Device::printLastDocument);
        } else if (KKM_CMD_EQ(command, L"report-x", L"p")) {
            callMethod(
                Device { Registry::load(serial) },
                &Device::reportX,
                { s_cliOperatorName, s_cliOperatorInn, false, false }
            );
        } else if (KKM_CMD_EQ(command, L"close-shift", L"s")) {
            callMethod(
                Device { Registry::load(serial) },
                &Device::closeShift,
                { s_cliOperatorName, s_cliOperatorInn, true, false }
            );
        } else if (KKM_CMD_EQ(command, L"reset-state", L"e")) {
            callMethod(
                Device { Registry::load(serial) },
                &Device::resetState,
                { s_cliOperatorName, s_cliOperatorInn, true, true }
            );
        } else {
            return std::nullopt;
        }

        return EXIT_SUCCESS;
    }
}
