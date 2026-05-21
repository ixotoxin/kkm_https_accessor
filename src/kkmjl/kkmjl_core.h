// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "kkmjl_except.h"
#include "kkmjl_json.h"
#include <kkm/strings.h>
#include <kkm/device.h>
#include <kkm/registry.h>
#include <kkm/impex.h>
#include <kkm/callhelpers.h>
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace KkmJsonLoader {
    using namespace Kkm;

    [[nodiscard]]
    inline int exec(wchar_t * serialNumber, const wchar_t * fileName) {
        Log::asBackgroundProcess();
        const std::filesystem::path path { fileName };
        std::ifstream file { path };
        if (!file.is_open() || !file.good()) {
            throw Failure(Fmt<c_sStrSize>(Basic::Wcs::c_couldntReadFile, path.filename().wstring()));
        }

        JsonAlloc allocator {};
        JsonDoc details { Json::Type::Object, &allocator };
        details <<= path;

        std::wstring serial { serialNumber };
        if (serial == L"-") {
            serial.clear();
        } else if (serial == L"+") {
            if (!Json::handleKey(details, Wcs::c_id, serial)) {
                serial.clear();
            }
        }

        std::wstring query {};
        if (!Json::handleKey(details, Wcs::c_query, query)) {
            throw Failure(
                Fmt<c_sStrSize>(
                    Json::Wcs::c_requiresProperty,
                    std::wstring_view { Wcs::c_query.s, Wcs::c_query.length }
                )
            );
        }

        JsonDoc result { Json::Type::Object, &allocator };

        if (query == L"learn") {
            std::wstring connString;
            const bool found { Json::handleKey(details, L"connParams"_key, connString) };
            if (!found) {
                throw Failure(Fmt<c_sStrSize>(Kkm::Mbs::c_requiresProperty, "connParams"));
            }
            const auto connParams = Registry::make(connString);
            NewDevice kkm { connParams };
            Registry::save(connParams, kkm);
            StatusResult statusResult {};
            kkm.getStatus(statusResult);
            kkm.printHello();
            result <<= statusResult;
        } else if (query == L"base-status") {
            callMethod(Device { Registry::load(serial) }, &Device::getStatus, result);
        } else if (query == L"status") {
            collectDataFromMethods(
                result,
                Device { Registry::load(serial) },
                &Device::getStatus,
                &Device::getShiftState,
                &Device::getReceiptState,
                &Device::getCashStat
            );
        } else if (query == L"full-status") {
            collectDataFromMethods(
                result,
                Device { Registry::load(serial) },
                &Device::getStatus,
                &Device::getShiftState,
                &Device::getReceiptState,
                &Device::getCashStat,
                &Device::getFndtOfdExchangeStatus,
                &Device::getFndtFnInfo,
                &Device::getFndtRegistrationInfo,
                &Device::getFndtLastRegistration,
                &Device::getFndtLastReceipt,
                &Device::getFndtLastDocument,
                &Device::getFndtErrors,
                &Device::getFfdVersions,
                &Device::getFwVersions
            );
        } else if (query == L"print-demo") {
            callMethod(Device { Registry::load(serial) }, &Device::printDemo, result);
        } else if (query == L"print-non-fiscal-doc") {
            callMethod(Device { Registry::load(serial) }, &Device::printNonFiscalDocument, details, result);
        } else if (query == L"print-info") {
            callMethod(Device { Registry::load(serial) }, &Device::printInfo, result);
        } else if (query == L"print-fn-registrations") {
            callMethod(Device { Registry::load(serial) }, &Device::printFnRegistrations, result);
        } else if (query == L"print-ofd-exchange-status") {
            callMethod(Device { Registry::load(serial) }, &Device::printOfdExchangeStatus, result);
        } else if (query == L"print-ofd-test") {
            callMethod(Device { Registry::load(serial) }, &Device::printOfdTest, result);
        } else if (query == L"print-close-shift-reports") {
            callMethod(Device { Registry::load(serial) }, &Device::printCloseShiftReports, result);
        } else if (query == L"print-last-document") {
            callMethod(Device { Registry::load(serial) }, &Device::printLastDocument, result);
        } else if (query == L"cash-stat") {
            callMethod(Device { Registry::load(serial) }, &Device::getCashStat, result);
        } else if (query == L"cash-in") {
            callMethod(Device { Registry::load(serial) }, &Device::registerCashIn, details, result);
        } else if (query == L"cash-out") {
            callMethod(Device { Registry::load(serial) }, &Device::registerCashOut, details, result);
        } else if (query == L"sell") {
            callMethod(Device { Registry::load(serial) }, &Device::registerSell, details, result);
        } else if (query == L"sell-return") {
            callMethod(Device { Registry::load(serial) }, &Device::registerSellReturn, details, result);
        } else if (query == L"report-z" || query == L"close-shift") {
            callMethod(Device { Registry::load(serial) }, &Device::closeShift, details, result);
        } else if (query == L"report-x") {
            callMethod(Device { Registry::load(serial) }, &Device::reportX, details, result);
        } else if (query == L"reset-state") {
            callMethod(Device { Registry::load(serial) }, &Device::resetState, details, result);
        } else {
            throw Failure(
                Fmt<c_sStrSize>(
                    Json::Wcs::c_requiresProperty,
                    std::wstring_view { Wcs::c_query.s, Wcs::c_query.length }
                )
            );
        }

#if WITH_CRTD || WITH_SNTZ
        result.AddMember(JsonStr(MEMORY_PROFILING_FLAG_KEY), JsonVal(MEMORY_PROFILING_FLAG_VALUE, allocator), allocator);
#endif

        std::wcout << result;
        return EXIT_SUCCESS;
    }

    inline void printError(const std::wstring_view error) {
        std::wcerr
            << L"{\n"
#if WITH_CRTD || WITH_SNTZ
            L"    \"" MEMORY_PROFILING_FLAG_KEY "\": \"" MEMORY_PROFILING_FLAG_VALUE "\",\n"
#endif
            L"    \"" << Json::Wcs::c_successKeyEsc << L"\": false,\n"
            L"    \"" << Json::Wcs::c_messageKeyEsc << L"\": \"" << Json::escapeBasic(error) << L"\"\n"
            L"}";
    }

    [[nodiscard, maybe_unused]]
    inline int safeExec(wchar_t * serialNumber, const wchar_t * fileName) noexcept try {
        return exec(serialNumber, fileName);
    } catch (const Failure & e) {
        printError(e.explain()); // TODO: Подумать, есть ли необходимость в выводе источника исключения
        return EXIT_FAILURE;
    } catch (const std::exception & e) {
        printError(Text::convert(e.what()));
        return EXIT_FAILURE;
    } catch (...) {
        printError(Basic::Wcs::c_somethingWrong);
        return EXIT_FAILURE;
    }
}
