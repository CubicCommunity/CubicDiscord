#include "../CommandManager.hpp"

#include <dpp/dpp.h>

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

void CommandManager::registerAll(dpp::cluster& bot, dpp::snowflake server) {
    auto& cmds = base::Command::getAll();
    if (cmds.empty()) return log::critical("No commands found");

    for (auto const& cmd : cmds) {
        auto name = cmd->name();

        log::debug("Integrating '{}'...", name);
        m_commands[std::move(name)] = cmd;
    };

    cmds.clear();
    cmds.shrink_to_fit();

    std::vector<dpp::slashcommand> commands;
    commands.reserve(m_commands.size());

    for (auto const& [name, cmd] : m_commands) {
        log::trace("Building command '{}'...", cmd->name());
        commands.push_back(cmd->build());
    };

    log::debug("Bulk registering {} commands to Discord...", commands.size());

    bot.guild_bulk_command_create(commands, server);

    log::info("Command registration finished");
};

dpp::task<void> CommandManager::handleCommand(dpp::slashcommand_t const& event) {
    auto const name = event.command.get_command_name();
    log::debug("Received command event for '{}'", name);

    if (event.command.type == dpp::interaction_type::it_application_command) {
        log::trace("Looking through {} entries to find '{}'...", m_commands.size(), name);

        if (auto it = m_commands.find(name); it != m_commands.end()) {
            log::debug("Found slash command '{}'", name);

            if (auto cmd = it->second) {
                event.thinking(cmd->ephemeral());
                co_await cmd->handle(event);
            } else {
                log::error("Command '{}' is null", name);
                co_await event.co_reply(
                    dpp::message()
                        .add_embed(
                            dpp::embed()
                                .set_description(":x: Command found but has no callback.")
                                .set_color(theme::colors::secondary)));
            };
        } else {
            log::error("Failed to find slash command '{}'", name);
            co_await event.co_reply(
                dpp::message()
                    .add_embed(
                        dpp::embed()
                            .set_description(":x: Failed to find callback for this command.")
                            .set_color(theme::colors::secondary)));
        };
    } else {
        log::error("Interaction '{}' is not a slash command", name);
        co_await event.co_reply(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_description(":x: Invalid interaction type, slash command expected.")
                        .set_color(theme::colors::secondary)));
    };

    co_return;
};