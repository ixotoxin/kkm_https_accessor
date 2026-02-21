// Copyright (c) 2025 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_response.h"
#include "asio.h"
#include <lib/datetime.h>
#include <utility>
#include <atomic>
#include <vector>

namespace Http {
    class Request {
        using SequenceType = int64_t;

        static constexpr SequenceType c_idMask { 0xfff };
        static inline std::atomic<SequenceType> s_sequence { 1 + (DateTime::windows() & c_idMask) };

    public:
        using IdType = uint16_t;

        Header m_header {};
        Response m_response {};
        std::string m_verb {};
        std::string m_path {};
        std::string m_body {};
        Asio::IpAddress m_remote;
        std::vector<std::string> m_hint {};

        Method m_method { Method::NotImplemented };
        const IdType m_id;

        Request() = delete;

        explicit Request(Asio::IpAddress && remote)
        : m_remote { std::forward<Asio::IpAddress>(remote) },
          m_id { static_cast<IdType>(s_sequence.fetch_add(1 + (DateTime::windows() & c_idMask))) } {}

        Request(const Request &) = delete;
        Request(Request &&) = delete;
        ~Request() = default;

        Request & operator=(const Request &) = delete;
        Request & operator=(Request &&) = delete;

        [[nodiscard]]
        bool emptyResponse() const {
            return std::visit(
                Response::RenderOverloads {
                    [] (const std::nullptr_t) -> bool { return true; },
                    [] (const std::string_view data) -> bool { return data.empty(); },
                    [] (const std::shared_ptr<ProtoResponse> data) -> bool { return !data /*|| !*data*/; }
                },
                m_response.m_data
            );
        }
    };
}
