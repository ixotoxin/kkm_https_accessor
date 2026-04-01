// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "variables.h"
#include "strings.h"
#include "except.h"
#include "logger.h"
#include <lib/json.h>
#include <main/variables.h>
#include <concepts>
#include <fstream>
#include <filesystem>
#include <string>
#include <format>

namespace Config {
    void setBaseVars(wchar_t ** envp);

    template<typename T, std::same_as<void (*)(const Nln::Json &)> ... SETTERS>
    requires std::is_same_v<T, std::filesystem::path> || std::is_same_v<T, std::wstring>
    void readJson(const T & file, SETTERS ... setters) {
        if (!std::filesystem::is_regular_file(file)) {
            throw Failure(CONF_WFMT(Wcs::c_cantReadConfig, file.wstring())); // NOLINT(*-exception-baseclass)
        }
        try {
            std::filesystem::current_path(Config::s_directory);
            Nln::Json json(Nln::Json::parse(std::ifstream(file)));
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
        throw Failure(Wcs::c_invalidConfig); // NOLINT(*-exception-baseclass)
    }
}
