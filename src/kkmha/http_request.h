// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "http_types.h"
#include "http_strings.h"
#include "http_logger.h"
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

        // ISSUE: Разобраться, почему MSVC с LTCG эту переменную без явного указания выравнивания превращает в разных TU
        //  в разные типы [ '__declspec(align(8)) struct (8 bytes)' and 'struct (8 bytes)' ] и, как следствие,
        //  не может слинковать.
        alignas(alignof(int64_t)) static inline std::atomic<SequenceType> s_sequence { 1 + (DateTime::windows() & c_idMask) };

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
        LoggerPtr m_logger;

        Request() = delete;

        explicit Request(Asio::IpAddress && remote)
        : m_remote { std::forward<Asio::IpAddress>(remote) },
          m_id { static_cast<IdType>(s_sequence.fetch_add(1 + (DateTime::windows() & c_idMask))) },
          m_logger { std::make_shared<Logger>(std::format(Wcs::c_requestPrefix, m_id)) } {}

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
