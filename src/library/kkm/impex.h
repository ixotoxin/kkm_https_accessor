// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "json.h"
#include "device.h"
#include <concepts>

namespace Kkm {
    bool assign(JsonDoc &, const Result &);
    bool assign(JsonDoc &, const StatusResult &);
    bool assign(JsonDoc &, const ShiftStateResult &);
    bool assign(JsonDoc &, const ReceiptStateResult &);
    bool assign(JsonDoc &, const CashStatResult &);
    bool assign(JsonDoc &, const FndtOfdExchangeStatusResult &);
    bool assign(JsonDoc &, const FndtFnInfoResult &);
    bool assign(JsonDoc &, const FndtRegistrationInfoResult &);
    bool assign(JsonDoc &, const FndtLastRegistrationResult &);
    bool assign(JsonDoc &, const FndtLastReceiptResult &);
    bool assign(JsonDoc &, const FndtLastDocumentResult &);
    bool assign(JsonDoc &, const FndtErrorsResult &);
    bool assign(JsonDoc &, const FfdVersionsResult &);
    bool assign(JsonDoc &, const FwVersionsResult &);
    void assign(Details &, const JsonDoc &);
    void assign(PrintDetails &, const JsonDoc &);
    void assign(OperatorDetails &, const JsonDoc &);
    void assign(CashDetails &, const JsonDoc &);
    void assign(ReceiptDetails &, const JsonDoc &);
    void assign(CloseDetails &, const JsonDoc &);

    JsonDoc & operator<<=(JsonDoc & json, const std::derived_from<Result> auto & result) {
        assign(json, result);
        return json;
    }

    const JsonDoc & operator>>=(const JsonDoc & json, std::derived_from<Details> auto & details) {
        assign(details, json);
        return json;
    }
}
