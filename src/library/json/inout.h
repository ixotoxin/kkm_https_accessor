// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "error.h"
#include "types.h"
#include "except.h"
#include <lib/except.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <filesystem>
#include <fstream>

namespace Json {
    template<Meta::JsonDocument J, Meta::String S>
    void read(J & json, const S & input) {
        if constexpr (std::is_same_v<typename Trait<J>::Encoding, typename Trait<S>::Encoding>) {
            json.Parse(input.c_str());
        } else {
            using SourceEncoding = Trait<S>::Encoding;
            using SourceStreamL1 = rapidjson::MemoryStream;
            using SourceStreamL2 = rapidjson::EncodedInputStream<SourceEncoding, SourceStreamL1>;
            SourceStreamL1 streamL1 { input.c_str(), input.size() };
            SourceStreamL2 streamL2 { streamL1 };
            json.template ParseStream<0, SourceEncoding, SourceStreamL2>(streamL2);
        }
        if (json.HasParseError()) {
            throw Failure(Fmt(Wcs::getParseError(json.GetParseError()), json.GetErrorOffset()));
        }
    }

    template<Meta::JsonDocument J>
    void read(J & json, const std::filesystem::path & path) {
        if (!std::filesystem::is_regular_file(path)) {
            throw Failure(Fmt(Basic::Wcs::c_couldntReadFile, path.filename().wstring()));
        }
        std::ifstream file(path, std::ios::binary);
        if (!file.is_open()) {
            throw Failure(Fmt(Basic::Wcs::c_couldntReadFile, path.filename().wstring()));
        }
        using SourceEncoding = rapidjson::UTF8<>;
        using SourceStreamL1 = rapidjson::IStreamWrapper;
        using SourceStreamL2 = rapidjson::EncodedInputStream<SourceEncoding, SourceStreamL1>;
        SourceStreamL1 streamL1 { file };
        SourceStreamL2 streamL2 { streamL1 };
        json.template ParseStream<0, SourceEncoding, SourceStreamL2>(streamL2);
        if (json.HasParseError()) {
            throw Failure(Fmt(Wcs::getParseError(json.GetParseError()), json.GetErrorOffset()));
        }
    }

    template<Meta::JsonDocument J, Meta::String S>
    void write(const J & json, S & output) {
        using SourceEncoding = Trait<J>::Encoding;
        using TargetEncoding = Trait<S>::Encoding;
        using TargetStream = Trait<S>::StringOutputStream;
#ifdef DEBUG
        using Writer = rapidjson::PrettyWriter<TargetStream, SourceEncoding, TargetEncoding>;
#else
        using Writer = rapidjson::Writer<TargetStream, SourceEncoding, TargetEncoding>;
#endif
        TargetStream stream { output };
        Writer writer { stream };
#ifdef DEBUG
        writer.SetIndent(' ', 4);
#endif
        output.clear();
        if (!json.Accept(writer)) {
            throw Failure(Wcs::c_couldntSerialize);
        }
    }

    template<Meta::JsonDocument J>
    void write(const J & json, const std::filesystem::path & path) {
        using SourceEncoding = Trait<J>::Encoding;
        using TargetEncoding = rapidjson::UTF8<>;
        using TargetStream = rapidjson::OStreamWrapper;
#ifdef DEBUG
        using Writer = rapidjson::PrettyWriter<TargetStream, SourceEncoding, TargetEncoding>;
#else
        using Writer = rapidjson::Writer<TargetStream, SourceEncoding, TargetEncoding>;
#endif
        std::ofstream file { path, std::ios::binary };
        if (!file.is_open()) {
            throw Failure(Fmt(Basic::Wcs::c_couldntWriteFile, path.wstring()));
        }
        TargetStream stream { file };
        Writer writer { stream };
#ifdef DEBUG
        writer.SetIndent(' ', 4);
#endif
        if (!json.Accept(writer)) {
            throw Failure(Wcs::c_couldntSerialize);
        }
        if (!file.good()) {
            throw Failure(Fmt(Basic::Wcs::c_couldntWriteFile, path.wstring()));
        }
    }

    template<Meta::JsonDocument J>
    void write(const J & json, std::wostream & output) {
        using SourceEncoding = Trait<J>::Encoding;
        using TargetEncoding = rapidjson::UTF8<>;
        using TargetStream = rapidjson::WOStreamWrapper;
#ifdef DEBUG
        using Writer = rapidjson::PrettyWriter<TargetStream, SourceEncoding, TargetEncoding>;
#else
        using Writer = rapidjson::Writer<TargetStream, SourceEncoding, TargetEncoding>;
#endif
        TargetStream stream { output };
        Writer writer { stream };
#ifdef DEBUG
        writer.SetIndent(' ', 4);
#endif
        if (!json.Accept(writer)) {
            throw Failure(Wcs::c_couldntSerialize);
        }
    }

    template<Meta::JsonDocument J>
    void write(const J & json, std::ostream & output) {
        using SourceEncoding = Trait<J>::Encoding;
        using TargetEncoding = rapidjson::UTF8<>;
        using TargetStream = rapidjson::OStreamWrapper;
#ifdef DEBUG
        using Writer = rapidjson::PrettyWriter<TargetStream, SourceEncoding, TargetEncoding>;
#else
        using Writer = rapidjson::Writer<TargetStream, SourceEncoding, TargetEncoding>;
#endif
        TargetStream stream { output };
        Writer writer { stream };
#ifdef DEBUG
        writer.SetIndent(' ', 4);
#endif
        if (!json.Accept(writer)) {
            throw Failure(Wcs::c_couldntSerialize);
        }
    }
}

void operator<<=(Meta::JsonDocument auto & json, const Meta::String auto & inputString) {
    Json::read(json, inputString);
}

void operator<<=(Meta::JsonDocument auto & json, const std::filesystem::path & inputPath) {
    Json::read(json, inputPath);
}

void operator>>=(const Meta::JsonDocument auto & json, Meta::String auto & outputString) {
    Json::write(json, outputString);
}

void operator>>=(const Meta::JsonDocument auto & json, const std::filesystem::path & outputPath) {
    Json::write(json, outputPath);
}

void operator>>=(const Meta::JsonDocument auto & json, std::wostream & outputStream) {
    Json::write(json, outputStream);
}

std::wostream & operator<<(std::wostream & outputStream, const Meta::JsonDocument auto & json) {
    Json::write(json, outputStream);
    return outputStream;
}

void operator>>=(const Meta::JsonDocument auto & json, std::ostream & outputStream) {
    Json::write(json, outputStream);
}

std::ostream & operator<<(std::ostream & outputStream, const Meta::JsonDocument auto & json) {
    Json::write(json, outputStream);
    return outputStream;
}
