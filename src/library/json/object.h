// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "xetters.h"
#include <lib/types.h>

namespace Json {
    template<Meta::JsonDocument J>
    class Object {
        class KeyAccessor;

        Allocator & m_allocator;
        Value<J> & m_value;
        Ptr m_pointer;

        static Value<J> & getKeyRef(Value<J> & json, const Key<J> & key, Allocator & allocator, const PtrView pointer) {
            if (!json.IsObject()) {
                if (!json.IsNull()) {
                    throw DataError(Wcs::c_isNotObject, pointer);
                }
                json.SetObject();
            }
            if (!json.HasMember(key)) {
                json.AddMember(key, Value<J> { Type::Object }, allocator);
            }
            return json[kv(key)];
        }

        void assertValueIsObject() {
            if (!m_value.IsObject()) {
                if (!m_value.IsNull()) {
                    throw DataError(Wcs::c_isNotObject, std::move(m_pointer), true);
                }
                m_value.SetObject();
            }
        }

    public:
        Object() = delete;
        Object(const Object &) noexcept = default;
        Object(Object &&) = delete;

        explicit Object(J & document, const PtrView pointer = {})
        : m_allocator { document.GetAllocator() }, m_value { document },
          m_pointer { pointer.empty() ? Meta::WcsText::c_solidusStr : pointer } {
            assert(!m_pointer.empty());
            assertValueIsObject();
        }

        Object(Value<J> & value, J & document, const PtrView pointer = {})
        : m_allocator { document.GetAllocator() }, m_value { value },
          m_pointer { pointer.empty() ? Meta::WcsText::c_solidusStr : pointer } {
            assert(!m_pointer.empty());
            assertValueIsObject();
        }

        Object(J & document, const Key<J> & key, const PtrView basePointer = {})
        : m_allocator { document.GetAllocator() }, m_value { getKeyRef(document, key, m_allocator, basePointer) },
          m_pointer { makePointer(basePointer, key) } {
            assert(!m_pointer.empty());
            assertValueIsObject();
        }

        Object(Value<J> & value, const Key<J> & key, J & document, const PtrView basePointer = {})
        : m_allocator { document.GetAllocator() }, m_value { getKeyRef(value, key, m_allocator, basePointer) },
          m_pointer { makePointer(basePointer, key) } {
            assert(!m_pointer.empty());
            assertValueIsObject();
        }

        Object(const Object & builder, const Key<J> & key, const PtrView basePointer = {})
        : m_allocator { builder.m_allocator }, m_value { getKeyRef(builder.m_value, key, m_allocator, basePointer) },
          m_pointer { makePointer(basePointer, key) } {
            assert(!m_pointer.empty());
            assertValueIsObject();
        }

        ~Object() = default;

        Object & operator=(const Object &) = delete;
        Object & operator=(Object &&) = delete;

        [[maybe_unused]]
        KeyAccessor operator[](const Key<J> key) { // NOLINT
            return { *this, key };
        }

        [[nodiscard, maybe_unused]]
        bool rjEmpty() const {
            return m_value.ObjectEmpty();
        }

        [[nodiscard, maybe_unused]]
        SizeType rjCount() const {
            return m_value.MemberCount();
        }

        [[maybe_unused]]
        void clear() {
            m_value.SetObject();
        }
    };

    template<Meta::JsonDocument J>
    class Object<J>::KeyAccessor {
        const Key<J> m_key;
        Allocator & m_allocator;
        Value<J> & m_value;
        Ptr & m_pointer;

    public:
        KeyAccessor() = delete;
        KeyAccessor(const KeyAccessor &) = delete;

        KeyAccessor(Object & builder, Key<J> key) // NOLINT
        : m_key { key }, m_allocator { builder.m_allocator }, m_value { builder.m_value },
          m_pointer { builder.m_pointer } {}

        KeyAccessor(KeyAccessor &&) = delete;
        ~KeyAccessor() = default;

