// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "impex.h"
#include "variables.h"
#include "strings.h"
#include "except.h"

namespace Kkm {
    bool assign(JsonDoc & json, const Result & result) {
        if (!json.IsObject()) {
            json.SetObject();
        }
        bool overrideMessage { false };
        if (
            !json.HasMember(Json::Wcs::c_successKey)
            || !json[kv(Json::Wcs::c_successKey)].IsBool()
            || (json[kv(Json::Wcs::c_successKey)].GetBool() && !result.m_success)
        ) {
            Json::setKey(json, Json::Wcs::c_successKey, result.m_success);
            overrideMessage = true;
        }
        if (
            overrideMessage
            || !json.HasMember(Json::Wcs::c_messageKey)
            || !json[kv(Json::Wcs::c_messageKey)].IsString()
            || json[kv(Json::Wcs::c_messageKey)].GetStringLength() == 0
            || json[kv(Json::Wcs::c_messageKey)].GetString() == Basic::Wcs::c_ok
        ) {
            Json::setKey(json, Json::Wcs::c_messageKey, result.m_message);
        }
        assert(json.IsObject());
        assert(json[kv(Json::Wcs::c_successKey)].IsBool());
        return json[kv(Json::Wcs::c_successKey)].GetBool();
    }

    bool assign(JsonDoc & json, const StatusResult & result) {
        using Basic::RefPair;
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj status { json, L"status"_key };
            status[L"blocked"_key] <<= result.m_blocked;
            status[L"cashDrawerOpened"_key] <<= result.m_cashDrawerOpened;
            status[L"coverOpened"_key] <<= result.m_coverOpened;
            status[L"cutError"_key] <<= result.m_cutError;
            status[L"dateTime"_key] <<= result.m_dateTime;
            status[L"documentNumber"_key] <<= result.m_documentNumber;
            status[L"documentType"_key] <<= result.m_documentType;
            status[L"documentTypeText"_key] <<= RefPair { result.m_documentType, Wcs::c_documentTypeLabels };
            status[L"fiscal"_key] <<= result.m_fiscal;
            status[L"fnFiscal"_key] <<= result.m_fnFiscal;
            status[L"fnPresent"_key] <<= result.m_fnPresent;
            status[L"invalidFn"_key] <<= result.m_invalidFn;
            status[L"logicalNumber"_key] <<= result.m_logicalNumber;
            status[L"mode"_key] <<= result.m_mode;
            status[L"model"_key] <<= result.m_model;
            status[L"modelName"_key] <<= result.m_modelName;
            status[L"modelText"_key] <<= RefPair { result.m_model, Wcs::c_models };
            status[L"operatorId"_key] <<= result.m_operatorId;
            status[L"operatorRegistered"_key] <<= result.m_operatorRegistered;
            status[L"paperNearEnd"_key] <<= result.m_paperNearEnd;
            status[L"printerConnectionLost"_key] <<= result.m_printerConnectionLost;
            status[L"printerError"_key] <<= result.m_printerError;
            status[L"printerOverheat"_key] <<= result.m_printerOverheat;
            status[L"receiptLineLength"_key] <<= result.m_receiptLineLength;
            status[L"receiptLineLengthPix"_key] <<= result.m_receiptLineLengthPix;
            status[L"receiptNumber"_key] <<= result.m_receiptNumber;
            status[L"receiptPaperPresent"_key] <<= result.m_receiptPaperPresent;
            status[L"receiptSum"_key] <<= result.m_receiptSum;
            status[L"receiptType"_key] <<= result.m_receiptType;
            status[L"receiptTypeText"_key] <<= RefPair { result.m_receiptType, Wcs::c_receiptTypeLabels };
            status[L"serialNumber"_key] <<= result.m_serialNumber;
            status[L"shiftNumber"_key] <<= result.m_shiftNumber;
            status[L"shiftState"_key] <<= result.m_shiftState;
            status[L"shiftStateText"_key] <<= RefPair { result.m_shiftState, Wcs::c_shiftStateLabels };
            status[L"subMode"_key] <<= result.m_subMode;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const ShiftStateResult & result) {
        using Basic::RefPair;
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj shiftState { json, L"shiftState"_key };
            shiftState[L"documentsCount"_key] <<= result.m_documentsCount;
            shiftState[L"receiptNumber"_key] <<= result.m_receiptNumber;
            shiftState[L"expiredAt"_key] <<= result.m_expirationDateTime;
            shiftState[L"shiftNumber"_key] <<= result.m_shiftNumber;
            shiftState[L"shiftState"_key] <<= result.m_shiftState;
            shiftState[L"shiftStateText"_key] <<= RefPair { result.m_shiftState, Wcs::c_shiftStateLabels };
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const ReceiptStateResult & result) {
        using Basic::RefPair;
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj receiptState { json, L"receiptState"_key };
            receiptState[L"receiptType"_key] <<= result.m_receiptType;
            receiptState[L"receiptTypeText"_key] <<= RefPair { result.m_receiptType, Wcs::c_receiptTypeLabels };
            receiptState[L"receiptNumber"_key] <<= result.m_receiptNumber;
            receiptState[L"documentNumber"_key] <<= result.m_documentNumber;
            receiptState[L"sum"_key] <<= result.m_sum;
            receiptState[L"remainder"_key] <<= result.m_remainder;
            receiptState[L"change"_key] <<= result.m_change;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const CashStatResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj cashStat { json, L"cashStat"_key };
            cashStat[L"sellCashSum"_key] <<= result.m_sellCashSum;
            cashStat[L"sellReturnCashSum"_key] <<= result.m_sellReturnCashSum;
            cashStat[L"cashSum"_key] <<= result.m_cashSum;
            cashStat[L"cashInSum"_key] <<= result.m_cashInSum;
            cashStat[L"cashOutSum"_key] <<= result.m_cashOutSum;
            cashStat[L"cashInCount"_key] <<= result.m_cashInCount;
            cashStat[L"cashOutCount"_key] <<= result.m_cashOutCount;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FndtOfdExchangeStatusResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj ofdExchangeStatus { json, L"ofdExchangeStatus"_key };
            ofdExchangeStatus[L"exchangeStatus"_key] <<= result.m_exchangeStatus;
            ofdExchangeStatus[L"unsentCount"_key] <<= result.m_unsentCount;
            ofdExchangeStatus[L"firstUnsentNumber"_key] <<= result.m_firstUnsentNumber;
            ofdExchangeStatus[L"ofdMessageRead"_key] <<= result.m_ofdMessageRead;
            ofdExchangeStatus[L"firstUnsentDateTime"_key] <<= result.m_firstUnsentDateTime;
            ofdExchangeStatus[L"okpDateTime"_key] <<= result.m_okpDateTime;
            ofdExchangeStatus[L"lastSentDateTime"_key] <<= result.m_lastSentDateTime;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FndtFnInfoResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj fnInfo { json, L"fnInfo"_key };
            fnInfo[L"serial"_key] <<= result.m_serial;
            fnInfo[L"version"_key] <<= result.m_version;
            fnInfo[L"execution"_key] <<= result.m_execution;
            fnInfo[L"type"_key] <<= result.m_type;
            fnInfo[L"state"_key] <<= result.m_state;
            fnInfo[L"flags"_key] <<= result.m_flags;
            fnInfo[L"needReplacement"_key] <<= result.m_needReplacement;
            fnInfo[L"exhausted"_key] <<= result.m_exhausted;
            fnInfo[L"memoryOverflow"_key] <<= result.m_memoryOverflow;
            fnInfo[L"ofdTimeout"_key] <<= result.m_ofdTimeout;
            fnInfo[L"criticalError"_key] <<= result.m_criticalError;
            fnInfo[L"keysUpdaterServerUri"_key] <<= result.m_keysUpdaterServerUri;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FndtRegistrationInfoResult & result) {
        using Basic::RefPair;
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj registrationInfo { json, L"registrationInfo"_key };
            registrationInfo[L"fnsUrl"_key] <<= result.m_fnsUrl;
            registrationInfo[L"organizationAddress"_key] <<= result.m_organizationAddress;
            registrationInfo[L"organizationVATIN"_key] <<= result.m_organizationVATIN;
            registrationInfo[L"organizationName"_key] <<= result.m_organizationName;
            registrationInfo[L"organizationEmail"_key] <<= result.m_organizationEmail;
            registrationInfo[L"paymentsAddress"_key] <<= result.m_paymentsAddress;
            registrationInfo[L"registrationNumber"_key] <<= result.m_registrationNumber;
            registrationInfo[L"machineNumber"_key] <<= result.m_machineNumber;
            registrationInfo[L"ofdVATIN"_key] <<= result.m_ofdVATIN;
            registrationInfo[L"ofdName"_key] <<= result.m_ofdName;
            {
                JsonObj taxationTypes { registrationInfo, L"taxationTypes"_key };
                taxationTypes[L"osn"_key] <<= static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_OSN);
                taxationTypes[L"usnIncome"_key] <<= static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_USN_INCOME);
                taxationTypes[L"usnIncomeOutcome"_key] <<= static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_USN_INCOME_OUTCOME);
                taxationTypes[L"esn"_key] <<= static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_ESN);
                taxationTypes[L"patent"_key] <<= static_cast<bool>(result.m_taxationTypes & Atol::LIBFPTR_TT_PATENT);
            }
            {
                JsonObj agentSign { registrationInfo, L"agentSign"_key };
                agentSign[L"bankPayingAgent"_key] <<= static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_BANK_PAYING_AGENT);
                agentSign[L"bankPayingSubagent"_key] <<= static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_BANK_PAYING_SUBAGENT);
                agentSign[L"payingAgent"_key] <<= static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_PAYING_AGENT);
                agentSign[L"payingSubagent"_key] <<= static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_PAYING_SUBAGENT);
                agentSign[L"attorney"_key] <<= static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_ATTORNEY);
                agentSign[L"commissionAgent"_key] <<= static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_COMMISSION_AGENT);
                agentSign[L"another"_key] <<= static_cast<bool>(result.m_agentSign & Atol::LIBFPTR_AT_ANOTHER);
            }
            registrationInfo[L"ffdVersion"_key] <<= RefPair { result.m_ffdVersion, Wcs::c_ffdVersions };
            registrationInfo[L"autoModeSign"_key] <<= result.m_autoModeSign;
            registrationInfo[L"offlineModeSign"_key] <<= result.m_offlineModeSign;
            registrationInfo[L"encryptionSign"_key] <<= result.m_encryptionSign;
            registrationInfo[L"internetSign"_key] <<= result.m_internetSign;
            registrationInfo[L"serviceSign"_key] <<= result.m_serviceSign;
            registrationInfo[L"bsoSign"_key] <<= result.m_bsoSign;
            registrationInfo[L"lotterySign"_key] <<= result.m_lotterySign;
            registrationInfo[L"gamblingSign"_key] <<= result.m_gamblingSign;
            registrationInfo[L"exciseSign"_key] <<= result.m_exciseSign;
            registrationInfo[L"machineInstallationSign"_key] <<= result.m_machineInstallationSign;
            registrationInfo[L"tradeMarkedProducts"_key] <<= result.m_tradeMarkedProducts;
            registrationInfo[L"insuranceActivity"_key] <<= result.m_insuranceActivity;
            registrationInfo[L"pawnShopActivity"_key] <<= result.m_pawnShopActivity;
            registrationInfo[L"vending"_key] <<= result.m_vending;
            registrationInfo[L"catering"_key] <<= result.m_catering;
            registrationInfo[L"wholesale"_key] <<= result.m_wholesale;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FndtLastRegistrationResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj lastRegistration { json, L"lastRegistration"_key };
            lastRegistration[L"documentNumber"_key] <<= result.m_documentNumber;
            lastRegistration[L"registrationsCount"_key] <<= result.m_registrationsCount;
            lastRegistration[L"registrationDateTime"_key] <<= result.m_registrationDateTime;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FndtLastReceiptResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj lastReceipt { json, L"lastReceipt"_key };
            lastReceipt[L"documentNumber"_key] <<= result.m_documentNumber;
            lastReceipt[L"receiptSum"_key] <<= result.m_receiptSum;
            lastReceipt[L"fiscalSign"_key] <<= result.m_fiscalSign;
            lastReceipt[L"documentDateTime"_key] <<= result.m_documentDateTime;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FndtLastDocumentResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj lastDocument { json, L"lastDocument"_key };
            lastDocument[L"documentNumber"_key] <<= result.m_documentNumber;
            lastDocument[L"fiscalSign"_key] <<= result.m_fiscalSign;
            lastDocument[L"documentDateTime"_key] <<= result.m_documentDateTime;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FndtErrorsResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj fndtErrors { json, L"fndtErrors"_key };
            fndtErrors[L"networkError"_key] <<= result.m_networkError;
            fndtErrors[L"networkErrorText"_key] <<= result.m_networkErrorText;
            fndtErrors[L"ofdError"_key] <<= result.m_ofdError;
            fndtErrors[L"ofdErrorText"_key] <<= result.m_ofdErrorText;
            fndtErrors[L"fnError"_key] <<= result.m_fnError;
            fndtErrors[L"fnErrorText"_key] <<= result.m_fnErrorText;
            fndtErrors[L"failedDocumentNumber"_key] <<= result.m_documentNumber;
            fndtErrors[L"failedCommandCode"_key] <<= result.m_commandCode;
            fndtErrors[L"successDateTime"_key] <<= result.m_successDateTime;
            fndtErrors[L"dataForSendIsEmpty"_key] <<= result.m_dataForSendIsEmpty;
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FfdVersionsResult & result) {
        using Basic::RefPair;
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj ffdVersions { json, L"ffdVersions"_key };
            ffdVersions[L"deviceFfd"_key] <<= RefPair { result.m_deviceFfdVersion, Wcs::c_ffdVersions };
            ffdVersions[L"deviceMaxFfd"_key] <<= RefPair { result.m_devMaxFfdVersion, Wcs::c_ffdVersions };
            ffdVersions[L"deviceMinFfd"_key] <<= RefPair { result.m_devMinFfdVersion, Wcs::c_ffdVersions };
            ffdVersions[L"fnFfd"_key] <<= RefPair { result.m_fnFfdVersion, Wcs::c_ffdVersions };
            ffdVersions[L"fnMaxFfd"_key] <<= RefPair { result.m_fnMaxFfdVersion, Wcs::c_ffdVersions };
            ffdVersions[L"ffd"_key] <<= RefPair { result.m_ffdVersion, Wcs::c_ffdVersions };
            return true;
        }
        return false;
    }

    bool assign(JsonDoc & json, const FwVersionsResult & result) {
        const Result & base { result };
        assign(json, base);
        if (result.m_success) {
            JsonObj fwVersions { json, L"fwVersions"_key };
            fwVersions[L"boot"_key] <<= result.m_bootVersion;
            fwVersions[L"configuration"_key] <<= result.m_configurationVersion;
            fwVersions[L"controlUnit"_key] <<= result.m_controlUnitVersion;
            fwVersions[L"firmware"_key] <<= result.m_firmwareVersion;
            fwVersions[L"release"_key] <<= result.m_releaseVersion;
            fwVersions[L"templates"_key] <<= result.m_templatesVersion;
            return true;
        }
        return false;
    }

    void assign(Details & details, const JsonDoc & json) {
        assert(json.IsObject());
        Json::handleKey(json, L"electronically"_key, details.m_electronically);
    }

    void assign(PrintDetails & details, const JsonDoc & json) {
        Details & base { details };
        assign(base, json);
        bool margin { false };
        Json::handleKey(json, L"cliche"_key, details.m_cliche);
        Json::handleKey(json, L"footer"_key, details.m_footer);
        Json::handleKey(json, L"margin"_key, margin);
        auto & document = Json::requireArray(json, L"document"_key, 1, 80);
        details.m_document.reserve(document.Size());
        for (Json::SizeType i { 0 }; i < document.Size(); ++i) {
            bool center { false };
            bool magnified { false };
            bool separated { false };
            bool separator { false };
            std::wstring content {};
            content.reserve(c_mStrSize);
            std::wstring basePath {};
            basePath.reserve(c_xsStrSize);
            std::format_to(std::back_inserter(basePath), L"document/{}", i);
            Json::handleKey(document[i], L"separator"_key, separator, basePath);
            if (separator) {
                separated = true;
            } else {
                const bool found {
                    Json::handleKey(
                        document[i], L"content"_key, content,
                        Text::length<std::wstring>(1, c_maxTextLength), basePath
                    )
                };
                if (!found) {
                    throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, basePath, L"content"));
                }
                Json::handleKey(document[i], L"center"_key, center, basePath);
                Json::handleKey(document[i], L"magnified"_key, magnified, basePath);
                Json::handleKey(document[i], L"separated"_key, separated, basePath);
            }
            auto actualMargin = margin && !content.empty();
            details.m_document.emplace_back(std::move(content), center, magnified, separated, actualMargin);
        }
    }

    void assign(OperatorDetails & details, const JsonDoc & json) {
        Details & base { details };
        assign(base, json);
        const bool result {
            Json::handleKey(
                json, L"operator"_key,
                [& details] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                    const bool found {
                        Json::handleKey(
                            json2, L"name"_key, details.m_operatorName, Text::Wcs::length(1, 64, Text::Wcs::trim()), jptr2
                        )
                    };
                    if (!found) {
                        throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, jptr2, L"name"));
                    }
                    Json::handleKey(json2, L"inn"_key, details.m_operatorInn, Text::Wcs::maxLength(12), jptr2);
                    return true;
                }
            )
        };
        if (!result) {
            throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty, L"operator"));
        }
    }

    void assign(CashDetails & details, const JsonDoc & json) {
        OperatorDetails & base { details };
        assign(base, json);
        const bool found {
            Json::handleKey(json, L"cashSum"_key, details.m_cashSum, Numeric::between(c_minCashInOut, s_maxCashInOut))
        };
        if (!found) {
            throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty, L"cashSum"));
        }
    }

    void assign(ReceiptDetails & details, const JsonDoc & json) {
        OperatorDetails & base { details };
        assign(base, json);
        Json::handleKey(
            json, L"customer"_key,
            [& details] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(json2, L"account"_key, details.m_customerAccount, Text::Wcs::maxLength(32), jptr2);
                Json::handleKey(json2, L"contact"_key, details.m_customerContact, Text::Wcs::maxLength(64), jptr2);
                Json::handleKey(json2, L"name"_key, details.m_customerName, Text::Wcs::maxLength(256), jptr2);
                Json::handleKey(json2, L"inn"_key, details.m_customerInn, Text::Wcs::maxLength(12), jptr2);
                Json::handleKey(json2, L"birthdate"_key, details.m_customerBirthdate, Text::Wcs::maxLength(10), jptr2);
                Json::handleKey(json2, L"citizenship"_key, details.m_customerCitizenship, Text::Wcs::maxLength(3), jptr2);
                Json::handleKey(json2, L"documentCode"_key, details.m_customerDocumentCode, Text::Wcs::maxLength(32), jptr2);
                Json::handleKey(json2, L"documentData"_key, details.m_customerDocumentData, Text::Wcs::maxLength(64), jptr2);
                Json::handleKey(json2, L"address"_key, details.m_customerAddress, Text::Wcs::maxLength(256), jptr2);
                details.m_customerDataIsPresent
                    = !details.m_customerAccount.empty() || !details.m_customerContact.empty()
                      || !details.m_customerName.empty() || !details.m_customerInn.empty()
                      || !details.m_customerBirthdate.empty() || !details.m_customerCitizenship.empty()
                      || !details.m_customerDocumentCode.empty() || !details.m_customerDocumentData.empty()
                      || !details.m_customerAddress.empty();
                return true;
            }
        );
        Json::handleKey(
            json, L"seller"_key,
            [& details] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(json2, L"email"_key, details.m_sellerEmail, Text::Wcs::maxLength(64), jptr2);
                details.m_sellerDataIsPresent = !details.m_sellerEmail.empty();
                return true;
            }
        );
        Json::handleKey(
            json, L"text"_key,
            [& details] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(
                    json2, L"content"_key, details.m_text.m_content,
                    Text::length<std::wstring>(1, c_maxTextLength), jptr2
                );
                Json::handleKey(json2, L"center"_key, details.m_text.m_center, jptr2);
                Json::handleKey(json2, L"magnified"_key, details.m_text.m_magnified, jptr2);
                Json::handleKey(json2, L"separated"_key, details.m_text.m_separated, jptr2);
                return true;
            }
        );
        Json::handleKey(
            json, L"headerText"_key,
            [& details] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(
                    json2, L"content"_key, details.m_headerText.m_content,
                    Text::length<std::wstring>(1, c_maxTextLength), jptr2
                );
                Json::handleKey(json2, L"center"_key, details.m_headerText.m_center, jptr2);
                Json::handleKey(json2, L"magnified"_key, details.m_headerText.m_magnified, jptr2);
                Json::handleKey(json2, L"separated"_key, details.m_headerText.m_separated, jptr2);
                return true;
            }
        );
        Json::handleKey(
            json, L"footerText"_key,
            [& details] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(
                    json2, L"content"_key, details.m_footerText.m_content,
                    Text::length<std::wstring>(1, c_maxTextLength), jptr2
                );
                Json::handleKey(json2, L"center"_key, details.m_footerText.m_center, jptr2);
                Json::handleKey(json2, L"magnified"_key, details.m_footerText.m_magnified, jptr2);
                Json::handleKey(json2, L"separated"_key, details.m_footerText.m_separated, jptr2);
                return true;
            }
        );
        auto defaultTax { Tax::No };
        const bool hasDefaultTax { Json::handleKey(json, L"tax"_key, defaultTax, Wcs::c_taxCastMap) };
        auto & items = Json::requireArray(json, L"items"_key, 1);
        details.m_items.reserve(items.Size());
        for (Json::SizeType i { 0 }; i < items.Size(); ++i) {
            std::wstring basePath {};
            basePath.reserve(c_xsStrSize);
            std::format_to(std::back_inserter(basePath), L"items/{}", i);
            std::wstring title {};
            title.reserve(c_sStrSize);
            bool found {
                Json::handleKey(items[i], L"title"_key, title, Text::Wcs::length(1, 128, Text::Wcs::trim()), basePath)
            };
            if (!found) {
                throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, basePath, L"title"));
            }
            double price;
            found = Json::handleKey(items[i], L"price"_key, price, Numeric::between(c_minPrice, s_maxPrice), basePath);
            if (!found) {
                throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, basePath, L"price"));
            }
            double quantity;
            found
                = Json::handleKey(
                    items[i], L"quantity"_key, quantity, Numeric::between(c_minQuantity, s_maxQuantity), basePath
                );
            if (!found) {
                throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, basePath, L"quantity"));
            }
            auto unit = MeasurementUnit::Piece;
            Json::handleKey(items[i], L"unit"_key, unit, Wcs::c_measurementUnitMap);
            Tax tax;
            found = Json::handleKey(items[i], L"tax"_key, tax, Wcs::c_taxCastMap);
            if (!found) {
                if (hasDefaultTax) {
                    tax = defaultTax;
                } else {
                    throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, basePath, L"tax"));
                }
            }
            details.m_paymentSum += price * quantity;
            details.m_items.emplace_back(std::move(title), price, quantity, unit, tax);
        }
        const bool paymentFound {
            Json::handleKey(
                json, L"payment"_key,
                [& details] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                    if (std::wstring sum {}; Json::handleKey(json2, L"sum"_key, sum, jptr2)) {
                        try {
                            Text::lower(sum);
                            if (sum == L"auto") {
                                details.m_paymentSum = -1;
                            } else {
                                details.m_paymentSum = Text::cast<double>(sum);
                            }
                        } catch (...) {
                            throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, jptr2, L"sum"));
                        }
                    } else {
                        throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, jptr2, L"sum"));
                    }
                    const bool found {
                        Json::handleKey(json2, L"type"_key, details.m_paymentType, Wcs::c_paymentTypeCastMap, jptr2)
                    };
                    if (!found) {
                        throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, jptr2, L"type"));
                    }
                    if (details.m_paymentType == PaymentType::Electronically) {
                        details.m_electroPaymentInfo
                            = Json::handleKey(
                                json2, L"electroPaymentInfo"_key,
                                [& details] (const JsonVal & json3, const JsonPtr jptr3) -> bool {
                                    bool found3 {
                                        Json::handleKey(json3, L"method"_key, details.m_electroPaymentMethod, jptr3)
                                    };
                                    if (!found3) {
                                        throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, jptr3, L"method"));
                                    }
                                    found3
                                        = Json::handleKey(
                                            json3, L"id"_key, details.m_electroPaymentId,
                                            Text::Wcs::length(1, 256), jptr3
                                        );
                                    if (!found3) {
                                        throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty2, jptr3, L"id"));
                                    }
                                    Json::handleKey(
                                        json3, L"addInfo"_key, details.m_electroPaymentAddInfo,
                                        Text::Wcs::maxLength(256), jptr3
                                    );
                                    return true;
                                }
                            );
                    }
                    return true;
                }
            )
        };
        if (!paymentFound) {
            throw Failure(Fmt<c_sStrSize>(Json::Wcs::c_requiresProperty, L"payment"));
        }
    }

    void assign(CloseDetails & details, const JsonDoc & json) {
        OperatorDetails & base { details };
        assign(base, json);
        Json::handleKey(json, L"closeShift"_key, details.m_closeShift);
        Json::handleKey(json, L"cashOut"_key, details.m_cashOut);
    }
}
