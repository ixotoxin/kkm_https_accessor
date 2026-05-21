// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "varop.h"
#include "variables.h"
#include "strings.h"
#include "core.h"
#include "json.h"
#include <lib/numeric.h>
#include <lib/text.h>
#include <lib/path.h>
#include <main/variables.h>

namespace Log {
    void setVars(const JsonVal & json) {
        const bool found {
            Json::handleKey(
                json, L"log"_key,
                [] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                    Json::handleKey(
                        json2, L"console"_key,
                        [] (const JsonVal & json3, const JsonPtr jptr3) -> bool {
                            Json::handleKey(
                                json3, L"level"_key,
                                [] (const JsonVal & json4, const JsonPtr jptr4) -> bool {
                                    if (json4.IsObject()) {
                                        Json::handleKey(json4, L"foreground"_key, Console::s_level, Wcs::c_levelCastMap, jptr4);
                                    } else {
                                        Json::handle(json4, Console::s_level, Wcs::c_levelCastMap, jptr4);
                                    }
                                    return true;
                                },
                                jptr3
                            );
                            Json::handleKey(json3, L"terse"_key, Console::s_terse, jptr3);
                            Json::handleKey(json3, L"output"_key, Console::s_output, Wcs::c_outputCastMap, jptr3);
                            Json::handleKey(json3, L"flushEveryWrite"_key, Console::s_flushEveryWrite, jptr3);
                            return true;
                        },
                        jptr2
                    );
                    Json::handleKey(
                        json2, L"file"_key,
                        [] (const JsonVal & json3, const JsonPtr jptr3) -> bool {
                            Json::handleKey(
                                json3, L"level"_key,
                                [] (const JsonVal & json4, const JsonPtr jptr4) -> bool {
                                    if (json4.IsObject()) {
                                        Json::handleKey(json4, L"foreground"_key, File::s_fgLevel, Wcs::c_levelCastMap, jptr4);
                                        Json::handleKey(json4, L"background"_key, File::s_bgLevel, Wcs::c_levelCastMap, jptr4);
                                    } else {
                                        LevelUnderlying level { c_levelNone };
                                        if (Json::handle(json4, level, Wcs::c_levelCastMap, jptr4)) {
                                            File::s_fgLevel = level;
                                            File::s_bgLevel = level;
                                        }
                                    }
                                    return true;
                                },
                                jptr3
                            );
                            Json::handleKey(
                                json3, L"directory"_key, File::s_directory,
                                Path::touchDir(Path::absolute(Main::s_directory, Path::noEmpty())), jptr3
                            );
                            Json::handleKey(json3, L"flushEveryWrite"_key, File::s_flushEveryWrite, jptr3);
                            return true;
                        },
                        jptr2
                    );
                    Json::handleKey(
                        json2, L"eventLog"_key,
                        [] (const JsonVal & json3, const JsonPtr jptr3) -> bool {
                            Json::handleKey(
                                json3, L"level"_key,
                                [] (const JsonVal & json4, const JsonPtr jptr4) -> bool {
                                    if (json4.IsObject()) {
                                        Json::handleKey(
                                            json4, L"foreground"_key, EventLog::s_fgLevel,
                                            Wcs::c_levelCastMap, Numeric::min(c_levelInfo), jptr4
                                        );
                                        Json::handleKey(
                                            json4, L"background"_key, EventLog::s_bgLevel,
                                            Wcs::c_levelCastMap, Numeric::min(c_levelInfo), jptr4
                                        );
                                    } else {
                                        LevelUnderlying level { c_levelNone };
                                        const bool found4 {
                                            Json::handle(
                                                json4, level, Wcs::c_levelCastMap, Numeric::min(c_levelInfo), jptr4
                                            )
                                        };
                                        if (found4) {
                                            EventLog::s_fgLevel = level;
                                            EventLog::s_bgLevel = level;
                                        }
                                    }
                                    return true;
                                },
                                jptr3
                            );
                            return true;
                        },
                        jptr2
                    );
                    Json::handleKey(json2, L"appendLocation"_key, s_appendLocation, jptr2);
                    Json::handleKey(
                        json2, L"lineSize"_key, s_lineSize,
                        Numeric::between(c_lineSizeMin, c_lineSizeMax), jptr2
                    );
#ifndef SINGLE_THREAD
                    Json::handleKey(
                        json2, L"maxQueueBlocks"_key, s_blocksNumber,
                        Numeric::between(c_blocksNumberMin, c_blocksNumberMax), jptr2
                    );
                    Json::handleKey(json2, L"enableAsync"_key, s_enableAsync, jptr2);
#endif
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
#ifndef SINGLE_THREAD
            L"CFG: log.enableAsync = " << Text::Wcs::yesNo(s_enableAsync) << L"\n"
            L"DEF: log.queueBlockSize = " << c_blockSize << L"\n"
            L"CFG: log.maxQueueBlocks = " << s_blocksNumber << L"\n"
#endif
            ;

        return stream;
    }
}
