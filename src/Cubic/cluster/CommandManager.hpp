#pragma once

#include <Cubic/base/Include.h>

namespace cubic {
    namespace cluster {
        class CommandManager final : public base::Singleton<CommandManager> {
        private:
            std::unordered_map<std::string, std::shared_ptr<base::Command>> m_commands;

        public:
            void registerCommand(std::shared_ptr<base::Command> cmd);
            void registerToDiscord(dpp::snowflake server);

            void clearFromDiscord(dpp::snowflake server);

            void handleCommand(dpp::slashcommand_t const& event);
        };
    };

    struct CommandRegister final {
        CommandRegister(std::shared_ptr<base::Command> cmd);
    };
};

#define CUBIC_REGISTER_COMMAND(T) \
    static cubic::CommandRegister reg(std::make_shared<T>())