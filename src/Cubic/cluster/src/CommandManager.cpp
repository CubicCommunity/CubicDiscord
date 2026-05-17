#include "../CommandManager.hpp"

#include <dpp/dpp.h>

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

void CommandManager::registerAll(dpp::snowflake server) {
    if (queuedCommands().empty()) return log::critical("No commands found");

    for (auto& cmd : queuedCommands()) {
        auto name = cmd->name();

        log::debug("Integrating '{}'...", name);
        m_commands[std::move(name)] = std::move(cmd);
    };

    queuedCommands().clear();
    queuedCommands().shrink_to_fit();

    std::vector<dpp::slashcommand> cmds;
    cmds.reserve(m_commands.size());

    for (auto& [name, cmd] : m_commands) {
        log::trace("Building command '{}'...", cmd->name());
        cmds.push_back(cmd->build());
    };

    log::debug("Bulk registering {} commands to Discord...", cmds.size());

    Bot::get().guild_bulk_command_create(cmds, server);

    log::info("Command registration finished");
};

void CommandManager::handleCommand(dpp::slashcommand_t const& event) {
    auto const name = event.command.get_command_name();
    log::debug("Received command event for '{}'", name);

    if (event.command.type == dpp::interaction_type::it_application_command) {
        log::trace("Looking through {} entries to find '{}'...", m_commands.size(), name);

        if (auto it = m_commands.find(name); it != m_commands.end()) {
            log::debug("Found slash command '{}'", name);

            event.thinking(true);
            (void)it->second->handle(event);
        } else {
            log::error("Failed to find slash command '{}'", name);
            event.reply(
                dpp::message()
                    .add_embed(
                        dpp::embed()
                            .set_description(":x: Failed to find callback for this command.")
                            .set_color(theme::colors::secondary)));
        };
    } else {
        log::error("Interaction '{}' is not a slash command", name);
        event.reply(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_description(":x: Invalid interaction type, slash command expected.")
                        .set_color(theme::colors::secondary)));
    };
};

CommandQueue::CommandQueue(std::shared_ptr<base::Command> cmd) {
    queuedCommands().push_back(std::move(cmd));
};