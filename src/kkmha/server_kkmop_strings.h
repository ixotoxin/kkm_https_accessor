// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <string_view>

namespace Server::KkmOp {
    namespace Wcs {
        using Csv = const std::wstring_view;

        constexpr Csv c_requestPrefix { L"Запрос [{:04x}]: " };
        constexpr Csv c_selectKkm { L"Выбрана ККМ [{}] (параметры подключения: {})" };
        constexpr Csv c_getKkmInfo { L"Получение информации об устройстве" };
        constexpr Csv c_connParamsSaved { L"Параметры подключения ККМ [{}] успешно сохранены" };
    }

    namespace Mbs {
        using Csv = const std::string_view;

        constexpr Csv c_notFound { "ККМ [{}] не доступна" };
        constexpr Csv c_cantClearRegistry { "Не удалось очистить реестр параметров подключения" };
    }
}
