// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "strings.h"
#include "types.h"
#include "keys.h"
#include "cast.h"
#include <lib/datetime.h>
#include <cassert>

namespace Json {
    template<Meta::Json J>
    [[maybe_unused]]
    auto & requireArray(
        const J & json,
        const SizeType minItems = 0,
        const SizeType maxItems = 0,
        const PtrView pointer = {}
    ) {
        assert(minItems >= 0 && (maxItems >= minItems || maxItems == 0)); // NOLINT
        if (!json.IsArray()) {
            throw DataError(Wcs::c_isNotArray, pointer, true);
        }
        if (minItems > 0 && json.Size() < minItems) {
            throw DataError(Wcs::c_arrayWrongLength, pointer, true);
        }
        if (maxItems > minItems && json.Size() > maxItems) {
            throw DataError(Wcs::c_arrayWrongLength, pointer, true);
        }
        return json;
    }

    template<Meta::Json J>
    [[maybe_unused]]
    auto & requireArray(
        const J & json,
        const Key<J> key, // NOLINT
        const SizeType minItems = 0,
        const SizeType maxItems = 0,
        const PtrView basePointer = {}
    ) {
        assert(minItems >= 0 && (maxItems >= minItems || maxItems == 0)); // NOLINT
        if (!json.IsObject()) {
            throw DataError(Wcs::c_isNotObject, basePointer.empty() ? Meta::WcsText::c_solidusStr : basePointer);
        }
        Ptr pointer { makePointer(basePointer, key) };
        if (!json.HasMember(key)) {
            throw DataError(Wcs::c_keyNotFound, std::move(pointer));
        }
        auto & result = json[kv(key)];
        if (!result.IsArray()) {
            throw DataError(Wcs::c_isNotArray, std::move(pointer));
        }
        if (minItems > 0 && result.Size() < minItems) {
            throw DataError(Wcs::c_arrayWrongLength, std::move(pointer));
        }
        if (maxItems > minItems && result.Size() > maxItems) {
            throw DataError(Wcs::c_arrayWrongLength, std::move(pointer));
        }
        return result;
    }

    template<Meta::Json J>
    [[maybe_unused]]
    auto & requireObject(const J & json, const PtrView pointer = {}) {
        if (!json.IsObject()) {
            throw DataError(Wcs::c_isNotObject, pointer.empty() ? Meta::WcsText::c_solidusStr : pointer);
        }
        return json;
    }

