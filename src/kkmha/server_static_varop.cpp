// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_static_varop.h"
#include "server_static_variables.h"
#include "server_static_strings.h"
#include "server_except.h"
#include "server_json.h"
#include <lib/path.h>

namespace Server::Static {
    using namespace std::string_literals;

    void setVars(const JsonVal & json) {
        Json::handleKey(
            json, L"server"_key,
            [] (const JsonVal & json2, const JsonPtr jptr2) -> bool {
                Json::handleKey(json2, L"enableStatic"_key, s_enable, jptr2);
                Json::handleKey(
                    json2, L"staticDirectory"_key, s_directory,
                    Path::testDir(s_enable, Path::absolute(Path::noEmpty())), jptr2
                );
                Json::handleKey(json2, L"indexFile"_key, s_indexFile, Path::Mbs::goodFileName(), jptr2);
                Json::handleKey(
                    json2, L"mimeMap"_key, s_mimeMapFile,
                    Path::testFile(s_enable, Path::absolute(Path::noEmpty())), jptr2
                );
                Json::handleKey(json2, L"enableUnknownType"_key, s_enableUnknownType, jptr2);
                return true;
            }
        );
    }

    void setMimeMap(const JsonVal & json) {
        if (!json.IsObject()) {
            throw Failure(Wcs::c_invalidMimeMap);
        }
        for (JsonVal::ConstMemberIterator it = json.MemberBegin(); it != json.MemberEnd(); ++it) {
            if (!it->name.IsString() || it->name.GetStringLength() < 1) {
                throw Failure(Wcs::c_invalidMimeMap);
            }
            if (!it->value.IsString() || it->value.GetStringLength() < 3) {
                throw Failure(Wcs::c_invalidMimeMap);
            }
            auto ext = Text::lowered(Text::trimmed(it->name.GetString()));
            auto type = Text::lowered(Text::convert(it->value.GetString()));
            std::ranges::replace_if(type, [] (const char c) { return c == 0xa || c == 0xd; }, ' ');
            Text::trim(type);
            if (ext.empty() || type.empty()) {
                throw Failure(Wcs::c_invalidMimeMap);
            }
            if (ext[0] != '.') {
                ext.insert(0, 1, L'.');
            }
            s_mimeMap[std::move(ext)] = std::move(type);
        }
    }

    std::wostream & vars(std::wostream & stream) {
        stream
            << L"CFG: server.enableStatic = " << Text::Wcs::yesNo(s_enable) << L"\n"
            L"CFG: server.staticDirectory = \"" << s_directory.wstring() << L"\"\n"
            L"CFG: server.indexFile = \"" << Text::convert(s_indexFile) << L"\"\n"
            L"CFG: server.mimeMapFile = \"" << s_mimeMapFile.wstring() << L"\"\n"
            L"CFG: server.enableUnknownType = " << Text::Wcs::yesNo(s_enableUnknownType) << L"\n";

        return stream;
    }
}
