// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "variables.h"
#include "strings.h"
#include "core.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>
#include <lib/json.h>
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
                            Json::handleKey(json3, "terse", Console::s_terse, path3);
                            Json::handleKey(json3, "output", Console::s_output, Wcs::c_outputCastMap, path3);
                            Json::handleKey(json3, "flushEveryWrite", Console::s_flushEveryWrite, path3);
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
                            Json::handleKey(json3, "flushEveryWrite", File::s_flushEveryWrite, path3);
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
                    Json::handleKey(
                        json2, "lineSize", s_lineSize,
                        Numeric::between(c_lineSizeMin, c_lineSizeMax), path2
                    );
                    Json::handleKey(
                        json2, "maxQueueBlocks", s_blocksNumber,
                        Numeric::between(c_blocksNumberMin, c_blocksNumberMax), path2
                    );
                    Json::handleKey(json2, "enableAsync", s_enableAsync, path2);
                    return true;
                }
            )
        };
        if (found) {
            Config::reinitLogger();
        }
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"CFG: log.console.level.foreground = " << levelLabel(Console::s_level) << L"\n"
            L"CFG: log.console.terse = " << Text::Wcs::yesNo(Console::s_terse) << L"\n"
            L"CFG: log.console.output = " << Wcs::c_outputLabels.at(Console::s_output) << L"\n"
            L"CFG: log.console.flushEveryWrite = " << Text::Wcs::yesNo(Console::s_flushEveryWrite) << L"\n"
            L"CFG: log.file.level.foreground = " << levelLabel(File::s_fgLevel) << L"\n"
            L"CFG: log.file.level.background = " << levelLabel(File::s_bgLevel) << L"\n"
            L"CFG: log.file.directory = \"" << File::s_directory.wstring() << L"\"\n"
            L"CFG: log.file.flushEveryWrite = " << Text::Wcs::yesNo(File::s_flushEveryWrite) << L"\n"
            L"CFG: log.eventLog.level.foreground = " << levelLabel(EventLog::s_fgLevel) << L"\n"
            L"CFG: log.eventLog.level.background = " << levelLabel(EventLog::s_bgLevel) << L"\n"
            L"DEF: log.eventLog.source = \"" << EventLog::c_eventSource << L"\"\n"
            L"CFG: log.appendLocation = " << Text::Wcs::yesNo(s_appendLocation) << L"\n"
            L"CFG: log.lineSize = " << s_lineSize << L"\n"
            L"CFG: log.enableAsync = " << Text::Wcs::yesNo(s_enableAsync) << L"\n"
            L"DEF: log.queueBlockSize = " << c_blockSize << L"\n"
            L"CFG: log.maxQueueBlocks = " << s_blocksNumber << L"\n";

        return stream;
    }
}
