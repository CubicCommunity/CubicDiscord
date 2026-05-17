#pragma once

#include <Cubic/base/Include.h>

namespace cubic {
    namespace cluster {
        inline std::vector<std::shared_ptr<base::Command>>& queuedCommands() {
            static std::vector<std::shared_ptr<base::Command>> cmds;
            return cmds;
        };

        class CommandManager final : public base::Singleton<CommandManager> {
        private:
            std::unordered_map<std::string, std::shared_ptr<base::Command>> m_commands;

        public:
            void registerAll(dpp::snowflake server);

            void handleCommand(dpp::slashcommand_t const& event);
        };
    };

    struct CommandQueue final {
        CommandQueue(std::shared_ptr<base::Command> cmd);
    };
};

#define CUBIC_QUEUE_COMMAND(T) \
    static cubic::CommandQueue reg(std::make_shared<T>())