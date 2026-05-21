// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <json/keys.h>

namespace Http {
    inline void renderSimpleJson(std::string & output, const bool success, const std::string_view message) {
        const size_t size {
            11 + Json::Mbs::c_successKeyEsc.length() + Json::Mbs::c_messageKeyEsc.length() + message.length()
        };
        if (output.capacity() < size) {
            output.reserve(size);
        }
        output.assign("{\"");
        output.append(Json::Mbs::c_successKeyEsc);
        output.append("\":");
        output.append(Text::Mbs::trueFalse(success));
        output.append(",\"");
        output.append(Json::Mbs::c_messageKeyEsc);
        output.append("\":\"");
        Json::escapeBasic(output, message);
        output.append("\"}");
    }
}
