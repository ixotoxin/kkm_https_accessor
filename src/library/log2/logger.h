// Copyright (c) 2026 Vitaly Anasenko
// Distributed under the MIT License, see accompanying file LICENSE.txt

#pragma once

#include "core.h"
#include <constants.h>
#include <lib/msgbuild.h>
#include <memory>

namespace Log {
    class CategoryLogger;

    using LoggerPtr = std::shared_ptr<CategoryLogger>;

    class CategoryLogger {
        std::wstring m_prefix {};
        Category m_category;

    public:
        CategoryLogger() = delete;
        CategoryLogger(const CategoryLogger &) noexcept = default;
        CategoryLogger(CategoryLogger &&) noexcept = default;

        explicit CategoryLogger(const Category category, const std::wstring_view prefix = {}) noexcept
        : m_prefix { prefix }, m_category { category } {}

        CategoryLogger(const Category category, std::wstring && prefix) noexcept
        : m_prefix { std::move(prefix) }, m_category { category } {}

        CategoryLogger(const Category category, Basic::Wcs::Message && prefix) noexcept
        : m_prefix { std::move(prefix.m_message) }, m_category { category } {}

        explicit CategoryLogger(CategoryLogger & parent, const std::wstring_view prefix = {}) noexcept // NOLINT
        : m_prefix { Text::concat<c_xsStrSize>(parent.m_prefix, prefix) }, m_category { parent.m_category } {}

        CategoryLogger(CategoryLogger & parent, const Category category, const std::wstring_view prefix = {}) noexcept // NOLINT
        : m_prefix { Text::concat<c_xsStrSize>(parent.m_prefix, prefix) }, m_category { category } {}

        virtual ~CategoryLogger() = default;

        CategoryLogger & operator=(const CategoryLogger &) = delete;
        CategoryLogger & operator=(CategoryLogger &&) = delete;

        template<typename ... Args>
        void appendPrefix(const std::wstring_view prefix, Args && ... args) {
            if constexpr (sizeof...(Args) > 0) {
                std::vformat_to(std::back_inserter(m_prefix), prefix, std::make_wformat_args(args...));
            } else {
                m_prefix.append(prefix);
            }
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void debug(const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Debug, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void debug(const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Debug, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void debug(const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Debug, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void debug(const SrcLoc::Point & location, const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Debug, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void debug(const SrcLoc::Point & location, const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Debug, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void debug(const SrcLoc::Point & location, const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Debug, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void info(const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Info, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void info(const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Info, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void info(const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Info, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void info(const SrcLoc::Point & location, const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Info, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void info(const SrcLoc::Point & location, const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Info, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void info(const SrcLoc::Point & location, const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Info, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void warning(const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Warning, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void warning(const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Warning, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void warning(const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Warning, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void warning(const SrcLoc::Point & location, const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Warning, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void warning(const SrcLoc::Point & location, const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Warning, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void warning(const SrcLoc::Point & location, const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Warning, m_prefix, message, std::forward<Args>(args)...
            );
        }

        void warning(const Basic::Failure & e) const noexcept {
            write(m_category, Level::Warning, m_prefix, e);
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void error(const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Error, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void error(const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Error, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void error(const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                m_category, Level::Error, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::View T, typename ... Args>
        [[maybe_unused]]
        void error(const SrcLoc::Point & location, const T message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Error, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::Char T, typename ... Args>
        [[maybe_unused]]
        void error(const SrcLoc::Point & location, const T * message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Error, m_prefix, message, std::forward<Args>(args)...
            );
        }

        template<Meta::String T, typename ... Args>
        [[maybe_unused]]
        void error(const SrcLoc::Point & location, const T & message, Args && ... args) const noexcept {
            write<typename Meta::TextTrait<T>::View>(
                location, m_category, Level::Error, m_prefix, message, std::forward<Args>(args)...
            );
        }

        void error(const Basic::Failure & e) const noexcept {
            write(m_category, Level::Error, m_prefix, e);
        }
    };
}
