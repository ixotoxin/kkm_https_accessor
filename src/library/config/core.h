// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "variables.h"
#include "strings.h"
#include "except.h"
#include "json.h"
#include "logger.h"
#include <main/variables.h>
#include <concepts>
#include <filesystem>

namespace Config {
    void setBaseVars(wchar_t ** envp);

    template<std::same_as<void (*)(const JsonVal &)> ... Setters>
    void readJson(const std::filesystem::path & file, Setters ... setters) {
        try {
            std::filesystem::current_path(Config::s_directory);
            JsonDoc json {};
            json <<= file;
            (setters(json), ...);
            std::filesystem::current_path(Main::s_directory);
            return;
        } catch (Failure & e) {
            log(Log::Level::Warning, e);
        } catch (const std::exception & e) {
            log(Log::Level::Warning, e.what());
        } catch (...) {
            log(Log::Level::Warning, Basic::Wcs::c_somethingWrong);
        }

        std::filesystem::current_path(Main::s_directory);
        throw Failure(Wcs::c_invalidConfig);
    }
}
