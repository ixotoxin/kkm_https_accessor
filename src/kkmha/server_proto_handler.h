// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_request.h"
#include "http_text_response.h"
#include <cassert>

namespace Server {
    class ProtoHandler {
    public:
        ProtoHandler() = default;
        ProtoHandler(const ProtoHandler &) = default;
        ProtoHandler(ProtoHandler &&) = default;
        virtual ~ProtoHandler() = default;

        ProtoHandler & operator=(const ProtoHandler &) = default;
        ProtoHandler & operator=(ProtoHandler &&) noexcept = default;

        [[nodiscard]] virtual bool asyncReady() const noexcept = 0;
        virtual void operator()(Http::Request &) const noexcept = 0;

        static void fail(
            Http::Request & request,
            const Http::Status status,
            const std::string_view message,
            const SrcLoc::Point & location = SrcLoc::Point::current()
        ) {
            assert(Meta::toUnderlying(status) >= 400);
            request.m_logger->error(location, message);
            request.m_response.m_status = status;
            request.m_response.m_data = std::make_shared<Http::TextResponse>(false, message);
        }
    };
}
