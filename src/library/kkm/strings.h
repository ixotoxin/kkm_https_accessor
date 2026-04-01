// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "types.h"
#include <lib/meta.h>
#include <string_view>
#include <array>

namespace Kkm {
    namespace Wcs {
        KKM_WSTRI(c_requiresProperty, L"Требуется корректное свойство '{}'");
        KKM_WSTRI(c_unsupportedValue, L"В данный момент значение '{}' свойства '{}' не поддерживается");
        KKM_WSTRI(c_requiresProperty2, L"Требуется корректное свойство '{}.{}'");
        KKM_WSTRI(c_unsupportedValue2, L"В данный момент значение '{}' свойства '{}.{}' не поддерживается");

        KKM_WSTRI(c_invalidSerialNumber, L"Недопустимый серийный номер");
        KKM_WSTRI(c_invalidConnParams, L"Недопустимые параметры подключения");
        KKM_WSTRI(c_notImplemented, L"Не реализовано");
        KKM_WSTRI(c_notAvailable, L"Недоступна");
        KKM_WSTRI(c_invalidData, L"Ошибка данных");
        KKM_WSTRI(c_exchangeError, L"Ошибка обмена с ККМ");
        KKM_WSTRI(c_serialNumberMismatch, L"Ошибка подключения (ожидаемый СН: {}, фактический СН: {})");

        KKM_WSTRI(c_registryError, L"Ошибка доступа к реестру ККМ");
        KKM_WSTRI(c_saved, L"Параметры подключения ККМ [{}] успешно сохранены");
        KKM_WSTRI(c_savingError, L"Не удалось сохранить параметры подключения ККМ [{}]");
        KKM_WSTRI(c_loaded, L"Параметры подключения ККМ [{}] успешно загружены");
        KKM_WSTRI(c_loadingError, L"Не удалось загрузить параметры подключения ККМ [{}]");

        KKM_WSTRI(c_kkmPrefix, L"ККМ [{}]: ");
        KKM_WSTRI(c_fault, L"Ошибка: {}");
        KKM_WSTRI(c_wrongLength, L"Не удалось получить ширину ленты, будет использовано значение по-умолчанию");
        KKM_WSTRI(c_cancelingError, L"Ошибка отмены чека: {}");
        KKM_WSTRI(c_closingError, L"Ошибка закрытия документа: {}");
        KKM_WSTRI(c_printingError, L"Ошибка печати документа: {}");
        KKM_WSTRI(c_shiftMismatch, L"Номер смены в ККМ не совпадает с номером смены в ФН");
        KKM_WSTRI(c_checkingError, L"Не удалось проверить закрытие документа");
        KKM_WSTRI(c_requiresItems, L"Не предоставлено ни одной позиции");
        KKM_WSTRI(c_badOperator, L"Данные оператора некорректны");
        KKM_WSTRI(c_badCustomer, L"Данные покупателя некорректны");

        KKM_WSTRI(c_typeSell, L"ПРИХОД");
        KKM_WSTRI(c_typeSellReturn, L"ВОЗВРАТ ПРИХОДА");

        KKM_WSTRI(c_subSetOperator, L"Операция: Регистрация оператора");
        KKM_WSTRI(c_subSetCustomer, L"Операция: Регистрация покупателя");
        KKM_WSTRI(c_subSetSeller, L"Операция: Регистрация продавца");
        KKM_WSTRI(c_subRegisterCashInAndPrint, L"Операция: Регистрация внесения и печать");
        KKM_WSTRI(c_subCashOut, L"Операция: Выплата");
        KKM_WSTRI(c_subCashOutNoNeed, L"Выплата не требуется");
        KKM_WSTRI(c_subRegisterCashOutAndPrint, L"Операция: Регистрация выплаты и печать");
        KKM_WSTRI(c_subRegisterItems, L"Операция: Регистрация позиций (товаров/услуг)");
        KKM_WSTRI(c_subRegisterPayment, L"Операция: Регистрация оплаты");
        KKM_WSTRI(c_subRegisterReceiptAndPrint, L"Операция: Регистрация чека и печать");
        KKM_WSTRI(c_subCloseShift, L"Операция: Закрытие смены");
        KKM_WSTRI(c_subCloseShiftNoNeed, L"Закрытие смены не требуется");
        KKM_WSTRI(c_subCancelReceipt, L"Операция: Отмена открытого чека");
        KKM_WSTRI(c_subCancelReceiptNoNeed, L"Отмена открытого чека не требуется");
        KKM_WSTRI(c_subPrint, L"Операция: Печать");

