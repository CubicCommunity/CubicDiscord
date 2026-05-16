#pragma once

#include <fmt/core.h>

#include <dpp/dpp.h>

#include <Cubic/cluster/Bot.hpp>

namespace cubic {
    namespace log {
        namespace internal {
            inline void write(dpp::loglevel level, std::string_view str) {
                bot::get()->log(level, str.data());
            };
        };

        template <typename... T>
        inline void trace(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_trace, fmt::format(str, args...));
        };

        template <typename... T>
        inline void debug(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_debug, fmt::format(str, args...));
        };

        template <typename... T>
        inline void info(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_info, fmt::format(str, args...));
        };

        template <typename... T>
        inline void warn(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_warning, fmt::format(str, args...));
        };

        template <typename... T>
        inline void error(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_error, fmt::format(str, args...));
        };

        template <typename... T>
        inline void critical(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_critical, fmt::format(str, args...));
        };
    };
};