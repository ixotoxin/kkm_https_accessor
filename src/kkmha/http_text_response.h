// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_helpers.h"
#include "http_proto_response.h"
#include <constants.h>
#include <cassert>
#include <ostream>
#include <format>

namespace Http {
    struct TextResponse final : ProtoResponse {
        std::string m_mimeType {};
        std::string m_data {};

        TextResponse() = default;
        TextResponse(const TextResponse &) = delete;
        TextResponse(TextResponse &&) = delete;

        TextResponse(std::string && data, const std::string_view mimeType)
        : m_mimeType { mimeType }, m_data { std::move(data) } {}

        TextResponse(const std::string_view data, const std::string_view mimeType)
        : m_mimeType { mimeType }, m_data { data } {}

        TextResponse(const bool success, const std::string_view message)
        : m_mimeType { Mbs::c_jsonMimeType } {
            renderSimpleJson(m_data, success, message);
        }

        ~TextResponse() override = default;

        TextResponse & operator=(const TextResponse &) = delete;
        TextResponse & operator=(TextResponse &&) = delete;

        explicit operator bool() override {
            return !m_data.empty();
        }

        void render(Asio::StreamBuffer & buffer, const Status status) override {
            assert(Mbs::c_statusStrings.contains(status));

            std::string headerText {};
            headerText.reserve(c_sStrSize);
            std::format_to(
                std::back_inserter(headerText),
                Mbs::c_responseHeaderTemplate,
                Meta::toUnderlying(status),
                Mbs::c_statusStrings.at(status),
                m_mimeType,
                m_data.size()
            );

            std::ostream output { &buffer };
            output << headerText << m_data;
        }
    };
}