        KeyAccessor & operator=(const KeyAccessor &) = delete;
        KeyAccessor & operator=(KeyAccessor &&) = delete;

        [[nodiscard, maybe_unused]]
        bool rjExists() const {
            return m_value.HasMember(m_key);
        }

        [[nodiscard, maybe_unused]]
        bool rjEmpty() const {
            auto it = m_value.FindMember(m_key);
            return it == m_value.MemberEnd()
                || it->value.IsNull()
                || (it->value.IsArray() && it->value.Empty())
                || (it->value.IsObject() && it->value.ObjectEmpty())
                || (it->value.IsString() && it->value.GetStringLength() == 0);
        }

        [[nodiscard, maybe_unused]]
        SizeType rjCount() const {
            auto it = m_value.FindMember(m_key);
            if (it != m_value.MemberEnd()) {
                if (it->value.IsArray()) {
                    return it->value.Size();
                }
                if (it->value.IsObject()) {
                    return it->value.MemberCount();
                }
                return 1;
            }
            return 0;
        }

        [[nodiscard, maybe_unused]]
        bool rjIsNull() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsNull();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsFalse() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsFalse();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsTrue() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsTrue();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsBool() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsBool();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsObject() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsObject();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsArray() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsArray();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsNumber() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsNumber();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsInt() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsInt();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsUint() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsUint();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsInt64() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsInt64();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsUint64() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsUint64();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsDouble() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsDouble();
        }

        [[nodiscard, maybe_unused]]
        bool rjIsString() const {
            auto it = m_value.FindMember(m_key);
            return it != m_value.MemberEnd() && it->value.IsString();
        }

        [[maybe_unused]]
        bool handle(const Handler<J> & handler) const {
            return handleKey(m_value, m_key, handler, m_pointer);
        }

        [[maybe_unused]]
        bool operator()(const Handler<J> & handler) const {
            return handleKey(m_value, m_key, handler, m_pointer);
        }

        template<typename ... Args>
        [[maybe_unused]]
        bool handle(auto & output, Args && ... args) const {
            return handleKey(m_value, m_key, output, std::forward<Args>(args)..., m_pointer);
        }

        template<typename ... Args>
        [[maybe_unused]]
        bool operator()(auto & output, Args && ... args) const {
            return handleKey(m_value, m_key, output, std::forward<Args>(args)..., m_pointer);
        }

        [[maybe_unused]]
        void get(auto & value) const {
            getKey(m_value, m_key, value, m_pointer);
        }

        [[maybe_unused]]
        void operator>>=(auto & value) const {
            getKey(m_value, m_key, value, m_pointer);
        }

        template<typename V, Meta::Domain<V> D>
        [[maybe_unused]]
        void get(V & value, const D & domain) const {
            getKey(m_value, m_key, value, domain, m_pointer);
        }

        template<typename V, Meta::Domain<V> D>
        [[maybe_unused]]
        void operator>>=(Basic::RefPair<V, D> && agrs) const {
            getKey(m_value, m_key, agrs.m_first, agrs.m_second, m_pointer);
        }

        template<typename V, Meta::CastTo<V> M>
        [[maybe_unused]]
        void get(V & value, const M & castMap) const {
            getKey(m_value, m_key, value, castMap, m_pointer);
        }

        template<typename V, Meta::CastTo<V> M>
        [[maybe_unused]]
        void operator>>=(Basic::RefPair<V, M> && agrs) const {
            getKey(m_value, m_key, agrs.m_first, agrs.m_second, m_pointer);
        }

        [[maybe_unused]]
        void set(Value<J> && value) {
            setKey(m_value, m_key, std::move(value), m_allocator, m_pointer);
        }

        [[maybe_unused]]
        void operator<<=(Value<J> && value) {
            setKey(m_value, m_key, std::move(value), m_allocator, m_pointer);
        }

        [[maybe_unused]]
        void set(const Meta::Arithmetic auto value) {
            setKey(m_value, m_key, value, m_allocator, m_pointer);
        }

