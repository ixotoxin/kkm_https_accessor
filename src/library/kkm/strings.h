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
    }

    namespace Wcs {
        KKM_WSTRI(c_closedShift, L"Сессия закрыта");
        KKM_WSTRI(c_openedShift, L"Сессия открыта");
        KKM_WSTRI(c_expiredShift, L"Сессия истекла");
        KKM_WSTRI(c_closedReceipt, L"Чек закрыт");
        KKM_WSTRI(c_sellReceipt, L"Чек прихода");
        KKM_WSTRI(c_sellReturnReceipt, L"Чек возврата прихода");
        KKM_WSTRI(c_sellCorrectionReceipt, L"Чек коррекции прихода");
        KKM_WSTRI(c_sellReturnCorrectionReceipt, L"Чек коррекции возврата прихода");
        KKM_WSTRI(c_buyReceipt, L"Чек расхода");
        KKM_WSTRI(c_buyReturnReceipt, L"Чек возврата расхода");
        KKM_WSTRI(c_buyCorrectionReceipt, L"Чек коррекции расхода");
        KKM_WSTRI(c_buyReturnCorrectionReceipt, L"Чек коррекции возврата расхода");
        KKM_WSTRI(c_closedDocument, L"Документ закрыт");
        KKM_WSTRI(c_openShiftDocument, L"Документ открытия смены");
        KKM_WSTRI(c_closeShiftDocument, L"Документ закрытия смены");
        KKM_WSTRI(c_registrationDocument, L"Документ пере/регистрации");
        KKM_WSTRI(c_closeArchiveDocument, L"Документ закрытия архива ФН");
        KKM_WSTRI(c_ofdExchangeStatusDocument, L"Отчёт о состоянии расчётов");
        KKM_WSTRI(c_serviceDocument, L"Сервисный документ");
        KKM_WSTRI(c_documentCopy, L"Копия документа");

        inline const std::unordered_map<unsigned int, std::wstring_view> c_models {
            { Atol::LIBFPTR_MODEL_ALLIANCE_20F, L"АЛЬЯНС 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_1F, L"АТОЛ 1Ф" },
#if VERSION_LIMIT >= VERSION_10107
            { Atol::LIBFPTR_MODEL_ATOL_2F, L"АТОЛ 2Ф" },
#endif
            { Atol::LIBFPTR_MODEL_ATOL_11F, L"АТОЛ 11Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_15F, L"АТОЛ 15Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_20F, L"АТОЛ 20Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_22F, L"АТОЛ 22Ф (АТОЛ FPrint-22ПТК)" },
            { Atol::LIBFPTR_MODEL_ATOL_22V2F, L"АТОЛ 22 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_25F, L"АТОЛ 25Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27F, L"АТОЛ 27Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_27_FP7_F, L"АТОЛ 27 FP7 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_30F, L"АТОЛ 30Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_35F, L"АТОЛ 35Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_42FA, L"АТОЛ 42ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_42FS, L"АТОЛ 42ФС" },
            { Atol::LIBFPTR_MODEL_ATOL_47FA, L"АТОЛ 47ФА" },
            { Atol::LIBFPTR_MODEL_ATOL_50F, L"АТОЛ 50Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_52F, L"АТОЛ 52Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55F, L"АТОЛ 55Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_55V2F, L"АТОЛ 55 v2 Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_60F, L"АТОЛ 60Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_77F, L"АТОЛ 77Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_90F, L"АТОЛ 90Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_91F, L"АТОЛ 91Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_92F, L"АТОЛ 92Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_PT_5F, L"АТОЛ PT-5Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_10, L"АТОЛ Sigma 10" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_7F, L"АТОЛ Sigma 7Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_SIGMA_8F, L"АТОЛ Sigma 8Ф" },
            { Atol::LIBFPTR_MODEL_ATOL_STB_6F, L"АТОЛ СТБ 6Ф" },
            { Atol::LIBFPTR_MODEL_KAZNACHEY_FA, L"Казначей ФА" }
        };

#if VERSION_LIMIT >= VERSION_10107
        inline const std::unordered_map<std::wstring, TimeZone> c_timeZoneMap {
            { L"device", TimeZone::Device },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Device)), TimeZone::Device },
            { L"+2", TimeZone::Zone1 },
            { L"utc+2", TimeZone::Zone1 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone1)), TimeZone::Zone1 },
            { L"+3", TimeZone::Zone2 },
            { L"utc+3", TimeZone::Zone2 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone2)), TimeZone::Zone2 },
            { L"+4", TimeZone::Zone3 },
            { L"utc+4", TimeZone::Zone3 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone3)), TimeZone::Zone3 },
            { L"+5", TimeZone::Zone4 },
            { L"utc+5", TimeZone::Zone4 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone4)), TimeZone::Zone4 },
            { L"+6", TimeZone::Zone5 },
            { L"utc+6", TimeZone::Zone5 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone5)), TimeZone::Zone5 },
            { L"+7", TimeZone::Zone6 },
            { L"utc+7", TimeZone::Zone6 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone6)), TimeZone::Zone6 },
            { L"+8", TimeZone::Zone7 },
            { L"utc+8", TimeZone::Zone7 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone7)), TimeZone::Zone7 },
            { L"+9", TimeZone::Zone8 },
            { L"utc+9", TimeZone::Zone8 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone8)), TimeZone::Zone8 },
            { L"+10", TimeZone::Zone9 },
            { L"utc+10", TimeZone::Zone9 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone9)), TimeZone::Zone9 },
            { L"+11", TimeZone::Zone10 },
            { L"utc+11", TimeZone::Zone10 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone10)), TimeZone::Zone10 },
            { L"+12", TimeZone::Zone11 },
            { L"utc+12", TimeZone::Zone11 },
            { L"tz" + std::to_wstring(Meta::toUnderlying(TimeZone::Zone11)), TimeZone::Zone11 }
        };
