// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/meta.h>
#include <lib/winapi.h>
#include <elr/messages.h>

namespace Log {
    enum class Category : unsigned short {
        Generic = CATEGORY_GENERIC,
        Configuration = CATEGORY_CONFIGURATION,
        Service = CATEGORY_SERVICE,
        Network = CATEGORY_NETWORK,
        WebServer = CATEGORY_WEB_SERVER,
        CashRegister = CATEGORY_CASH_REGISTER
    };

    using CategoryUnderlying = std::underlying_type_t<Category>;

    constexpr CategoryUnderlying c_catUnknown [[maybe_unused]] { 0 };
    constexpr CategoryUnderlying c_catGeneric [[maybe_unused]] { Meta::toUnderlying(Category::Generic) };
    constexpr CategoryUnderlying c_catConfiguration [[maybe_unused]] { Meta::toUnderlying(Category::Configuration) };
    constexpr CategoryUnderlying c_catService [[maybe_unused]] { Meta::toUnderlying(Category::Service) };
    constexpr CategoryUnderlying c_catNetwork [[maybe_unused]] { Meta::toUnderlying(Category::Network) };
    constexpr CategoryUnderlying c_catWebServer [[maybe_unused]] { Meta::toUnderlying(Category::WebServer) };
    constexpr CategoryUnderlying c_catCashRegister [[maybe_unused]] { Meta::toUnderlying(Category::CashRegister) };

    enum class Level {
        Debug = 0,
        Info = 1,
        Warning = 2,
        Error = 3
    };

    using LevelUnderlying = std::underlying_type_t<Level>;

    constexpr LevelUnderlying c_levelDebug [[maybe_unused]] { Meta::toUnderlying(Level::Debug) };
    constexpr LevelUnderlying c_levelInfo [[maybe_unused]] { Meta::toUnderlying(Level::Info) };
    constexpr LevelUnderlying c_levelWarning [[maybe_unused]] { Meta::toUnderlying(Level::Warning) };
    constexpr LevelUnderlying c_levelError [[maybe_unused]] { Meta::toUnderlying(Level::Error) };
    constexpr LevelUnderlying c_levelNone [[maybe_unused]] { c_levelError + 1 };

    namespace Console {
        enum class Output { Separated, StdOut, StdErr };
    }
}
