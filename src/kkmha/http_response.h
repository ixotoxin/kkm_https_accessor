// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_proto_response.h"
#include <lib/meta.h>
#include <lib/json.h>
#include <cassert>
#include <memory>
#include <variant>
#include <ostream>
#include <format>

namespace Http {
    struct Response {
        template<class... T>
        struct RenderOverloads : T... { using T::operator()...; };

        std::variant<std::nullptr_t, std::string, std::shared_ptr<ProtoResponse>> m_data { nullptr };
        Status m_status { Status::Ok };

        Response() = default;
        Response(const Response &) = delete;
        Response(Response &&) = delete;
        ~Response() = default;

        Response & operator=(const Response &) = delete;
        Response & operator=(Response &&) = delete;

        static void render(Asio::StreamBuffer & buffer, const Status status, const std::string_view message = {}) {
            assert(Mbs::c_statusStrings.contains(status));
            const Nln::Json json(
                {
                    { Json::Mbs::c_successKey, status < Status::BadRequest },
                    { Json::Mbs::c_messageKey, message.empty() ? Mbs::c_statusStrings.at(status) : message }
                },
                false,
                Nln::EmptyJsonObject
            );
            const std::string responseText { json.dump() };
            std::ostream output { &buffer };
            output
                << std::format(
                    Mbs::c_responseHeaderTemplate,
                    Meta::toUnderlying(status),
                    Mbs::c_statusStrings.at(status),
                    Mbs::c_jsonMimeType,
                    responseText.size()
                )
                << responseText;
        }

        void render(Asio::StreamBuffer & buffer) {
            std::visit(
                RenderOverloads {
                    [& buffer, status = m_status] (std::nullptr_t) {
                        render(buffer, status);
                    },
                    [& buffer, status = m_status] (const std::string_view data) {
                        render(buffer, status, data);
                    },
                    [& buffer, status = m_status] (const std::shared_ptr<ProtoResponse> data) {
                        if (data) {
                            data->render(buffer, status);
                        } else {
                            render(buffer, Status::InternalServerError);
                        }
                    }
                },
                m_data
            );
        }
    };
}
