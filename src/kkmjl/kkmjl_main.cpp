// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include <cmake/variables.h>
#include <lib/setcli.h>
#include <log2/varop.h>
#include <log2/core.h>
#include <debug/memprof.h>
#include <kkm/varop.h>
#include <config/core.h>
#include <cstdlib>
#include <iostream>
#include "kkmjl_core.h"

int wmain(const int argc, wchar_t ** argv, wchar_t ** envp) {
    Config::initConsole(Config::c_u16Text);
    Config::initLogger();
    Config::initProfiler();

    try {
        if (argc == 3) {
            Config::setBaseVars(envp);
            Config::readJson(Config::s_file, Log::setVars, Kkm::setVars);
            return KkmJsonLoader::exec(argv[1], argv[2]);
        }
        std::wcerr
            << L"{\n"
#if WITH_CRTD || WITH_SNTZ
            L"    \"" MEMORY_PROFILING_FLAG_KEY "\": \"" MEMORY_PROFILING_FLAG_VALUE "\",\n"
#endif
            L"    \"" << Json::Wcs::c_successKey << L"\": false,\n"
            L"    \"" << Json::Wcs::c_messageKey << L"\": \"Неверное использование\",\n"
            L"    \"!version\": \"" << BUILD_VERSION << L"\",\n"
            L"    \"!usage\": \"" << Json::escapeBasic(argv[0]) << L" {сн} {вф}\"\n"
            L"}";
    } catch (const Basic::Failure & e) {
        KkmJsonLoader::printError(e.explain()); // TODO: Подумать, есть ли необходимость в выводе источника исключения
    } catch (const std::exception & e) {
        KkmJsonLoader::printError(Text::convert(e.what()));
    } catch (...) {
        KkmJsonLoader::printError(Basic::Wcs::c_somethingWrong);
    }

    return EXIT_FAILURE;
}
