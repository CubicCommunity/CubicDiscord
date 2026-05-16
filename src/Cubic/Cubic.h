#pragma once

#include <dpp/dpp.h>

#include <fmt/core.h>

#include <Cubic/std.hpp>

#include <Cubic/base/Include.h>
#include <Cubic/cluster/Include.h>
#include <Cubic/util/Include.h>

namespace cubic {
    namespace fs = asp::fs;

    namespace env {
        inline std::optional<std::string> get(const char* name) {
#ifdef _WIN32
            char* value = nullptr;
            size_t len = 0;

            if (_dupenv_s(&value, &len, name) == 0 && value) {
                std::string result(value);
                free(value);
                return result;
            };

            return std::nullopt;
#else
            const char* value = std::getenv(name);
            if (value)
                return std::string(value);
            return std::nullopt;
#endif
        };
    };

    namespace server {
#ifdef CUBIC_LOCAL_BUILD
        constexpr dpp::snowflake id = 1330925786634522787;
#else
        constexpr dpp::snowflake id = 460081436637134859;
#endif
    };

    namespace prelude {
        using namespace ::cubic;
        using namespace ::cubic::cluster;
    };
};