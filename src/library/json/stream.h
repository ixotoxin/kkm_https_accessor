// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <lib/text_traits.h>

namespace Json {
    template<Meta::Wideness W>
    class StringInputStream {
        const Text::String<W> & m_target;
        size_t m_position {};

    public:
        using Ch = Text::Char<W>;

        StringInputStream() = delete;
        StringInputStream(const StringInputStream &) = delete;
        StringInputStream(StringInputStream &&) = delete;

        explicit StringInputStream(const Text::String<W> & target) noexcept
        : m_target { target } {}

        ~StringInputStream() = default;

        Ch Peek() const {
            if (m_position < m_target.length()) {
                return m_target[m_position];
            }
            return W::c_terminator;
        }

        Ch Take() {
            if (m_position < m_target.length()) {
                return m_target[m_position++];
            }
            return W::c_terminator;
        }

        [[nodiscard]] size_t Tell() const {
            return m_position;
        }
    };

    template<Meta::Wideness W>
    class StringOutputStream {
        Text::String<W> & m_target;

    public:
        using Ch = Text::Char<W>;

        StringOutputStream() = delete;
        StringOutputStream(const StringOutputStream &) = delete;
        StringOutputStream(StringOutputStream &&) = delete;

        explicit StringOutputStream(Text::String<W> & target) noexcept
        : m_target { target } {}

        ~StringOutputStream() = default;

        void Put(Ch c) {
            m_target.push_back(c);
        }

        // NOLINTNEXTLINE(readability-convert-member-functions-to-static,readability-make-member-function-const)
        void Flush() {}
    };

    template<typename T>
    void PutUnsafe(StringOutputStream<Text::Wideness<T>> & stream, T c) {
        stream.Put(c);
    }
}
