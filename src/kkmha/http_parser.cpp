// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "http_parser.h"
#include "http_defaults.h"
#include <lib/text.h>

namespace Http {
    using Basic::Failure;

    void Parser::parseMethod(std::istream & stream) {
        std::getline(stream, m_line);

        auto pos1 = m_line.find_first_of(' ');
        if (pos1 == std::string::npos) {
            m_request.m_response.m_status = Status::BadRequest;
            m_reader = &Parser::dummyReader;
            return;
        }
        m_request.m_verb.assign(m_line.data(), /*m_line.data() +*/ pos1);

        ++pos1;
        auto pos2 = m_line.find_first_of(" ?#\r\n", pos1);
        if (pos2 == std::string::npos || pos1 == pos2) {
            m_request.m_response.m_status = Status::BadRequest;
            m_reader = &Parser::dummyReader;
            return;
        }
        m_request.m_path.reserve(pos2 - pos1);
        // CLEANUP
        // m_request.m_path.assign(m_line.data() + pos1, m_line.data() + pos2);
        m_request.m_path.assign(m_line.data() + pos1, pos2 - pos1);

        Text::splitTo(m_request.m_hint, Text::lowered<std::string>({ m_line.c_str(), pos2 }), " /\\");
        if (m_request.m_hint.empty()) {
            m_request.m_response.m_status = Status::BadRequest;
            m_reader = &Parser::dummyReader;
            return;
        }

        if (m_request.m_hint[0] == "post") {
            m_request.m_method = Method::Post;
        } else if (m_request.m_hint[0] == "get") {
            m_request.m_method = Method::Get;
        } else {
            m_request.m_response.m_status = Status::NotImplemented;
            m_reader = &Parser::dummyReader;
            return;
        }

        m_reader = &Parser::parseHeader;
        ++m_step;
    }

    void Parser::parseHeader(std::istream & stream) {
        std::getline(stream, m_line);
        Text::trim(m_line);

        if (m_line.empty()) {
            if (m_request.m_method == Method::Post && m_expectedBodySize) {
                ++m_step;
                m_reader = &Parser::parseBody;
            } else {
                m_reader = &Parser::dummyReader;
            }
            return;
        }

        Text::splitVariable(m_line, m_field, m_value);
        m_request.m_header[m_field] = m_value;
        if (m_field == "content-length") {
            m_expectedBodySize = Text::cast<size_t>(m_value);
            if (m_expectedBodySize > c_requestBodySizeLimit) {
                m_request.m_logger->error(Wcs::c_bodySizeLimitExceeded);
                m_expectedBodySize = 0;
                m_request.m_response.m_status = Status::BadRequest;
                m_request.m_response.m_data.emplace<std::string>(Mbs::c_bodySizeLimitExceeded);
                m_reader = &Parser::dummyReader;
            } else {
                m_request.m_body.reserve(m_expectedBodySize + 1);
            }
        }
    }

    void Parser::parseBody(std::istream & stream) { // NOLINT
        char buffer[c_parserBufferSize];
        while (stream.read(buffer, sizeof(buffer))) {
            m_request.m_body.append(buffer, sizeof(buffer));
        }
        m_request.m_body.append(buffer, stream.gcount());
    }

    void Parser::dummyReader(std::istream & stream) { // NOLINT // NOLINT(*-convert-member-functions-to-static)
        stream.ignore(std::numeric_limits<std::streamsize>::max());
    }

    void Parser::operator()(Asio::StreamBuffer & buffer) {
        try {
            std::istream input(&buffer);
            std::noskipws(input);
            while (input.good()) {
                (this->*m_reader)(input);
            }
            return;
        } catch (const Failure & e) {
            m_request.m_logger->error(e);
        } catch (const std::exception & e) {
            m_request.m_logger->error(e.what());
        } catch (...) {
            m_request.m_logger->error(Basic::Wcs::c_somethingWrong);
        }
        if (m_request.m_response.m_status == Status::Ok) {
            m_request.m_response.m_status = Status::BadRequest;
        }
        m_reader = &Parser::dummyReader;
    }
}
