// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#include "server_config_handler.h"
#include "server_json.h"
#include "server_strings.h"
#include <constants.h>
#include <lib/wconv.h>
#include <lib/text.h>
#include <kkm/variables.h>
#include <cassert>

namespace Server::Config {
    bool Handler::asyncReady() const noexcept {
        return false;
    }

    void Handler::operator()(Http::Request & request) const noexcept try {
        // TODO: Реализовать кеширование ответа

        assert(request.m_response.m_status == Http::Status::Ok);

        if (request.m_method == Http::Method::Get && request.m_hint.size() == 3 && request.m_hint[2] == "general") {
            JsonAlloc allocator {};
            JsonDoc jsonResponse { Json::Type::Object, &allocator };

            {
                Json::Object response { jsonResponse };
                response[Json::Wcs::c_successKey] <<= true;
                response[Json::Wcs::c_messageKey] <<= L"OK";

                Json::Object cliOperator { jsonResponse, L"cliOperator"_key };
                cliOperator[L"name"_key] <<= Kkm::s_cliOperatorName;
                cliOperator[L"inn"_key] <<= Kkm::s_cliOperatorInn;

                JsonVal knownDevices { Json::Type::Array };
                for (auto const & entry: std::filesystem::directory_iterator { Kkm::s_dbDirectory }) {
                    if (entry.is_regular_file()) {
                        if (Text::lowered(entry.path().extension().wstring()) != L".json") {
                            continue;
                        }
                        auto serialNumber = entry.path().stem().wstring();
                        knownDevices.PushBack(JsonVal(serialNumber.c_str(), allocator), allocator);
                    }
                }
                response[L"knownDevices"_key] <<= std::move(knownDevices);
            }

            auto response = std::make_shared<Http::TextResponse>();
            response->m_mimeType = Http::Mbs::c_jsonMimeType;
            response->m_data.reserve(c_mStrSize);
            jsonResponse >>= response->m_data;
            request.m_response.m_data = std::move(response);

        } else {
            fail(request, Http::Status::MethodNotAllowed, Server::Mbs::c_methodNotAllowed);
        }

    } catch (const Basic::Failure & e) {
        fail(request, Http::Status::InternalServerError, Text::convert(e.what()), e.where());
    } catch (const std::exception & e) {
        fail(request, Http::Status::InternalServerError, e.what());
    } catch (...) {
        fail(request, Http::Status::InternalServerError, Basic::Mbs::c_somethingWrong);
    }
}