        KKM_WSTRI(c_statusMethod, L"Запрос общей информации и статуса");
        KKM_WSTRI(c_shiftStateMethod, L"Запрос состояния смены");
        KKM_WSTRI(c_receiptStateMethod, L"Запрос состояния чека");
        KKM_WSTRI(c_cashStatMethod, L"Запрос статистики по наличным");
        KKM_WSTRI(c_ofdExchangeStatusMethod, L"Запрос статуса информационного обмена с ОФД");
        KKM_WSTRI(c_fnInfoMethod, L"Запрос информации и статуса ФН");
        KKM_WSTRI(c_registrationInfoMethod, L"Запрос информации о регистрации");
        KKM_WSTRI(c_lastRegistrationMethod, L"Запрос информации о последней (пере)регистрации");
        KKM_WSTRI(c_lastReceiptMethod, L"Запрос информации о последнем чеке");
        KKM_WSTRI(c_lastDocumentMethod, L"Запрос информации о последнем фискальном документе");
        KKM_WSTRI(c_errorsMethod, L"Запрос информации об ошибках обмена с ОФД");
        KKM_WSTRI(c_ffdVersionMethod, L"Запрос версий ФФД");
        KKM_WSTRI(c_fwVersionMethod, L"Запрос версий ПО");
        KKM_WSTRI(c_printDemoMethod, L"Демо-печать");
        KKM_WSTRI(c_printHelloMethod, L"Печать приветствия");
        KKM_WSTRI(c_printNfDocumentMethod, L"Печать не фискального документа");
        KKM_WSTRI(c_printInfoMethod, L"Печать информации о ККТ");
        KKM_WSTRI(c_printFnRegistrationsMethod, L"Печать итогов регистрации / перерегистрации");
        KKM_WSTRI(c_printOfdExchangeStatusMethod, L"Печать отчета о состоянии расчетов");
        KKM_WSTRI(c_printOfdTestMethod, L"Тестирование подключения к ОФД");
        KKM_WSTRI(c_printShiftReportsMethod, L"Печать нераспечатанных отчетов о закрытии смены");
        KKM_WSTRI(c_printLastDocumentMethod, L"Печать копии последнего документа");
        KKM_WSTRI(c_cashInMethod, L"Регистрация внесения");
        KKM_WSTRI(c_cashOutMethod, L"Регистрация выплаты");
        KKM_WSTRI(c_sellMethod, L"Регистрация чека прихода (продажи)");
        KKM_WSTRI(c_sellReturnMethod, L"Регистрация чека возврата прихода (продажи)");
        KKM_WSTRI(c_reportXMethod, L"Печать X-отчета");
        KKM_WSTRI(c_closeShiftMethod, L"Закрытие смены");
        KKM_WSTRI(c_resetStateMethod, L"Возврат к исходному состоянию");

        KKM_WSTRI(c_helloTitle, L"Привет!");
        KKM_WSTRI(c_helloText, L"\nЯ трудолюбивая касса.\n\nЯ очень рада работать в компании таких замечательных людей.");

