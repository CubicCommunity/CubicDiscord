#pragma once

#include "Command.hpp"

#include <Cubic/base/Singleton.hpp>

namespace cubic {
    namespace cluster {
        class CommandManager final : public base::Singleton<CommandManager> {
        private:
            std::unordered_map<std::string, std::shared_ptr<Command>> m_commands;

        public:
            void registerCommand(std::shared_ptr<Command> cmd);
            void registerToDiscord(dpp::snowflake server);

            void clearFromDiscord(dpp::snowflake server);

            void handleCommand(dpp::slashcommand_t const& event);
        };
    };
};