        [[maybe_unused]]
        void operator<<=(const Meta::Arithmetic auto value) {
            setKey(m_value, m_key, value, m_allocator, m_pointer);
        }

        template<typename E>
        requires std::is_enum_v<E>
        [[maybe_unused]]
        void set(const E value) {
            setKey(m_value, m_key, Meta::toUnderlying(value), m_allocator, m_pointer);
        }

        template<typename E>
        requires std::is_enum_v<E>
        [[maybe_unused]]
        void operator<<=(const E value) {
            setKey(m_value, m_key, Meta::toUnderlying(value), m_allocator, m_pointer);
        }

        [[maybe_unused]]
        void set(const View<J> value) {
            setKey(m_value, m_key, value, m_allocator, m_pointer);
        }

        [[maybe_unused]]
        void operator<<=(const View<J> value) {
            setKey(m_value, m_key, value, m_allocator, m_pointer);
        }

        [[maybe_unused]]
        void set(const std::tm & value) {
            setKey(m_value, m_key, value, m_allocator, m_pointer);
        }

        [[maybe_unused]]
        void operator<<=(const std::tm & value) {
            setKey(m_value, m_key, value, m_allocator, m_pointer);
        }

        template<Meta::NotText V, Meta::Domain<V> D>
        [[maybe_unused]]
        void set(V && value, const D & domain) {
            setKey(m_value, m_key, Domain::cast<V>(std::forward<V>(value), domain), m_allocator, m_pointer);
        }

        template<Meta::NotText V, Meta::Domain<V> D>
        [[maybe_unused]]
        void operator<<=(Basic::RefPair<V, D> && agrs) {
            setKey(m_value, m_key, Domain::cast<V>(agrs.m_first, agrs.m_second), m_allocator, m_pointer);
        }

        template<Meta::Text<Wideness<J>> V, Meta::TextDomain<Wideness<J>> D>
        [[maybe_unused]]
        void set(V && value, const D & domain) {
            setKey(m_value, m_key, Domain::cast<String<V>>(std::forward<V>(value), domain), m_allocator, m_pointer);
        }

        template<Meta::Text<Wideness<J>> V, Meta::TextDomain<Wideness<J>> D>
        [[maybe_unused]]
        void operator<<=(Basic::RefPair<V, D> && agrs) {
            setKey(m_value, m_key, Domain::cast<String<V>>(agrs.m_first, agrs.m_second), m_allocator, m_pointer);
        }

        template<Meta::NotText V, Meta::CastFrom<V> M>
        [[maybe_unused]]
        void set(V && value, const M & castMap) {
            setKey(
                m_value, m_key,
                Dictionary::cast<Meta::ValueType<M>>(std::forward<V>(value), castMap, true),
                m_allocator, m_pointer
            );
        }

        template<Meta::NotText V, Meta::CastFrom<V> M>
        [[maybe_unused]]
        void operator<<=(Basic::RefPair<V, M> && agrs) {
            setKey(
                m_value, m_key,
                Dictionary::cast<Meta::ValueType<M>>(agrs.m_first, agrs.m_second, true),
                m_allocator, m_pointer
            );
        }

        template<Meta::AnyText V, Meta::CastFromText<V> M>
        [[maybe_unused]]
        void set(V && value, const M & castMap) {
            setKey(
                m_value, m_key,
                Dictionary::cast<Meta::ValueType<M>>(std::forward<V>(value), castMap, true),
                m_allocator, m_pointer
            );
        }

        template<Meta::AnyText V, Meta::CastFromText<V> M>
        [[maybe_unused]]
        void operator<<=(Basic::RefPair<V, M> && agrs) {
            setKey(
                m_value, m_key,
                Dictionary::cast<Meta::ValueType<M>>(agrs.m_first, agrs.m_second, true),
                m_allocator, m_pointer
            );
        }
    };
}