#endif

        inline const std::unordered_map<FfdVersion, std::wstring_view> c_ffdVersions {
            { FfdVersion::Unknown, L"[неизвестная версия]" },
            { FfdVersion::V_1_0_5, L"1.0.5" },
            { FfdVersion::V_1_1, L"1.1" },
            { FfdVersion::V_1_2, L"1.2" }
        };

        inline const std::unordered_map<std::wstring, FfdVersion> c_ffdVersionsMap {
            { L"1.0.5", FfdVersion::V_1_0_5 },
            { L"1.1", FfdVersion::V_1_1 },
            { L"1.2", FfdVersion::V_1_2 }
        };

        inline const std::unordered_map<FfdVersionDetect, std::wstring_view> c_ffdVersionDetect {
            { FfdVersionDetect::Never, L"never" },
            { FfdVersionDetect::Once, L"once" },
            /*{ FfdVersionDetect::Sometimes, L"sometimes" },*/
            { FfdVersionDetect::Always, L"always" }
        };

        inline const std::unordered_map<std::wstring, FfdVersionDetect> c_ffdVersionDetectMap {
            { L"never", FfdVersionDetect::Never },
            { L"once", FfdVersionDetect::Once },
            /*{ L"sometimes", FfdVersionDetect::Sometimes },*/
            { L"always", FfdVersionDetect::Always }
        };

        inline const std::unordered_map<ShiftState, std::wstring_view> c_shiftStateLabels {
            { ShiftState::Closed, c_closedShift },
            { ShiftState::Opened, c_openedShift },
            { ShiftState::Expired, c_expiredShift }
        };

        inline const std::unordered_map<ReceiptType, std::wstring_view> c_receiptTypeLabels {
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

        inline const std::unordered_map<DocumentType, std::wstring_view> c_documentTypeLabels {
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

        inline const std::unordered_map<std::wstring, MeasurementUnit> c_measurementUnitMap {
            { L"piece", MeasurementUnit::Piece },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Piece)), MeasurementUnit::Piece },
            { L"gram", MeasurementUnit::Gram },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Gram)), MeasurementUnit::Gram },
            { L"kilogram", MeasurementUnit::Kilogram },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Kilogram)), MeasurementUnit::Kilogram },
            { L"ton", MeasurementUnit::Ton },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Ton)), MeasurementUnit::Ton },
            { L"centimeter", MeasurementUnit::Centimeter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Centimeter)), MeasurementUnit::Centimeter },
            { L"decimeter", MeasurementUnit::Decimeter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Decimeter)), MeasurementUnit::Decimeter },
            { L"meter", MeasurementUnit::Meter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Meter)), MeasurementUnit::Meter },
            { L"squarecentimeter", MeasurementUnit::SquareCentimeter },
            { L"square_centimeter", MeasurementUnit::SquareCentimeter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::SquareCentimeter)), MeasurementUnit::SquareCentimeter },
            { L"squaredecimeter", MeasurementUnit::SquareDecimeter },
            { L"square_decimeter", MeasurementUnit::SquareDecimeter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::SquareDecimeter)), MeasurementUnit::SquareDecimeter },
            { L"squaremeter", MeasurementUnit::SquareMeter },
            { L"square_meter", MeasurementUnit::SquareMeter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::SquareMeter)), MeasurementUnit::SquareMeter },
            { L"milliliter", MeasurementUnit::Milliliter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Milliliter)), MeasurementUnit::Milliliter },
            { L"liter", MeasurementUnit::Liter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Liter)), MeasurementUnit::Liter },
            { L"cubicmeter", MeasurementUnit::CubicMeter },
            { L"cubic_meter", MeasurementUnit::CubicMeter },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::CubicMeter)), MeasurementUnit::CubicMeter },
            { L"kilowatthour", MeasurementUnit::KilowattHour },
            { L"kilowatt_hour", MeasurementUnit::KilowattHour },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::KilowattHour)), MeasurementUnit::KilowattHour },
            { L"gkal", MeasurementUnit::Gkal },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Gkal)), MeasurementUnit::Gkal },
            { L"day", MeasurementUnit::Day },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Day)), MeasurementUnit::Day },
            { L"hour", MeasurementUnit::Hour },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Hour)), MeasurementUnit::Hour },
            { L"minute", MeasurementUnit::Minute },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Minute)), MeasurementUnit::Minute },
            { L"second", MeasurementUnit::Second },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Second)), MeasurementUnit::Second },
            { L"kilobyte", MeasurementUnit::Kilobyte },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Kilobyte)), MeasurementUnit::Kilobyte },
            { L"megabyte", MeasurementUnit::Megabyte },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Megabyte)), MeasurementUnit::Megabyte },
            { L"gigabyte", MeasurementUnit::Gigabyte },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Gigabyte)), MeasurementUnit::Gigabyte },
            { L"terabyte", MeasurementUnit::Terabyte },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Terabyte)), MeasurementUnit::Terabyte },
            { L"other", MeasurementUnit::Other },
            { std::to_wstring(Meta::toUnderlying(MeasurementUnit::Other)), MeasurementUnit::Other }
        };

        inline const std::unordered_map<std::wstring, Tax> c_taxCastMap {
            { L"no", Tax::No },
            { std::to_wstring(Meta::toUnderlying(Tax::No)), Tax::No },
            { L"0%", Tax::Vat0 },
            { L"vat0", Tax::Vat0 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat0)), Tax::Vat0 },
            { L"5%", Tax::Vat5 },
            { L"vat5", Tax::Vat5 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat5)), Tax::Vat5 },
            { L"105%", Tax::Vat105 },
            { L"vat105", Tax::Vat105 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat105)), Tax::Vat105 },
            { L"7%", Tax::Vat7 },
            { L"vat7", Tax::Vat7 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat7)), Tax::Vat7 },
            { L"107%", Tax::Vat107 },
            { L"vat107", Tax::Vat107 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat107)), Tax::Vat107 },
            { L"10%", Tax::Vat10 },
            { L"vat10", Tax::Vat10 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat10)), Tax::Vat10 },
            { L"110%", Tax::Vat110 },
            { L"vat110", Tax::Vat110 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat110)), Tax::Vat110 },
            { L"20%", Tax::Vat20 },
            { L"vat20", Tax::Vat20 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat20)), Tax::Vat20 },
            { L"120%", Tax::Vat120 },
            { L"vat120", Tax::Vat120 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat120)), Tax::Vat120 },
#if VERSION_LIMIT >= VERSION_10108
            { L"22%", Tax::Vat22 },
            { L"vat22", Tax::Vat22 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat22)), Tax::Vat22 },
            { L"122%", Tax::Vat122 },
            { L"vat122", Tax::Vat122 },
            { std::to_wstring(Meta::toUnderlying(Tax::Vat122)), Tax::Vat122 },
#endif
        };

        inline const std::unordered_map<std::wstring, PaymentType> c_paymentTypeCastMap {
            { L"cash", PaymentType::Cash },
            { std::to_wstring(Meta::toUnderlying(PaymentType::Cash)), PaymentType::Cash },
            { L"electronically", PaymentType::Electronically },
            { std::to_wstring(Meta::toUnderlying(PaymentType::Electronically)), PaymentType::Electronically }
        };
    }
}
