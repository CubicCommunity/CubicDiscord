#pragma once

#include <dpp/dpp.h>

#include <Cubic/std.hpp>

namespace cubic {
    namespace base {
        struct Command {
        public:
            virtual ~Command() = default;

            virtual std::string name() const = 0;
            virtual dpp::slashcommand build() const = 0;
            virtual dpp::task<void> handle(dpp::slashcommand_t const& ev) = 0;

            virtual bool ephemeral() const { return false; };
        };
    };
};