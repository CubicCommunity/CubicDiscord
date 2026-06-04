#pragma once

#include <Cubic/std.hpp>

namespace cubic {
    namespace util {
        namespace string {
            std::string join(std::span<std::string const> strs, std::string_view separator);
            std::string join(std::span<std::string_view const> strs, std::string_view separator);

            bool startsWith(std::string_view str, std::string_view prefix);
            bool endsWith(std::string_view str, std::string_view suffix);
        };
    };
};