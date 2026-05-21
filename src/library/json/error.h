// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "strings.h"
#include <rapidjson/error/error.h>

namespace Json::Wcs {
    inline std::wstring_view getParseError(const rapidjson::ParseErrorCode parseErrorCode) {
        using namespace rapidjson;
        switch (parseErrorCode) {
            case kParseErrorNone: return c_parseErrorNone;
            case kParseErrorDocumentEmpty: return c_parseErrorDocumentEmpty;
            case kParseErrorDocumentRootNotSingular: return c_parseErrorDocumentRootNotSingular;
            case kParseErrorValueInvalid: return c_parseErrorValueInvalid;
            case kParseErrorObjectMissName: return c_parseErrorObjectMissName;
            case kParseErrorObjectMissColon: return c_parseErrorObjectMissColon;
            case kParseErrorObjectMissCommaOrCurlyBracket: return c_parseErrorObjectMissCommaOrCurlyBracket;
            case kParseErrorArrayMissCommaOrSquareBracket: return c_parseErrorArrayMissCommaOrSquareBracket;
            case kParseErrorStringUnicodeEscapeInvalidHex: return c_parseErrorStringUnicodeEscapeInvalidHex;
            case kParseErrorStringUnicodeSurrogateInvalid: return c_parseErrorStringUnicodeSurrogateInvalid;
            case kParseErrorStringEscapeInvalid: return c_parseErrorStringEscapeInvalid;
            case kParseErrorStringMissQuotationMark: return c_parseErrorStringMissQuotationMark;
            case kParseErrorStringInvalidEncoding: return c_parseErrorStringInvalidEncoding;
            case kParseErrorNumberTooBig: return c_parseErrorNumberTooBig;
            case kParseErrorNumberMissFraction: return c_parseErrorNumberMissFraction;
            case kParseErrorNumberMissExponent: return c_parseErrorNumberMissExponent;
            case kParseErrorTermination: return c_parseErrorTermination;
            case kParseErrorUnspecificSyntaxError: return c_parseErrorUnspecificSyntaxError;
            default: return c_parseUnknownError;
        }
    }
}
