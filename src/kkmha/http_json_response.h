// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_proto_response.h"
#include <constants.h>
#include <lib/meta.h>
#include <lib/json.h>
#include <cassert>
#include <utility>
#include <ostream>
#include <format>

namespace Http {
    struct JsonResponse final : ProtoResponse {
        Nln::Json m_data;
        size_t m_bodyReserve;

        JsonResponse()
        : ProtoResponse(), m_data(Nln::EmptyJsonObject), m_bodyReserve(c_sStrSize) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        explicit JsonResponse(const size_t bodyReserve)
        : ProtoResponse(), m_data(Nln::EmptyJsonObject), m_bodyReserve(bodyReserve) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        explicit JsonResponse(const Nln::Json & data, const size_t bodyReserve = c_sStrSize)
        : ProtoResponse(), m_data(data), m_bodyReserve(bodyReserve) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        explicit JsonResponse(Nln::Json && data, const size_t bodyReserve = c_sStrSize)
        : ProtoResponse(), m_data(std::move(data)), m_bodyReserve(bodyReserve) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        JsonResponse(bool success, const std::string_view message, const size_t bodyReserve = c_sStrSize)
        : ProtoResponse(),
            m_data({ { Json::Mbs::c_successKey, success }, { Json::Mbs::c_messageKey, message } }),
            m_bodyReserve(bodyReserve) {
            assert(m_data.is_object());
        }

        [[maybe_unused]]
        JsonResponse(bool success, std::string && message, const size_t bodyReserve = c_sStrSize)
        : ProtoResponse(),
            m_data({ { Json::Mbs::c_successKey, success }, { Json::Mbs::c_messageKey, std::move(message) } }),
            m_bodyReserve(bodyReserve) {
            assert(m_data.is_object());
        }

        JsonResponse(const JsonResponse &) = delete;
        JsonResponse(JsonResponse &&) = delete;
        ~JsonResponse() override = default;

        JsonResponse & operator=(const JsonResponse &) = delete;
        JsonResponse & operator=(JsonResponse &&) = delete;

        explicit operator bool() override {
            return !m_data.empty();
        }

        void render(Asio::StreamBuffer & buffer, const Status status) override {
            using Basic::Failure;
            using namespace std::string_view_literals;

            assert(Mbs::c_statusStrings.contains(status));
            assert(m_data.is_object());

            if (!m_data.contains(Json::Mbs::c_successKey) || !m_data[Json::Mbs::c_successKey].is_boolean()) {
                m_data[Json::Mbs::c_successKey] = status < Status::BadRequest;
            }
            if (!m_data.contains(Json::Mbs::c_messageKey) || !m_data[Json::Mbs::c_messageKey].is_string()) {
                m_data[Json::Mbs::c_messageKey] = Mbs::c_statusStrings.at(status);
            }

            std::string jsonText {};
            jsonText.reserve(m_bodyReserve);
            jsonText += m_data;

            std::string headerText {};
            headerText.reserve(c_sStrSize);
            std::format_to(
                std::back_inserter(headerText),
                Mbs::c_responseHeaderTemplate,
                Meta::toUnderlying(status),
                Mbs::c_statusStrings.at(status),
                Mbs::c_jsonMimeType,
                jsonText.size()
            );

            std::ostream output { &buffer };
            output << headerText << jsonText;
        }
    };
}