    template<Meta::Json J>
    [[maybe_unused]]
    auto & requireObject(
        const J & json,
        const Key<J> key, // NOLINT
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            throw DataError(Wcs::c_isNotObject, basePointer.empty() ? Meta::WcsText::c_solidusStr : basePointer);
        }
        Ptr pointer { makePointer(basePointer, key) };
        if (!json.HasMember(key)) {
            throw DataError(Wcs::c_keyNotFound, std::move(pointer));
        }
        auto & result = json[kv(key)];
        if (!result.IsObject()) {
            throw DataError(Wcs::c_isNotObject, std::move(pointer));
        }
        return result;
    }

    template<Meta::Json J>
    [[maybe_unused]]
    bool handle(const J & json, const Handler<J> & handler, const PtrView pointer = {}) try {
        if (json.IsNull()) {
            return false;
        }
        return handler(json, pointer);
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<Meta::Json J>
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        const Handler<J> & handler,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle(json[kv(key)], handler, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<typename T, Meta::Json J>
    requires (!Meta::BackSideGrowingRange<T> && (std::is_copy_assignable_v<T> || std::is_move_assignable_v<T>))
    [[maybe_unused]]
    bool handle(const J & json, T & output, const PtrView pointer = {}) try {
        if (json.IsNull()) {
            return false;
        }
        output = cast<Meta::UnderlyingType<T>>(json);
        return true;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<typename T, Meta::Json J>
    requires (!Meta::BackSideGrowingRange<T> && (std::is_copy_assignable_v<T> || std::is_move_assignable_v<T>))
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle<T>(json[kv(key)], output, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<typename T, Meta::Json J, Meta::Filter<T> F>
    requires (!Meta::BackSideGrowingRange<T> && (std::is_copy_assignable_v<T> || std::is_move_assignable_v<T>))
    [[maybe_unused]]
    bool handle(const J & json, T & output, F && filter, const PtrView pointer = {}) try {
        if (json.IsNull()) {
            return false;
        }
        output = filter(cast<std::remove_cvref_t<T>>(json));
        return true;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<typename T, Meta::Json J, Meta::Filter<T> F>
    requires (!Meta::BackSideGrowingRange<T> && (std::is_copy_assignable_v<T> || std::is_move_assignable_v<T>))
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        F && filter,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle<T>(json[kv(key)], output, filter, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<typename T, Meta::Json J, Meta::Domain<T> D>
    [[maybe_unused]]
    bool handle(const J & json, T & output, const D & domain, const PtrView pointer = {}) try {
        if (json.IsNull()) {
            return false;
        }
        if constexpr (Meta::AnyText<T>) {
            output = Domain::cast<T>(cast<Text::String<T>>(json), domain);
        } else {
            output = Domain::cast<T>(cast<Meta::ValueType<D>>(json), domain);
        }
        return true;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<typename T, Meta::Json J, Meta::Domain<T> D>
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        const D & domain,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle<T>(json[kv(key)], output, domain, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<typename T, Meta::Json J, Meta::Domain<T> D, Meta::Filter<T> F>
    [[maybe_unused]]
    bool handle(const J & json, T & output, const D & domain, F && filter, const PtrView pointer = {}) try {
        if (json.IsNull()) {
            return false;
        }
        if (T value {}; handle(json, value, domain, pointer)) {
            output = filter(value);
            return true;
        }
        return false;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<typename T, Meta::Json J, Meta::Domain<T> D, Meta::Filter<T> F>
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        const D & domain,
        const F && filter,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle<T>(json[kv(key)], output, domain, filter, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<typename T, Meta::Json J, Meta::CastTo<T> M>
    [[maybe_unused]]
    bool handle(const J & json, T & output, const M & castMap, const PtrView pointer = {}) try {
        if (json.IsNull()) {
            return false;
        }
        if constexpr (Meta::AnyText<Meta::KeyType<M>>) {
            output = Dictionary::cast<T>(cast<Text::String<Meta::KeyType<M>>>(json), castMap, true);
        } else {
            output = Dictionary::cast<T>(cast<Meta::KeyType<M>>(json), castMap, true);
        }
        return true;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<typename T, Meta::Json J, Meta::CastTo<T> M>
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        const M & castMap,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle<T>(json[kv(key)], output, castMap, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<typename T, Meta::Json J, Meta::CastTo<T> M, Meta::Filter<T> F>
    [[maybe_unused]]
    bool handle(const J & json, T & output, const M & castMap, F && filter, const PtrView pointer = {}) try {
        if (json.IsNull()) {
            return false;
        }
        if (T value {}; handle(json, value, castMap, pointer)) {
            output = filter(value);
            return true;
        }
        return false;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<typename T, Meta::Json J, Meta::CastTo<T> M, Meta::Filter<T> F>
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        const M & castMap,
        F && filter,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle<T>(json[kv(key)], output, castMap, filter, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<Meta::BackSideGrowingRange T, Meta::Json J>
    [[maybe_unused]]
    bool handle(const J & json, T & output, const PtrView pointer = {}) try {
        if (!json.IsArray()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotArray, pointer);
        }
        for (SizeType i { 0 }; i < json.Size(); ++i) {
            output.push_back(cast<Meta::ValueType<T>>(json[i]));
        }
        return true;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<Meta::BackSideGrowingRange T, Meta::Json J>
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle(json[kv(key)], output, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<Meta::BackSideGrowingRange T, Meta::Json J, Meta::Filter<Meta::ValueType<T>> F>
    [[maybe_unused]]
    bool handle(const J & json, T & output, F && filter, const PtrView pointer = {}) try {
        if (!json.IsArray()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotArray, pointer);
        }
        for (SizeType i { 0 }; i < json.Size(); ++i) {
            output.push_back(filter(cast<Meta::ValueType<T>>(json[i])));
        }
        return true;
    } catch (DataError & e) {
        e.variable(pointer, true);
        throw;
    }

    template<Meta::BackSideGrowingRange T, Meta::Json J, Meta::Filter<Meta::ValueType<T>> F>
    [[maybe_unused]]
    bool handleKey(
        const J & json,
        const Key<J> key, // NOLINT
        T & output,
        F && filter,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (json.IsNull()) {
                return false;
            }
            throw DataError(Wcs::c_isNotObject, basePointer);
        }
        if (!json.HasMember(key)) {
            return false;
        }
        Ptr pointer { makePointer(basePointer, key) };
        try {
            return handle<T>(json[kv(key)], output, filter, pointer);
        } catch (DataError & e) {
            e.variable(std::move(pointer), true);
            throw;
        }
    }

    template<Meta::Json J>
    [[maybe_unused]]
    void getKey(
        J & json,
        const Key<J> key, // NOLINT
        auto & value,
        const PtrView basePointer = {}
    ) {
        if (!handleKey(json, key, value, basePointer)) {
            throw DataError(Wcs::c_keyNotFound, makePointer(basePointer, key), true);
        }
    }

    template<Meta::Json J, typename T, Meta::Domain<T> D>
    [[maybe_unused]]
    void getKey(
        J & json,
        const Key<J> key, // NOLINT
        T & value,
        const D & domain,
        const PtrView basePointer = {}
    ) {
        if (!handleKey(json, key, value, domain, basePointer)) {
            throw DataError(Wcs::c_keyNotFound, makePointer(basePointer, key), true);
        }
    }

    template<Meta::Json J, typename T, Meta::CastTo<T> M>
    [[maybe_unused]]
    void getKey(
        J & json,
        const Key<J> key, // NOLINT
        T & value,
        const M & castMap,
        const PtrView basePointer = {}
    ) {
        if (!handleKey(json, key, value, castMap, basePointer)) {
            throw DataError(Wcs::c_keyNotFound, makePointer(basePointer, key), true);
        }
    }

    template<Meta::JsonValue J>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        Value<J> && value,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        if (json.IsObject()) {
            if (auto it = json.FindMember(key); it != json.MemberEnd()) {
                it->value = std::move(value);
                return;
            }
        } else if (json.IsNull()) {
            json.SetObject();
        } else {
            throw DataError(Wcs::c_isNotObject, basePointer, true);
        }
        json.AddMember(key, std::move(value), allocator);
        assert(json.HasMember(key));
    }

    template<Meta::JsonDocument J>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        Value<J> && value,
        const PtrView basePointer = {}
    ) {
        setKey(json, key, std::move(value), json.GetAllocator(), basePointer);
    }

    template<Meta::JsonValue J, Meta::Arithmetic T>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        const T value,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        if (json.IsObject()) {
            if (auto it = json.FindMember(key); it != json.MemberEnd()) {
                it->value.Set(value);
                assert(it->value.IsNumber() || it->value.IsBool());
                return;
            }
        } else if (json.IsNull()) {
            json.SetObject();
        } else {
            throw DataError(Wcs::c_isNotObject, basePointer, true);
        }
        json.AddMember(key, Value<J>(value), allocator);
        assert(json.HasMember(key));
        assert(json[kv(key)].IsNumber() || json[kv(key)].IsBool());
    }

    template<Meta::JsonDocument J, Meta::Arithmetic T>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        const T value,
        const PtrView basePointer = {}
    ) {
        setKey(json, key, value, json.GetAllocator(), basePointer);
    }

    template<Meta::JsonValue J>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        const View<J> value,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        if (json.IsObject()) {
            if (auto it = json.FindMember(key); it != json.MemberEnd()) {
                it->value.SetString(value.data(), value.size(), allocator);
                assert(it->value.IsString());
                assert(it->value.GetStringLength() == value.size());
                return;
            }
        } else if (json.IsNull()) {
            json.SetObject();
        } else {
            throw DataError(Wcs::c_isNotObject, basePointer, true);
        }
        json.AddMember(key, Value<J> { value.data(), static_cast<SizeType>(value.size()), allocator }, allocator);
        assert(json.HasMember(key));
        assert(json[kv(key)].IsString());
        assert(json[kv(key)].GetStringLength() == value.size());
    }

    template<Meta::JsonDocument J>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        const View<J> value,
        const PtrView basePointer = {}
    ) {
        setKey(json, key, value, json.GetAllocator(), basePointer);
    }

    template<Meta::JsonValue J>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        const std::tm & value,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        if (!json.IsObject()) {
            if (!json.IsNull()) {
                throw DataError(Wcs::c_isNotObject, basePointer, true);
            }
            json.SetObject();
        }
        String<J> buffer { DateTime::cast<String<J>>(value)};
        auto it = json.FindMember(key);
        if (it == json.MemberEnd()) {
            json.AddMember(key, Value<J> { buffer.data(), static_cast<SizeType>(buffer.size()), allocator }, allocator);
        } else {
            it->value.SetString(buffer.data(), static_cast<SizeType>(buffer.size()), allocator);
        }
        assert(json.HasMember(key));
        assert(json[kv(key)].IsString());
        assert(json[kv(key)].GetStringLength() == buffer.size());
    }

    template<Meta::JsonDocument J>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        const std::tm & value,
        const PtrView basePointer = {}
    ) {
        setKey(json, key, value, json.GetAllocator(), basePointer);
    }

    template<Meta::JsonValue J, Meta::NotText V, Meta::Domain<V> D>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const D & domain,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        setKey(json, key, Domain::cast<V>(std::forward<V>(value), domain), allocator, basePointer);
    }

    template<Meta::JsonDocument J, Meta::NotText V, Meta::Domain<V> D>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const D & domain,
        const PtrView basePointer = {}
    ) {
        setKey(json, key, Domain::cast<V>(std::forward<V>(value), domain), json.GetAllocator(), basePointer);
    }

    template<Meta::JsonValue J, Meta::Text<Wideness<J>> V, Meta::TextDomain<Wideness<J>> D>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const D & domain,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        setKey(
            json, key,
            Domain::cast<String<V>>(std::forward<V>(value), domain),
            allocator, basePointer
        );
    }

    template<Meta::JsonDocument J, Meta::Text<Wideness<J>> V, Meta::TextDomain<Wideness<J>> D>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const D & domain,
        const PtrView basePointer = {}
    ) {
        setKey(
            json, key,
            Domain::cast<String<V>>(std::forward<V>(value), domain),
            json.GetAllocator(), basePointer
        );
    }

    template<Meta::JsonValue J, Meta::NotText V, Meta::CastFrom<V> M>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const M & castMap,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        setKey(
            json, key,
            Dictionary::cast<Meta::ValueType<M>>(std::forward<V>(value), castMap, true),
            allocator, basePointer
        );
    }

    template<Meta::JsonDocument J, Meta::NotText V, Meta::CastFrom<V> M>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const M & castMap,
        const PtrView basePointer = {}
    ) {
        setKey(
            json, key,
            Dictionary::cast<Meta::ValueType<M>>(std::forward<V>(value), castMap, true),
            json.GetAllocator(), basePointer
        );
    }

    template<Meta::JsonValue J, Meta::AnyText V, Meta::CastFromText<V> M>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const M & castMap,
        Allocator & allocator,
        const PtrView basePointer = {}
    ) {
        setKey(
            json, key,
            Dictionary::cast<Meta::ValueType<M>>(std::forward<V>(value), castMap, true),
            allocator, basePointer
        );
    }

    template<Meta::JsonDocument J, Meta::AnyText V, Meta::CastFromText<V> M>
    [[maybe_unused]]
    void setKey(
        J & json,
        const Key<J> key, // NOLINT
        V && value,
        const M & castMap,
        const PtrView basePointer = {}
    ) {
        setKey(
            json, key,
            Dictionary::cast<Meta::ValueType<M>>(std::forward<V>(value), castMap, true),
            json.GetAllocator(), basePointer
        );
    }
}
