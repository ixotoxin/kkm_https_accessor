// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "types.h"
#include <lib/hexer.h>

namespace Json {
    namespace Detail {
        template<Meta::String T>
        void writeHex1(T & result, size_t & pos, const uint32_t code) {
            Bin::Int2Hex hex { code };
            result[pos++] = Trait<T>::c_letterU;
            result[pos++] = Trait<T>::c_zero;
            result[pos++] = Trait<T>::c_zero;
            hex.writeTo<1>(result, pos);
        }

        template<Meta::String T>
        void writeHex2(T & result, size_t & pos, const uint32_t code) {
            Bin::Int2Hex hex { code };
            result[pos++] = Trait<T>::c_letterU;
            hex.writeTo<2>(result, pos);
        }

        template<Meta::String T>
        void writeHex3(T & result, size_t & pos, const uint32_t code) {
            Bin::Int2Hex hex { code };
            result[pos++] = Trait<T>::c_letterU;
            result[pos++] = Trait<T>::c_openingCurlyBrace;
            hex.writeTo<3>(result, pos);
            result[pos++] = Trait<T>::c_closingCurlyBrace;
        }

        template<Meta::View T>
        size_t basicExtraSpace(const T text) {
            size_t result { 0 };
            auto it = text.begin();

            while (it != text.end()) {
                switch (const uint32_t high { static_cast<uint32_t>(*it) }) {
                    case Trait<T>::c_quotationMark:
                    case Trait<T>::c_solidus:
                    case Trait<T>::c_reverseSolidus:
                    case Trait<T>::c_backspace:
                    case Trait<T>::c_formFeed:
                    case Trait<T>::c_newLine:
                    case Trait<T>::c_carriageReturn:
                    case Trait<T>::c_horizontalTab:
                        ++result;
                        break;
                    default:
                        if (high <= 0x1f || high == 0x7f) {
                            result += 5;
                        }
                        break;
                }
                ++it;
            }

            return result;
        }

        inline size_t fullExtraSpace(const std::wstring_view text) {
            size_t result { 0 };
            auto it = text.begin();

            while (it != text.end()) {
                switch (const uint32_t high { *it }) {
                    case L'"':
                    case L'/':
                    case L'\\':
                    case L'\b':
                    case L'\f':
                    case L'\n':
                    case L'\r':
                    case L'\t':
                        ++result;
                        break;
                    default:
                        // NOLINTNEXTLINE(*-branch-clone)
                        if (high <= 0x001f) {
                            result += 5;
                        } else if (high <= 0x007e) {
                            /** NOP **/
                        } else if (high <= 0x00ff) {
                            result += 5;
                        } else if (high >= 0xd800 && high <= 0xdbff) {
                            ++it;
                            if (it == text.end()) {
                                result += 5;
                                break;
                            }
                            if (const uint32_t low { *it }; low >= 0xdc00 && low <= 0xdfff) {
                                result += 8;
                                break;
                            }
                            result += 10;
                        } else {
                            result += 5;
                        }
                        break;
                }
                ++it;
            }

            return result;
        }
    }

    template<Meta::View T>
    void escapeBasic(String<T> & output, const T text) {
        const auto extraSpace = Detail::basicExtraSpace(text);
        if (extraSpace == 0) {
            output.append(text.data(), text.size());
            return;
        }

        size_t size { text.size() };
        size_t pos { output.size() };
        output.append(size + extraSpace, Meta::WcsText::c_reverseSolidus);
        auto it = text.begin();

        while (it != text.end()) {
            switch (const uint32_t codePoint { static_cast<uint32_t>(*it) }) {
                case Trait<T>::c_quotationMark: output[++pos] = Trait<T>::c_quotationMark; ++pos; break;
                case Trait<T>::c_solidus: output[++pos] = Trait<T>::c_solidus; ++pos; break;
                case Trait<T>::c_reverseSolidus: pos += 2; break;
                case Trait<T>::c_backspace: output[++pos] = Trait<T>::c_backspaceLiteral; ++pos; break;
                case Trait<T>::c_formFeed: output[++pos] = Trait<T>::c_formFeedLiteral; ++pos; break;
                case Trait<T>::c_newLine: output[++pos] = Trait<T>::c_newLineLiteral; ++pos; break;
                case Trait<T>::c_carriageReturn: output[++pos] = Trait<T>::c_carriageReturnLiteral; ++pos; break;
                case Trait<T>::c_horizontalTab: output[++pos] = Trait<T>::c_horizontalTabLiteral; ++pos; break;
                default:
                    if (codePoint <= 0x1f || codePoint == 0x7f) {
                        Detail::writeHex1(output, ++pos, codePoint);
                    } else {
                        output[pos++] = static_cast<wchar_t>(codePoint);
                    }
                    break;
            }
            ++it;
        }
    }

