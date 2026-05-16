#pragma once

#include <dpp/dpp.h>

#include <Cubic/std.hpp>

namespace cubic {
    namespace cluster {
        struct Command final : std::enable_shared_from_this<Command> {
            using Callback = std::function<void(dpp::slashcommand_t const&)>;

        private:
            dpp::slashcommand m_cmd;
            Callback m_callback;

        public:
            Command(dpp::slashcommand cmd, Callback callback);

            static std::shared_ptr<Command> create(dpp::slashcommand cmd, Callback callback);

            dpp::slashcommand const& getCommand() const noexcept;
            Callback getCallback() const noexcept;

            std::shared_ptr<Command> autoRegister() &;
        };
    };
};