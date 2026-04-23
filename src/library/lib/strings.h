// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "macro.h"
#include "meta.h"
#include <string_view>

namespace Basic {
    namespace Wcs {
        LIB_WSTRI(c_ok, L"OK");
        LIB_WSTRI(c_somethingWrong, L"Что-то пошло не так!");
        LIB_WSTRI(c_fallbackErrorMessage, L"[ error ]");
        LIB_WSTRI(c_invalidValue, L"Недопустимое значение");
        LIB_WSTRI(c_rangeError, L"Значение вне диапазона");
        LIB_WSTRI(c_dataError, L"{0} (свойство '{1}')");
        LIB_WSTRI(c_source, L"источник");
        LIB_WSTRI(c_directoryNotFound, L"Директория '{}' не найдена");
        LIB_WSTRI(c_fileNotFound, L"Файл '{}' не найден");
        LIB_WSTRI(c_couldntReadFile, L"Не удалось прочитать файл '{}'");
        LIB_WSTRI(c_couldntWriteFile, L"Не удалось записать файл '{}'");
    }

    namespace Mbs {
        LIB_STRI(c_ok, "OK");
        LIB_STRI(c_somethingWrong, "Что-то пошло не так!");
        LIB_STRI(c_fallbackErrorMessage, "[ error ]");
        // LIB_STRI(c_source, "источник"); // CLEANUP
    }
}

namespace Meta {
    template<>
    struct BoolLabels<std::wstring_view, DaNet> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"Да" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"Нет" };
    };

    template<>
    struct BoolLabels<std::string_view, DaNet> {
        static constexpr std::string_view c_true [[maybe_unused]] { "Да" };
        static constexpr std::string_view c_false [[maybe_unused]] { "Нет" };
    };

    template<>
    struct BoolLabels<std::wstring_view, YesNo> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"yes" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"no" };
    };

    template<>
    struct BoolLabels<std::string_view, YesNo> {
        static constexpr std::string_view c_true [[maybe_unused]] { "yes" };
        static constexpr std::string_view c_false [[maybe_unused]] { "no" };
    };

    template<>
    struct BoolLabels<std::wstring_view, EnaDis> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"enable" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"disable" };
    };

    template<>
    struct BoolLabels<std::string_view, EnaDis> {
        static constexpr std::string_view c_true [[maybe_unused]] { "enable" };
        static constexpr std::string_view c_false [[maybe_unused]] { "disable" };
    };

    template<>
    struct BoolLabels<std::wstring_view, TrueFalse> {
        static constexpr std::wstring_view c_true [[maybe_unused]] { L"true" };
        static constexpr std::wstring_view c_false [[maybe_unused]] { L"false" };
    };

    template<>
    struct BoolLabels<std::string_view, TrueFalse> {
        static constexpr std::string_view c_true [[maybe_unused]] { "true" };
        static constexpr std::string_view c_false [[maybe_unused]] { "false" };
    };
}

namespace Json {
    namespace Wcs {
        LIB_WSTRI(c_jsonObjectWasExpected, L"Ожидался JSON-объект");
        LIB_WSTRI(c_successKey, L"!success");
        LIB_WSTRI(c_messageKey, L"!message");
    }

    namespace Mbs {
        LIB_STRI(c_successKey, "!success");
        LIB_STRI(c_messageKey, "!message");
    }
}
