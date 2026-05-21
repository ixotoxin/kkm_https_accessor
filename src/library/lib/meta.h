// Copyright (c) 2025-2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include <type_traits>
#include <concepts>
#include <ranges>
#include <string>

namespace Meta {
    template<typename T>
    using UnderlyingType = std::remove_cvref_t<T>;

    #define classTplt template<typename ...> class

    template<classTplt, classTplt> struct templatesComparing : std::false_type {};
    template<classTplt T> struct templatesComparing<T, T> : std::true_type {};

    template<classTplt T, classTplt U>
    [[maybe_unused]]
    inline constexpr bool isSameTemplate = templatesComparing<T, U>::value;

    template<class T, classTplt U>
    concept fromTemplate = requires(T t) { [] <typename ... V>(U<V ...> &) {} (t); };

    namespace Detail {
        template<typename ...>
        struct Rebind;

        template<template<typename ...> class Tmpl, typename ... SrcArgs>
        struct Rebind<Tmpl<SrcArgs...>> {
            template<typename ... NewArgs>
            using With = Tmpl<NewArgs...>;
        };
    }

    template<class T, typename ... NewTypes>
    using Rebind = Detail::Rebind<T>::template With<NewTypes...>;

    template<class T>
    constexpr std::underlying_type_t<T> toUnderlying(T e) noexcept {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    namespace Detail {
        template<typename>
        struct ValueType {};

        template<typename T>
        requires std::is_array_v<T>
        struct ValueType<T> { using Type = std::remove_all_extents_t<T>; };

        template<typename T>
        requires std::is_pointer_v<T>
        struct ValueType<T> { using Type = std::remove_pointer_t<T>; };

        template<typename T>
        requires (requires { typename T::value_type; } && !requires { typename T::mapped_type; })
        struct ValueType<T> { using Type = T::value_type; };

        template<typename T>
        requires requires { typename T::mapped_type; }
        struct ValueType<T> { using Type = T::mapped_type; };
    }

    template<typename T>
    requires requires { typename T::size_type; }
    using SizeType = UnderlyingType<typename UnderlyingType<T>::size_type>;

    template<typename T>
    using ValueType = UnderlyingType<typename Detail::ValueType<UnderlyingType<T>>::Type>;

    template<typename T>
    requires requires { typename T::key_type; }
    using KeyType = UnderlyingType<typename UnderlyingType<T>::key_type>;

    template<typename T>
    constexpr bool isBool = std::is_same_v<T, bool>;

    template<typename T>
    concept Bool = isBool<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isInt8 = !std::is_same_v<T, bool> && std::integral<T> && sizeof(T) == 1;

    template<typename T>
    concept Int8 = isInt8<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isInt16 = !std::is_same_v<T, bool> && std::integral<T> && sizeof(T) == 2;

    template<typename T>
    concept Int16 = isInt16<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isInt32 = !std::is_same_v<T, bool> && std::integral<T> && sizeof(T) == 4;

    template<typename T>
    concept Int32 = isInt32<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isInt64 = !std::is_same_v<T, bool> && std::integral<T> && sizeof(T) == 8;

    template<typename T>
    concept Int64 = isInt64<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isIntegral = !std::is_same_v<T, bool> && std::integral<T>;

    template<typename T>
    concept Integral = isIntegral<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isFloatingPoint = std::is_same_v<T, double> || std::is_same_v<T, long double>;

    template<typename T>
    concept FloatingPoint = isFloatingPoint<UnderlyingType<T>>;

    template<typename T>
    concept Numeric = Integral<T> || FloatingPoint<T>;

    template<typename T>
    concept Arithmetic = Bool<T> || Numeric<T>;

    template<typename T>
    constexpr bool isChar = std::is_same_v<T, wchar_t> || std::is_same_v<std::remove_cvref_t<T>, char>;

    template<typename T>
    concept Char = isChar<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isString = std::is_same_v<T, std::wstring> || std::is_same_v<T, std::string>;

    template<typename T>
    concept String = isString<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isView = std::is_same_v<T, std::wstring_view> || std::is_same_v<T, std::string_view>;

    template<typename T>
    concept View = isView<UnderlyingType<T>>;

    template<typename T>
    constexpr bool isCString
        = (std::is_array_v<T> && isChar<std::remove_cv_t<std::remove_extent_t<T>>>)
          || (std::is_pointer_v<T> && isChar<std::remove_cv_t<std::remove_pointer_t<T>>>);

    template<typename T>
    constexpr bool isCppString = isString<T> || isView<T>;

    /*template<typename T>
    constexpr bool isNotTextClass
        = !requires (T t) {
            t.substr();
        };*/

    template<typename T>
    constexpr bool isNotText = !isCString<T> && !isCppString<T>;

    template<typename T>
    concept NotText = isNotText<UnderlyingType<T>>;

    template<typename>
    struct CastTrait { using Type = void; };

    template<>
    struct CastTrait<signed char> { using Type = signed long; };

    template<>
    struct CastTrait<unsigned char> { using Type = unsigned long; };

    template<>
    struct CastTrait<signed short> { using Type = signed long; };

    template<>
    struct CastTrait<unsigned short> { using Type = unsigned long; };

    template<>
    struct CastTrait<signed int> { using Type = signed long; };

    template<>
    struct CastTrait<unsigned int> { using Type = unsigned long; };

    template<>
    struct CastTrait<signed long> { using Type = signed long; };

    template<>
    struct CastTrait<unsigned long> { using Type = unsigned long; };

    template<>
    struct CastTrait<signed long long> { using Type = signed long long; };

    template<>
    struct CastTrait<unsigned long long> { using Type = unsigned long long; };

    struct Wcs {};
    struct Mbs {};

    template<typename T>
    concept Wideness = std::is_same_v<T, Wcs> || std::is_same_v<T, Mbs>;

    template<typename>
    struct WideTypes : std::false_type {};

    template<>
    struct WideTypes<Wcs> : std::true_type {};

    template<>
    struct WideTypes<wchar_t> : std::true_type {};

    template<>
    struct WideTypes<wchar_t *> : std::true_type {};

    template<>
    struct WideTypes<const wchar_t *> : std::true_type {};

    template<>
    struct WideTypes<std::wstring> : std::true_type {};

    template<>
    struct WideTypes<std::wstring_view> : std::true_type {};

    template<typename T>
    constexpr bool isWide = WideTypes<UnderlyingType<T>>::value;

    struct DaNet {};
    struct YesNo {};
    struct EnaDis {};
    struct TrueFalse {};

    template<typename T>
    concept BoolTag
        = std::is_same_v<T, DaNet> || std::is_same_v<T, YesNo>
          || std::is_same_v<T, EnaDis> || std::is_same_v<T, TrueFalse>;

    template<Wideness, BoolTag>
    struct BoolLabels {};

    template<typename T>
    concept BackSideGrowingRange
        = std::ranges::range<T>
          && requires (T t, ValueType<T> v, SizeType<T> s) {
                t.clear();
                t.emplace_back(v);
                t.push_back(v);
             };

    template<typename T>
    constexpr bool isReservingRange
        = requires (T t, SizeType<T> s) {
            t.reserve(s);
          };

    template<typename T, typename V>
    constexpr bool isSetOf
        = requires (T t, V v) {
            { std::ranges::find(t, v) == t.end() } -> std::same_as<bool>;
          }
          && isNotText/*Class*/<T>;

    template<typename T, typename K, typename V>
    constexpr bool isMapOf
        = requires (T t, K k, V v) {
            { t.find(k) == t.end() } -> std::same_as<bool>;
            V(t.find(k)->second);
            v = t.find(k)->second;
          }
          && isNotText/*Class*/<T>;

    template<typename T, typename I>
    concept Domain = isSetOf<T, I>;

    template<typename T, typename K, typename V>
    concept CastMap = isMapOf<T, K, V>;

    template<typename T, typename V>
    concept CastTo = CastMap<T, KeyType<T>, V>;

    template<typename T, typename K>
    concept CastFrom = CastMap<T, K, ValueType<T>>;

    template<typename T, typename U>
    concept Filter
        = std::is_invocable_r_v<
            U, T,
            std::conditional_t<
                std::is_scalar_v<U>,
                std::conditional_t<std::is_volatile_v<U>, const volatile U, const U>,
                std::conditional_t<std::is_volatile_v<U>, const volatile U &, const U &>
            >
          >;
}
