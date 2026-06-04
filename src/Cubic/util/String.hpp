#pragma once

#include <Cubic/std.hpp>

namespace cubic {
    namespace util {
        namespace string {
            std::string join(std::span<std::string const> strs, std::string_view separator);
            std::string join(std::span<std::string_view const> strs, std::string_view separator);
        };
    };
};