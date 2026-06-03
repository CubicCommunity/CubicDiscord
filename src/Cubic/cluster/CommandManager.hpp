#pragma once

#include <Cubic/base/Include.h>

#include <Cubic/util/Include.h>

namespace cubic {
    namespace cluster {
        class CommandManager final : public base::Singleton<CommandManager> {
        private:
            StringMap<base::Command*> m_commands;

        public:
            void registerAll(dpp::snowflake server);

            dpp::task<void> handleCommand(dpp::slashcommand_t const& event);
        };
    };
};