// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "kkmop_strings.h"
#include <main/shortcut.h>
#include <log/write.h>
#include <kkm/variables.h>
#include <kkm/strings.h>
#include <kkm/device.h>
#include <kkm/callhelpers.h>
#include <cassert>
#include <cstdlib>
#include <optional>

#define KKM_LOG(S, F, V) \
    do { if ((S).m_success) { LOG_INFO_FMT(F, V); } else { LOG_ERROR_FMT(F, Wcs::c_error); } } while (false)

namespace KkmOperator {
    using namespace Kkm;

    [[nodiscard]]
    inline int learn(const int connParamCount, wchar_t ** connParamItems) {
        assert(connParamCount > 0);
        // TODO: Убедиться, что строка ниже не будет оптимизирована
        [[maybe_unused]] Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };

        for (int i = 0, n = 1; i < connParamCount; ++i, ++n) {
            try {
                NewConnParams connParams { std::wstring { connParamItems[i] } };
                Device kkm { connParams, std::format(Wcs::c_commandPrefix, n) };
                std::wstring serialNumber { kkm.serialNumber() };
                LOG_DEBUG_NTS(Wcs::c_getKkmInfo, n, serialNumber);
                connParams.save(serialNumber);
                kkm.printHello();
                LOG_INFO_NTS(Wcs::c_connParamsSaved, n, serialNumber);
            } catch (const Failure & e) {
                LOG_WARNING_NTS(std::format(Wcs::c_prefixedText, n, e.explain(Log::s_appendLocation)));
            }
        }

