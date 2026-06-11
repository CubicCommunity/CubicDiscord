#pragma once

#include <dpp/dpp.h>

#include <Cubic/std.hpp>

namespace cubic {
    namespace base {
        struct Command {
        public:
            Command() { getAll().emplace_back(this); };
            virtual ~Command() = default;

            virtual std::string name() const noexcept = 0;
            virtual dpp::slashcommand build() const = 0;
            virtual dpp::task<void> handle(dpp::slashcommand_t const& ev) = 0;

            virtual constexpr bool needsThinking() const noexcept { return false; };
            virtual constexpr bool ephemeral() const noexcept { return false; };

            static std::vector<Command*>& getAll() {
                static std::vector<Command*> evs;
                return evs;
            };
        };
    };
};