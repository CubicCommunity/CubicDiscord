#pragma once

#include <dpp/dpp.h>

#include <Cubic/std.hpp>

#include <Cubic/util/Macros.h>

namespace cubic {
    namespace base {
        struct Command {
        public:
            Command() { getAll().emplace_back(this); };
            virtual ~Command() = default;

            virtual CUBIC_CMD_FUNC_NAME_SIGN = 0;
            virtual CUBIC_CMD_FUNC_BUILD_SIGN = 0;
            virtual CUBIC_CMD_FUNC_HANDLE_SIGN = 0;

            virtual constexpr bool needsThinking() const noexcept { return false; };
            virtual constexpr bool ephemeral() const noexcept { return false; };

            static std::vector<Command*>& getAll() {
                static std::vector<Command*> evs;
                return evs;
            };
        };
    };
};