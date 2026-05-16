#pragma once

#include <fmt/core.h>

#include <dpp/dpp.h>

#include <Cubic/cluster/Bot.hpp>

namespace cubic {
    namespace log {
        namespace internal {
            void write(dpp::loglevel level, std::string const& str);
        };

        template <typename... T>
        inline void trace(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_trace, fmt::format(str, std::forward<T>(args)...));
        };

        template <typename... T>
        inline void debug(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_debug, fmt::format(str, std::forward<T>(args)...));
        };

        template <typename... T>
        inline void info(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_info, fmt::format(str, std::forward<T>(args)...));
        };

        template <typename... T>
        inline void warn(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_warning, fmt::format(str, std::forward<T>(args)...));
        };

        template <typename... T>
        inline void error(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_error, fmt::format(str, std::forward<T>(args)...));
        };

        template <typename... T>
        inline void critical(fmt::format_string<T...> str, T&&... args) {
            internal::write(dpp::loglevel::ll_critical, fmt::format(str, std::forward<T>(args)...));
        };
    };
};