        inline const std::array c_allowedBaudRate {
            std::to_wstring(Atol::LIBFPTR_PORT_BR_1200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_2400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_4800),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_9600),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_19200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_38400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_57600),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_115200),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_230400),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_460800),
            std::to_wstring(Atol::LIBFPTR_PORT_BR_921600)
        };
    }

    namespace Mbs {
        KKM_STRI(c_requiresProperty, "Требуется корректное свойство '{}'");
        KKM_STRI(c_requiresProperty2, "Требуется корректное свойство '{}.{}'");

        KKM_STRI(c_closedShift, "Сессия закрыта");
        KKM_STRI(c_openedShift, "Сессия открыта");
        KKM_STRI(c_expiredShift, "Сессия истекла");
        KKM_STRI(c_closedReceipt, "Чек закрыт");
        KKM_STRI(c_sellReceipt, "Чек прихода");
        KKM_STRI(c_sellReturnReceipt, "Чек возврата прихода");
        KKM_STRI(c_sellCorrectionReceipt, "Чек коррекции прихода");
        KKM_STRI(c_sellReturnCorrectionReceipt, "Чек коррекции возврата прихода");
        KKM_STRI(c_buyReceipt, "Чек расхода");
        KKM_STRI(c_buyReturnReceipt, "Чек возврата расхода");
        KKM_STRI(c_buyCorrectionReceipt, "Чек коррекции расхода");
        KKM_STRI(c_buyReturnCorrectionReceipt, "Чек коррекции возврата расхода");
        KKM_STRI(c_closedDocument, "Документ закрыт");
        KKM_STRI(c_openShiftDocument, "Документ открытия смены");
        KKM_STRI(c_closeShiftDocument, "Документ закрытия смены");
        KKM_STRI(c_registrationDocument, "Документ пере/регистрации");
        KKM_STRI(c_closeArchiveDocument, "Документ закрытия архива ФН");
        KKM_STRI(c_ofdExchangeStatusDocument, "Отчёт о состоянии расчётов");
        KKM_STRI(c_serviceDocument, "Сервисный документ");
        KKM_STRI(c_documentCopy, "Копия документа");

        inline const std::unordered_map<unsigned int, std::string_view> c_models {
            { Atol::LIBFPTR_MODEL_ALLIANCE_20F, "АЛЬЯНС 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_1F, "АТОЛ 1Ф" },
#if VERSION_LIMIT >= VERSION_10107
            { Atol::LIBFPTR_MODEL_ATOL_2F, "АТОЛ 2Ф" },
#endif
            { Atol::LIBFPTR_MODEL_ATOL_11F, "АТОЛ 11Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_15F, "АТОЛ 15Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_20F, "АТОЛ 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_22F, "АТОЛ 22Ф (АТОЛ FPrint-22ПТК)" },
            { Atol::LIBFPTR_MODEL_ATOL_22V2F, "АТОЛ 22 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_25F, "АТОЛ 25Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27F, "АТОЛ 27Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27_FP7_F, "АТОЛ 27 FP7 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_30F, "АТОЛ 30Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_35F, "АТОЛ 35Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_42FA, "АТОЛ 42ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_42FS, "АТОЛ 42ФС" },
            { Atol::LIBFPTR_MODEL_ATOL_47FA, "АТОЛ 47ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_50F, "АТОЛ 50Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_52F, "АТОЛ 52Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55F, "АТОЛ 55Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55V2F, "АТОЛ 55 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_60F, "АТОЛ 60Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_77F, "АТОЛ 77Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_90F, "АТОЛ 90Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_91F, "АТОЛ 91Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_92F, "АТОЛ 92Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_PT_5F, "АТОЛ PT-5Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_10, "АТОЛ Sigma 10" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_7F, "АТОЛ Sigma 7Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_8F, "АТОЛ Sigma 8Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_STB_6F, "АТОЛ СТБ 6Ф" },
            { Atol::LIBFPTR_MODEL_KAZNACHEY_FA, "Казначей ФА" }
        };

#if VERSION_LIMIT >= VERSION_10107
        inline const std::unordered_map<std::string, TimeZone> c_timeZoneMap {
            { "device", TimeZone::Device },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Device)), TimeZone::Device },
            { "+2", TimeZone::Zone1 },
            { "utc+2", TimeZone::Zone1 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone1)), TimeZone::Zone1 },
            { "+3", TimeZone::Zone2 },
            { "utc+3", TimeZone::Zone2 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone2)), TimeZone::Zone2 },
            { "+4", TimeZone::Zone3 },
            { "utc+4", TimeZone::Zone3 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone3)), TimeZone::Zone3 },
            { "+5", TimeZone::Zone4 },
            { "utc+5", TimeZone::Zone4 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone4)), TimeZone::Zone4 },
            { "+6", TimeZone::Zone5 },
            { "utc+6", TimeZone::Zone5 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone5)), TimeZone::Zone5 },
            { "+7", TimeZone::Zone6 },
            { "utc+7", TimeZone::Zone6 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone6)), TimeZone::Zone6 },
            { "+8", TimeZone::Zone7 },
            { "utc+8", TimeZone::Zone7 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone7)), TimeZone::Zone7 },
            { "+9", TimeZone::Zone8 },
            { "utc+9", TimeZone::Zone8 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone8)), TimeZone::Zone8 },
            { "+10", TimeZone::Zone9 },
            { "utc+10", TimeZone::Zone9 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone9)), TimeZone::Zone9 },
            { "+11", TimeZone::Zone10 },
            { "utc+11", TimeZone::Zone10 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone10)), TimeZone::Zone10 },
            { "+12", TimeZone::Zone11 },
            { "utc+12", TimeZone::Zone11 },
            { "tz" + std::to_string(Meta::toUnderlying(TimeZone::Zone11)), TimeZone::Zone11 }
        };
