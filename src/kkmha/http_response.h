// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_helpers.h"
#include "http_proto_response.h"
#include <constants.h>
#include <lib/meta.h>
#include <cassert>
#include <memory>
#include <ostream>
#include <format>

namespace Http {
    struct Response {
        std::shared_ptr<ProtoResponse> m_data { nullptr };
        Status m_status { Status::Ok };

        Response() noexcept = default;
        Response(const Response &) = delete;
        Response(Response &&) = delete;
        ~Response() = default;

        Response & operator=(const Response &) = delete;
        Response & operator=(Response &&) = delete;

        void render(Asio::StreamBuffer & buffer) const {
            assert(Mbs::c_statusStrings.contains(m_status));

            if (m_data) {
                m_data->render(buffer, m_status);
            } else {
                std::string jsonText {};
                renderSimpleJson(jsonText, m_status < Status::BadRequest, Mbs::c_statusStrings.at(m_status));

                std::string headerText {};
                headerText.reserve(c_sStrSize);
                std::format_to(
                    std::back_inserter(headerText),
                    Mbs::c_responseHeaderTemplate,
                    Meta::toUnderlying(m_status),
                    Mbs::c_statusStrings.at(m_status),
                    Mbs::c_jsonMimeType,
                    jsonText.size()
                );

                std::ostream output { &buffer };
                output << headerText << jsonText;
            }
        }
    };
}
