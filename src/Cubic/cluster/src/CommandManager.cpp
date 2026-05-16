#include "../CommandManager.hpp"

#include <dpp/dpp.h>

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

void CommandManager::registerCommand(std::shared_ptr<Command> cmd) {
    m_commands[cmd->getCommand().name] = cmd;
};

void CommandManager::registerToDiscord(dpp::snowflake server) {
    std::vector<dpp::slashcommand> cmd_list;
    cmd_list.reserve(m_commands.size());

    for (auto& [name, cmd] : m_commands) cmd_list.push_back(cmd->getCommand());

    bot::get()->guild_bulk_command_create(cmd_list, server);
};

void CommandManager::clearFromDiscord(dpp::snowflake server) {
    m_commands.clear();
    bot::get()->guild_bulk_command_delete(server::id);
};

void CommandManager::handleCommand(dpp::slashcommand_t const& event) {
    auto const& name = event.command.get_command_name();

    if (event.command.type == dpp::interaction_type::it_application_command) {
        log::trace("Handling slash command /{}", name);

        if (auto it = m_commands.find(name); it != m_commands.end()) {
            log::debug("Found slash command /{}", name);
            it->second->getCallback()(event);
        } else {
            log::error("Failed to find slash command /{}", name);
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