#endif

        inline const std::unordered_map<FfdVersion, std::string_view> c_ffdVersions {
            { FfdVersion::Unknown, "[неизвестная версия]" },
            { FfdVersion::V_1_0_5, "1.0.5" },
            { FfdVersion::V_1_1, "1.1" },
            { FfdVersion::V_1_2, "1.2" }
        };

        inline const std::unordered_map<std::string, FfdVersion> c_ffdVersionsMap {
            { "1.0.5", FfdVersion::V_1_0_5 },
            { "1.1", FfdVersion::V_1_1 },
            { "1.2", FfdVersion::V_1_2 }
        };

        inline const std::unordered_map<FfdVersionDetect, std::string_view> c_ffdVersionDetect {
            { FfdVersionDetect::Never, "never" },
            { FfdVersionDetect::Once, "once" },
            /*{ FfdVersionDetect::Sometimes, "sometimes" },*/
            { FfdVersionDetect::Always, "always" }
        };

        inline const std::unordered_map<std::string, FfdVersionDetect> c_ffdVersionDetectMap {
            { "never", FfdVersionDetect::Never },
            { "once", FfdVersionDetect::Once },
            /*{ "sometimes", FfdVersionDetect::Sometimes },*/
            { "always", FfdVersionDetect::Always }
        };

        inline const std::unordered_map<ShiftState, std::string_view> c_shiftStateLabels {
            { ShiftState::Closed, c_closedShift },
            { ShiftState::Opened, c_openedShift },
            { ShiftState::Expired, c_expiredShift }
        };

        inline const std::unordered_map<ReceiptType, std::string_view> c_receiptTypeLabels {
            { ReceiptType::Closed, c_closedReceipt },
            { ReceiptType::Sell, c_sellReceipt },
            { ReceiptType::SellReturn, c_sellReturnReceipt },
            { ReceiptType::SellCorrection, c_sellCorrectionReceipt },
            { ReceiptType::SellReturnCorrection, c_sellReturnCorrectionReceipt },
            { ReceiptType::Buy, c_buyReceipt },
            { ReceiptType::BuyReturn, c_buyReturnReceipt },
            { ReceiptType::BuyCorrection, c_buyCorrectionReceipt },
            { ReceiptType::BuyReturnCorrection, c_buyReturnCorrectionReceipt }
        };

        inline const std::unordered_map<DocumentType, std::string_view> c_documentTypeLabels {
            { DocumentType::Closed, c_closedDocument },
            { DocumentType::ReceiptSell, c_sellReceipt },
            { DocumentType::ReceiptSellReturn, c_sellReturnReceipt },
            { DocumentType::ReceiptSellCorrection, c_sellCorrectionReceipt },
            { DocumentType::ReceiptSellReturnCorrection, c_sellReturnCorrectionReceipt },
            { DocumentType::ReceiptBuy, c_buyReceipt },
            { DocumentType::ReceiptBuyReturn, c_buyReturnReceipt },
            { DocumentType::ReceiptBuyCorrection, c_buyCorrectionReceipt },
            { DocumentType::ReceiptBuyReturnCorrection, c_buyReturnCorrectionReceipt },
            { DocumentType::OpenShift, c_openShiftDocument },
            { DocumentType::CloseShift, c_closeShiftDocument },
            { DocumentType::Registration, c_registrationDocument },
            { DocumentType::CloseArchive, c_closeArchiveDocument },
            { DocumentType::OfdExchangeStatus, c_ofdExchangeStatusDocument },
            { DocumentType::DocumentService, c_serviceDocument },
            { DocumentType::DocumentCopy, c_documentCopy }
        };

        inline const std::unordered_map<std::string, MeasurementUnit> c_measurementUnitMap {
            { "piece", MeasurementUnit::Piece },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Piece)), MeasurementUnit::Piece },
            { "gram", MeasurementUnit::Gram },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Gram)), MeasurementUnit::Gram },
            { "kilogram", MeasurementUnit::Kilogram },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Kilogram)), MeasurementUnit::Kilogram },
            { "ton", MeasurementUnit::Ton },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Ton)), MeasurementUnit::Ton },
            { "centimeter", MeasurementUnit::Centimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Centimeter)), MeasurementUnit::Centimeter },
            { "decimeter", MeasurementUnit::Decimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Decimeter)), MeasurementUnit::Decimeter },
            { "meter", MeasurementUnit::Meter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Meter)), MeasurementUnit::Meter },
            { "squarecentimeter", MeasurementUnit::SquareCentimeter },
            { "square_centimeter", MeasurementUnit::SquareCentimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::SquareCentimeter)), MeasurementUnit::SquareCentimeter },
            { "squaredecimeter", MeasurementUnit::SquareDecimeter },
            { "square_decimeter", MeasurementUnit::SquareDecimeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::SquareDecimeter)), MeasurementUnit::SquareDecimeter },
            { "squaremeter", MeasurementUnit::SquareMeter },
            { "square_meter", MeasurementUnit::SquareMeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::SquareMeter)), MeasurementUnit::SquareMeter },
            { "milliliter", MeasurementUnit::Milliliter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Milliliter)), MeasurementUnit::Milliliter },
            { "liter", MeasurementUnit::Liter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Liter)), MeasurementUnit::Liter },
            { "cubicmeter", MeasurementUnit::CubicMeter },
            { "cubic_meter", MeasurementUnit::CubicMeter },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::CubicMeter)), MeasurementUnit::CubicMeter },
            { "kilowatthour", MeasurementUnit::KilowattHour },
            { "kilowatt_hour", MeasurementUnit::KilowattHour },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::KilowattHour)), MeasurementUnit::KilowattHour },
            { "gkal", MeasurementUnit::Gkal },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Gkal)), MeasurementUnit::Gkal },
            { "day", MeasurementUnit::Day },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Day)), MeasurementUnit::Day },
            { "hour", MeasurementUnit::Hour },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Hour)), MeasurementUnit::Hour },
            { "minute", MeasurementUnit::Minute },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Minute)), MeasurementUnit::Minute },
            { "second", MeasurementUnit::Second },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Second)), MeasurementUnit::Second },
            { "kilobyte", MeasurementUnit::Kilobyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Kilobyte)), MeasurementUnit::Kilobyte },
            { "megabyte", MeasurementUnit::Megabyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Megabyte)), MeasurementUnit::Megabyte },
            { "gigabyte", MeasurementUnit::Gigabyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Gigabyte)), MeasurementUnit::Gigabyte },
            { "terabyte", MeasurementUnit::Terabyte },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Terabyte)), MeasurementUnit::Terabyte },
            { "other", MeasurementUnit::Other },
            { std::to_string(Meta::toUnderlying(MeasurementUnit::Other)), MeasurementUnit::Other }
        };

        inline const std::unordered_map<std::string, Tax> c_taxCastMap {
            { "no", Tax::No },
            { std::to_string(Meta::toUnderlying(Tax::No)), Tax::No },
            { "0%", Tax::Vat0 },
            { "vat0", Tax::Vat0 },
            { std::to_string(Meta::toUnderlying(Tax::Vat0)), Tax::Vat0 },
            { "5%", Tax::Vat5 },
            { "vat5", Tax::Vat5 },
            { std::to_string(Meta::toUnderlying(Tax::Vat5)), Tax::Vat5 },
            { "105%", Tax::Vat105 },
            { "vat105", Tax::Vat105 },
            { std::to_string(Meta::toUnderlying(Tax::Vat105)), Tax::Vat105 },
            { "7%", Tax::Vat7 },
            { "vat7", Tax::Vat7 },
            { std::to_string(Meta::toUnderlying(Tax::Vat7)), Tax::Vat7 },
            { "107%", Tax::Vat107 },
            { "vat107", Tax::Vat107 },
            { std::to_string(Meta::toUnderlying(Tax::Vat107)), Tax::Vat107 },
            { "10%", Tax::Vat10 },
            { "vat10", Tax::Vat10 },
            { std::to_string(Meta::toUnderlying(Tax::Vat10)), Tax::Vat10 },
            { "110%", Tax::Vat110 },
            { "vat110", Tax::Vat110 },
            { std::to_string(Meta::toUnderlying(Tax::Vat110)), Tax::Vat110 },
            { "20%", Tax::Vat20 },
            { "vat20", Tax::Vat20 },
            { std::to_string(Meta::toUnderlying(Tax::Vat20)), Tax::Vat20 },
            { "120%", Tax::Vat120 },
            { "vat120", Tax::Vat120 },
            { std::to_string(Meta::toUnderlying(Tax::Vat120)), Tax::Vat120 },
#if VERSION_LIMIT >= VERSION_10108
            { "22%", Tax::Vat22 },
            { "vat22", Tax::Vat22 },
            { std::to_string(Meta::toUnderlying(Tax::Vat22)), Tax::Vat22 },
            { "122%", Tax::Vat122 },
            { "vat122", Tax::Vat122 },
            { std::to_string(Meta::toUnderlying(Tax::Vat122)), Tax::Vat122 },
#endif
        };

        inline const std::unordered_map<std::string, PaymentType> c_paymentTypeCastMap {
            { "cash", PaymentType::Cash },
            { std::to_string(Meta::toUnderlying(PaymentType::Cash)), PaymentType::Cash },
            { "electronically", PaymentType::Electronically },
            { std::to_string(Meta::toUnderlying(PaymentType::Electronically)), PaymentType::Electronically }
        };
    }
}