    template<Meta::String T>
    void escapeBasic(String<T> & output, const T & text) {
        escapeBasic<View<T>>(output, View<T> { text });
    }

    template<Meta::Char T>
    void escapeBasic(String<T> & output, const T * text) {
        escapeBasic<View<T>>(output, View<T> { text });
    }

    template<Meta::JsonKey T>
    void escapeBasic(String<T> & output, const T text) {
        escapeBasic(output, View<T> { text.s, text.length });
    }

    template<Meta::View T>
    String<T> escapeBasic(const T text) {
        String<T> result;
        escapeBasic<View<T>>(result, text);
        return result;
    }

    template<Meta::String T>
    String<T> escapeBasic(const T & text) {
        String<T> result;
        escapeBasic<View<T>>(result, View<T> { text });
        return result;
    }

    template<Meta::Char T>
    String<T> escapeBasic(const T * text) {
        String<T> result;
        escapeBasic<View<T>>(result, View<T> { text });
        return result;
    }

    template<Meta::JsonKey T>
    String<T> escapeBasic(const T text) {
        String<T> result;
        escapeBasic<View<T>>(result, View<T> { text.s, text.length });
        return result;
    }

    inline void escapeFull(std::wstring & output, const std::wstring_view text) {
        const auto extraSpace = Detail::fullExtraSpace(text);
        if (extraSpace == 0) {
            output.append(text.data(), text.size());
            return;
        }

        const size_t size { text.size() };
        size_t pos { output.size() };
        output.append(size + extraSpace, Meta::WcsText::c_reverseSolidus);
        auto it = text.begin();

        while (it != text.end()) {
            switch (const uint32_t high { *it /*static_cast<uint32_t>(*it)*/ }) {
                case L'"': output[++pos] = '"'; ++pos; break;
                case L'/': output[++pos] = '/'; ++pos; break;
                case L'\\': pos += 2; break;
                case L'\b': output[++pos] = L'b'; ++pos; break;
                case L'\f': output[++pos] = L'f'; ++pos; break;
                case L'\n': output[++pos] = L'n'; ++pos; break;
                case L'\r': output[++pos] = L'r'; ++pos; break;
                case L'\t': output[++pos] = L't'; ++pos; break;
                default:
                    // NOLINTNEXTLINE(*-branch-clone)
                    if (high <= 0x001f) {
                        Detail::writeHex1(output, ++pos, high);
                    } else if (high <= 0x007e) {
                        output[pos++] = static_cast<wchar_t>(high);
                    } else if (high <= 0x00ff) {
                        Detail::writeHex1(output, ++pos, high);
                    } else if (high >= 0xd800 && high <= 0xdbff) {
                        ++it;
                        if (it == text.end()) {
                            Detail::writeHex2(output, ++pos, high);
                            break;
                        }
                        const uint32_t low { *it /*static_cast<uint32_t>(*it)*/ };
                        if (low >= 0xdc00 && low <= 0xdfff) {
                            const uint32_t codePoint = 0x01'0000 + ((high & 0x03ff) << 10) | (low & 0x03ff);
                            Detail::writeHex3(output, ++pos, codePoint);
                            break;
                        }
                        Detail::writeHex2(output, ++pos, high);
                        Detail::writeHex2(output, ++pos, low);
                    } else {
                        Detail::writeHex2(output, ++pos, high);
                    }
                    break;
            }
            ++it;
        }
    }

    inline std::wstring escapeFull(const std::wstring_view text) {
        std::wstring result;
        escapeFull(result, text);
        return result;
    }
}
