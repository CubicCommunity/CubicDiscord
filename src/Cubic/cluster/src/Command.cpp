#include "../Command.hpp"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

Command::Command(dpp::slashcommand cmd, Callback callback) : m_cmd(std::move(cmd)), m_callback(std::move(callback)) {};

std::shared_ptr<Command> Command::create(dpp::slashcommand cmd, Callback callback) {
    cmd.set_application_id(bot::get()->me.id);
    return std::make_shared<Command>(std::move(cmd), std::move(callback));
};

dpp::slashcommand const& Command::getCommand() const noexcept {
    return m_cmd;
};

Command::Callback Command::getCallback() const noexcept {
    return m_callback;
};

std::shared_ptr<Command> Command::autoRegister() & {
    if (auto cm = CommandManager::get()) cm->registerCommand(shared_from_this());
    return shared_from_this();
};