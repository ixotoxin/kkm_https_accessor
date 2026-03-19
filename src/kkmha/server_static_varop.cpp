// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_static_varop.h"
#include "server_static_variables.h"
#include "server_static_strings.h"
#include <lib/path.h>
#include <algorithm>

namespace Server::Static {
    using namespace std::string_literals;

    using Basic::Failure;

    void setVars(const Nln::Json & json) {
        Json::handleKey(
            json, "server",
            [] (const Nln::Json & json2, const std::wstring & path2) -> bool {
                Json::handleKey(json2, "enableStatic", s_enable, path2);
                Json::handleKey(
                    json2, "staticDirectory", s_directory,
                    Path::testDir(s_enable, Path::absolute(Path::noEmpty())), path2
                );
                Json::handleKey(json2, "indexFile", s_indexFile, Path::Mbs::goodFileName(), path2);
                Json::handleKey(
                    json2, "mimeMap", s_mimeMapFile,
                    Path::testFile(s_enable, Path::absolute(Path::noEmpty())), path2
                );
                Json::handleKey(json2, "enableUnknownType", s_enableUnknownType, path2);
                return true;
            }
        );
    }

    void setMimeMap(const Nln::Json & json) {
        if (!json.is_object()) {
            throw Failure(Wcs::c_incorrectStructure); // NOLINT(*-exception-baseclass)
        }
        for (const auto & [key, value] : json.items()) {
            if (!value.is_string()) {
                throw Failure(Wcs::c_incorrectStructure); // NOLINT(*-exception-baseclass)
            }
            auto ext = Text::lowered(key);
            std::ranges::replace_if(ext, [] (const char c) { return c == 0xa || c == 0xd; }, ' ');
            Text::trim(ext);
            auto type = Text::lowered(value.get<std::string>());
            std::ranges::replace_if(type, [] (const char c) { return c == 0xa || c == 0xd; }, ' ');
            Text::trim(type);
            if (ext.empty() || type.empty()) {
                throw Failure(Wcs::c_incorrectStructure); // NOLINT(*-exception-baseclass)
            }
            if (ext[0] != '.') {
                ext = "."s + ext; // NOLINT(*-inefficient-string-concatenation)
            }
            s_mimeMap[ext] = std::move(type);
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
