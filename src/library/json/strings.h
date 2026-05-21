// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string>

namespace Json::Wcs {
    using Csv = const std::wstring_view;

    constexpr Csv c_allocatorsPoolInitFailed { L"Не удалось инициализировать пул аллокаторов для Json-объектов" };
    constexpr Csv c_cannotAcquireAllocator { L"Не удалось получить аллокатор для Json-объекта" };

    constexpr Csv c_isNotObject { L"JSON не является объектом" };
    constexpr Csv c_isNotArray { L"JSON не является массивом" };
    constexpr Csv c_arrayWrongLength { L"JSON массив имеет неверный размер" };
    constexpr Csv c_keyNotFound { L"Ключ JSON объекта не найден" };
    constexpr Csv c_keyExists { L"Ключ JSON объекта уже существует" };
    constexpr Csv c_couldntSerialize { L"Не удалось сериализовать JSON" };
    constexpr Csv c_requiresProperty { L"Требуется корректное свойство JSON объекта '{}'" };
    constexpr Csv c_requiresProperty2 { L"Требуется корректное свойство JSON объекта '{}/{}'" };

    constexpr Csv c_parseErrorNone {
        // ISSUE: а нужно ли это сообщение?
        L"RapidJSON: Без ошибки (смещение {})"
    };

    constexpr Csv c_parseErrorDocumentEmpty {
        L"RapidJSON: Документ пуст (смещение {})"
    };

    constexpr Csv c_parseErrorDocumentRootNotSingular {
        L"RapidJSON: Корень документа не должен сопровождаться другими значениями (смещение {})"
    };

    constexpr Csv c_parseErrorValueInvalid {
        L"RapidJSON: Недопустимое значение (смещение {})"
    };

    constexpr Csv c_parseErrorObjectMissName {
        L"RapidJSON: Отсутствует имя для элемента объекта (смещение {})"
    };

    constexpr Csv c_parseErrorObjectMissColon {
        L"RapidJSON: Отсутствует двоеточие после имени элемента объекта (смещение {})"
    };

    constexpr Csv c_parseErrorObjectMissCommaOrCurlyBracket {
        L"RapidJSON: Отсутствует запятая или '}}' после элемента объекта (смещение {})"
    };

    constexpr Csv c_parseErrorArrayMissCommaOrSquareBracket {
        L"RapidJSON: Отсутствует запятая или ']' после элемента массива (смещение {})"
    };

    constexpr Csv c_parseErrorStringUnicodeEscapeInvalidHex {
        L"RapidJSON: Неверная шестнадцатеричная цифра после \\u в строке (смещение {})"
    };

    constexpr Csv c_parseErrorStringUnicodeSurrogateInvalid {
        L"RapidJSON: Неверная суррогатная пара в строке (смещение {})"
    };

    constexpr Csv c_parseErrorStringEscapeInvalid {
        L"RapidJSON: Неверный escape-символ в строке (смещение {})"
    };

    constexpr Csv c_parseErrorStringMissQuotationMark {
        L"RapidJSON: Отсутствует закрывающая кавычка в строке (смещение {})"
    };

    constexpr Csv c_parseErrorStringInvalidEncoding {
        L"RapidJSON: Неверная кодировка в строке (смещение {})"
    };

    constexpr Csv c_parseErrorNumberTooBig {
        L"RapidJSON: Число слишком велико для хранения в типе double (смещение {})"
    };

    constexpr Csv c_parseErrorNumberMissFraction {
        L"RapidJSON: Отсутствует дробная часть в числе (смещение {})"
    };

    constexpr Csv c_parseErrorNumberMissExponent {
        L"RapidJSON: Отсутствует экспонента в числе (смещение {})"
    };

    constexpr Csv c_parseErrorTermination {
        L"RapidJSON: Остановка разбора из-за ошибки обработчика (смещение {})"
    };

    constexpr Csv c_parseErrorUnspecificSyntaxError {
        L"RapidJSON: Неспецифическая синтаксическая ошибка (смещение {})"
    };

    constexpr Csv c_parseUnknownError {
        L"RapidJSON: Неизвестная ошибка (смещение {})"
    };
}
