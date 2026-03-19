// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "defaults.h"
#include "variables.h"
#include "strings.h"
#include "core.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>
#include <main/variables.h>

namespace Log {
    void setVars(const Nln::Json & json) {
        const bool found {
            Json::handleKey(
                json, "log",
                [] (const Nln::Json & json2, const std::wstring & path2) -> bool {
                    Json::handleKey(
                        json2, "console",
                        [] (const Nln::Json & json3, const std::wstring & path3) -> bool {
                            Json::handleKey(
                                json3, "level",
                                [] (const Nln::Json & json4, const std::wstring & path4) -> bool {
                                    if (json4.is_object()) {
                                        Json::handleKey(json4, "foreground", Console::s_level, Wcs::c_levelCastMap, path4);
                                    } else {
                                        Json::handle(json4, Console::s_level, Wcs::c_levelCastMap, path4);
                                    }
                                    return true;
                                },
                                path3
                            );
                            Json::handleKey(json3, "outputTimestamp", Console::s_outputTimestamp, path3);
                            Json::handleKey(json3, "outputLevel", Console::s_outputLevel, path3);
                            return true;
                        },
                        path2
                    );
                    Json::handleKey(
                        json2, "file",
                        [] (const Nln::Json & json3, const std::wstring & path3) -> bool {
                            Json::handleKey(
                                json3, "level",
                                [] (const Nln::Json & json4, const std::wstring & path4) -> bool {
                                    if (json4.is_object()) {
                                        Json::handleKey(json4, "foreground", File::s_fgLevel, Wcs::c_levelCastMap, path4);
                                        Json::handleKey(json4, "background", File::s_bgLevel, Wcs::c_levelCastMap, path4);
                                    } else {
                                        LevelUnderlying level { c_levelNone };
                                        if (Json::handle(json4, level, Wcs::c_levelCastMap, path4)) {
                                            File::s_fgLevel = level;
                                            File::s_bgLevel = level;
                                        }
                                    }
                                    return true;
                                },
                                path3
                            );
                            Json::handleKey(
                                json3, "directory", File::s_directory,
                                Path::touchDir(Path::absolute(Main::s_directory, Path::noEmpty())), path3
                            );
                            return true;
                        },
                        path2
                    );
                    Json::handleKey(
                        json2, "eventLog",
                        [] (const Nln::Json & json3, const std::wstring & path3) -> bool {
                            Json::handleKey(
                                json3, "level",
                                [] (const Nln::Json & json4, const std::wstring & path4) -> bool {
                                    if (json4.is_object()) {
                                        Json::handleKey(
                                            json4, "foreground", EventLog::s_fgLevel,
                                            Wcs::c_levelCastMap, Numeric::min(c_levelInfo), path4
                                        );
                                        Json::handleKey(
                                            json4, "background", EventLog::s_bgLevel,
                                            Wcs::c_levelCastMap, Numeric::min(c_levelInfo), path4
                                        );
                                    } else {
                                        LevelUnderlying level { c_levelNone };
                                        const bool found4 {
                                            Json::handle(
                                                json4, level, Wcs::c_levelCastMap, Numeric::min(c_levelInfo), path4
                                            )
                                        };
                                        if (found4) {
                                            EventLog::s_fgLevel = level;
                                            EventLog::s_bgLevel = level;
                                        }
                                    }
                                    return true;
                                },
                                path3
                            );
                            return true;
                        },
                        path2
                    );
                    Json::handleKey(json2, "appendLocation", s_appendLocation, path2);
                    return true;
                }
            )
        };
        if (found) {
            reconfig();
        }
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"CFG: log.console.level.foreground = " << levelLabel(Console::s_level) << L"\n"
            L"CFG: log.console.outputTimestamp = " << Text::Wcs::yesNo(Console::s_outputTimestamp) << L"\n"
            L"CFG: log.console.outputLevel = " << Text::Wcs::yesNo(Console::s_outputLevel) << L"\n"
            L"CFG: log.file.level.foreground = " << levelLabel(File::s_fgLevel) << L"\n"
            L"CFG: log.file.level.background = " << levelLabel(File::s_bgLevel) << L"\n"
            L"CFG: log.file.directory = \"" << File::s_directory.wstring() << L"\"\n"
            L"CFG: log.eventLog.level.foreground = " << levelLabel(EventLog::s_fgLevel) << L"\n"
            L"CFG: log.eventLog.level.background = " << levelLabel(EventLog::s_bgLevel) << L"\n"
            L"DEF: log.eventLog.source = \"" << c_eventSource << L"\"\n"
            L"CFG: log.appendLocation = " << Text::Wcs::yesNo(s_appendLocation) << L"\n";

        return stream;
    }
}