        return EXIT_SUCCESS;
    }

    template<class R, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, UndetailedMethod<R> method) {
        // TODO: Убедиться, что строка ниже не будет оптимизирована
        [[maybe_unused]] Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };
        R result {};
        (kkm.*method)(result);
        if (result.m_success) {
            LOG_INFO_CLI(L"Done");
        } else {
            throw Basic::Failure(result.m_message); // NOLINT(*-exception-baseclass)
        }
    }

    template<class R, class D, class K>
    requires std::derived_from<std::remove_cvref_t<K>, Device>
    [[maybe_unused]]
    void callMethod(K && kkm, DetailedMethod<R, D> method, D && details) {
        // TODO: Убедиться, что строка ниже не будет оптимизирована
        [[maybe_unused]] Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };
        R result {};
        (kkm.*method)(details, result);
        if (result.m_success) {
            LOG_INFO_CLI(L"Done");
        } else {
            throw Basic::Failure(result.m_message); // NOLINT(*-exception-baseclass)
        }
    }

    [[nodiscard]]
    inline std::optional<int> exec(const std::wstring & command, wchar_t * serialNumber) {
        std::wstring serial { serialNumber };

        if (KKM_CMD_EQ(command, L"status", L"v")) {
            Device kkm { KnownConnParams { serial } };

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
                // TODO: Убедиться, что строка ниже не будет оптимизирована
                [[maybe_unused]] Log::Console::ScopeLevelDown scopeLevel { Log::Level::Info };
                // KKM_LOG(status, Wcs::c_fmtModel, wcsSafeGet(Mbs::c_models, status.m_model));
                KKM_LOG(status, Wcs::c_fmtModel, status.m_modelName);
                KKM_LOG(status, Wcs::c_fmtSerialNumber, status.m_serialNumber);
                KKM_LOG(status, Wcs::c_fmtBlocked, Text::Wcs::daNet(status.m_blocked));
                KKM_LOG(status, Wcs::c_fmtLogicalNumber, status.m_logicalNumber);
                KKM_LOG(status, Wcs::c_fmtDateTime, DateTime::cast<std::wstring>(status.m_dateTime));
                KKM_LOG(status, Wcs::c_fmtFnPresent, Text::Wcs::daNet(status.m_fnPresent));
                KKM_LOG(status, Wcs::c_fmtFnFiscal, Text::Wcs::daNet(status.m_fnFiscal));
                KKM_LOG(status, Wcs::c_fmtInvalidFn, Text::Wcs::daNet(status.m_invalidFn));
                KKM_LOG(status, Wcs::c_fmtFiscal, Text::Wcs::daNet(status.m_fiscal));
                KKM_LOG(status, Wcs::c_fmtCashDrawerOpened, Text::Wcs::daNet(status.m_cashDrawerOpened));
                KKM_LOG(status, Wcs::c_fmtCoverOpened, Text::Wcs::daNet(status.m_coverOpened));
                KKM_LOG(status, Wcs::c_fmtReceiptPaperPresent, Text::Wcs::daNet(status.m_receiptPaperPresent));
                KKM_LOG(status, Wcs::c_fmtPaperNearEnd, Text::Wcs::daNet(status.m_paperNearEnd));
                KKM_LOG(status, Wcs::c_fmtCutError, Text::Wcs::daNet(status.m_cutError));
                KKM_LOG(status, Wcs::c_fmtPrinterOverheat, Text::Wcs::daNet(status.m_printerOverheat));
                KKM_LOG(status, Wcs::c_fmtReceiptLineLength, status.m_receiptLineLength);
                KKM_LOG(status, Wcs::c_fmtReceiptLineLengthPix, status.m_receiptLineLengthPix);
                KKM_LOG(shift, Wcs::c_fmtShiftState, wcsSafeGet(Mbs::c_shiftStateLabels, shift.m_shiftState));
                KKM_LOG(shift, Wcs::c_fmtShiftExpiration, DateTime::cast<std::wstring>(shift.m_expirationDateTime));
                KKM_LOG(status, Wcs::c_fmtReceiptType, wcsSafeGet(Mbs::c_receiptTypeLabels, status.m_receiptType));
                KKM_LOG(status, Wcs::c_fmtDocumentType, wcsSafeGet(Mbs::c_documentTypeLabels, status.m_documentType));

                KKM_LOG(cash, Wcs::c_fmtCashInCount, cash.m_cashInCount);
                KKM_LOG(cash, Wcs::c_fmtCashInSum, cash.m_cashInSum);
                KKM_LOG(cash, Wcs::c_fmtCashOutCount, cash.m_cashOutCount);
                KKM_LOG(cash, Wcs::c_fmtCashOutSum, cash.m_cashOutSum);
                KKM_LOG(cash, Wcs::c_fmtSellCashSum, cash.m_sellCashSum);
                KKM_LOG(cash, Wcs::c_fmtSellReturnCashSum, cash.m_sellReturnCashSum);
                KKM_LOG(cash, Wcs::c_fmtCashSum, cash.m_cashSum);

                if (exchange.m_success) {
                    LOG_INFO_CLI(Wcs::c_ofdExchangeStatus);
                    LOG_INFO_FMT(Wcs::c_fmtOfdExSBit0, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'0001));
                    LOG_INFO_FMT(Wcs::c_fmtOfdExSBit1, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'0010));
                    LOG_INFO_FMT(Wcs::c_fmtOfdExSBit2, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'0100));
                    LOG_INFO_FMT(Wcs::c_fmtOfdExSBit3, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0000'1000));
                    LOG_INFO_FMT(Wcs::c_fmtOfdExSBit4, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0001'0000));
                    LOG_INFO_FMT(Wcs::c_fmtOfdExSBit5, Text::Wcs::daNet(exchange.m_exchangeStatus & 0b0010'0000));
                } else {
                    LOG_ERROR_CLI(Wcs::c_ofdExchangeError);
                }
                KKM_LOG(exchange, Wcs::c_fmtUnsentCount, exchange.m_unsentCount);
                KKM_LOG(exchange, Wcs::c_fmtFirstUnsentNumber, exchange.m_firstUnsentNumber);
                KKM_LOG(
                    exchange,
                    Wcs::c_fmtFirstUnsentDateTime,
                    DateTime::cast<std::wstring>(exchange.m_firstUnsentDateTime)
                );
                KKM_LOG(exchange, Wcs::c_fmtOkpDateTime, DateTime::cast<std::wstring>(exchange.m_okpDateTime));
                KKM_LOG(exchange, Wcs::c_fmtOfdMessageRead, Text::Wcs::daNet(exchange.m_ofdMessageRead));
                KKM_LOG(exchange, Wcs::c_fmtLastSentDateTime, DateTime::cast<std::wstring>(exchange.m_lastSentDateTime));

                KKM_LOG(errors, Wcs::c_fmtSuccessDateTime, DateTime::cast<std::wstring>(errors.m_successDateTime));
                KKM_LOG(errors, Wcs::c_fmtNetworkError, errors.m_networkError);
                KKM_LOG(errors, Wcs::c_fmtNetworkErrorText, errors.m_networkErrorText);
                KKM_LOG(errors, Wcs::c_fmtOfdError, errors.m_ofdError);
                KKM_LOG(errors, Wcs::c_fmtOfdErrorText, errors.m_ofdErrorText);
                KKM_LOG(errors, Wcs::c_fmtFnError, errors.m_fnError);
                KKM_LOG(errors, Wcs::c_fmtFnErrorText, errors.m_fnErrorText);
                KKM_LOG(errors, Wcs::c_fmtDocumentNumber, errors.m_documentNumber);
                KKM_LOG(errors, Wcs::c_fmtCommandCode, errors.m_commandCode);
                KKM_LOG(errors, Wcs::c_fmtDataForSendIsEmpty, Text::Wcs::daNet(errors.m_dataForSendIsEmpty));

                KKM_LOG(ffdVers, Wcs::c_fmtDeviceFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_deviceFfdVersion));
                KKM_LOG(ffdVers, Wcs::c_fmtDevMinFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_devMinFfdVersion));
                KKM_LOG(ffdVers, Wcs::c_fmtDevMaxFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_devMaxFfdVersion));
                KKM_LOG(ffdVers, Wcs::c_fmtFnFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_fnFfdVersion));
                KKM_LOG(ffdVers, Wcs::c_fmtFnMaxFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_fnMaxFfdVersion));
                KKM_LOG(ffdVers, Wcs::c_fmtFfdVersion, wcsSafeGet(Mbs::c_ffdVersions, ffdVers.m_ffdVersion));

                KKM_LOG(fwVers, Wcs::c_fmtFirmwareVersion, fwVers.m_firmwareVersion);
                KKM_LOG(fwVers, Wcs::c_fmtConfigurationVersion, fwVers.m_configurationVersion);
                KKM_LOG(fwVers, Wcs::c_fmtReleaseVersion, fwVers.m_releaseVersion);
                KKM_LOG(fwVers, Wcs::c_fmtTemplatesVersion, fwVers.m_templatesVersion);
                KKM_LOG(fwVers, Wcs::c_fmtControlUnitVersion, fwVers.m_controlUnitVersion);
                KKM_LOG(fwVers, Wcs::c_fmtBootVersion, fwVers.m_bootVersion);
            }
        } else if (KKM_CMD_EQ(command, L"demo-print", L"d")) {
            callMethod(Device { KnownConnParams { serial } }, &Device::printDemo);
        } else if (KKM_CMD_EQ(command, L"ofd-test", L"t")) {
            callMethod(Device { KnownConnParams { serial } }, &Device::printOfdTest);
        } else if (KKM_CMD_EQ(command, L"shift-reports", L"j")) {
            callMethod(Device { KnownConnParams { serial } }, &Device::printCloseShiftReports);
        } else if (KKM_CMD_EQ(command, L"last-document", L"m")) {
            callMethod(Device { KnownConnParams { serial } }, &Device::printLastDocument);
        } else if (KKM_CMD_EQ(command, L"report-x", L"p")) {
            callMethod(
                Device { KnownConnParams { serial } },
                &Device::reportX,
                { s_cliOperatorName, s_cliOperatorInn, false, false }
            );
        } else if (KKM_CMD_EQ(command, L"close-shift", L"s")) {
            callMethod(
                Device { KnownConnParams { serial } },
                &Device::closeShift,
                { s_cliOperatorName, s_cliOperatorInn, true, false }
            );
        } else if (KKM_CMD_EQ(command, L"reset-state", L"e")) {
            callMethod(
                Device { KnownConnParams { serial } },
                &Device::resetState,
                { s_cliOperatorName, s_cliOperatorInn, true, true }
            );
        } else {
            return std::nullopt;
        }

        return EXIT_SUCCESS;
    }
}
