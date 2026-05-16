#include <Cubic/Cubic.h>

using namespace cubic::prelude;

int main() {
    auto& bot = bot::get();

    bot->on_log(dpp::utility::cout_logger());

    bot->on_slashcommand([](dpp::slashcommand_t const& event) {
        if (auto cm = CommandManager::get()) cm->handleCommand(event);
    });

    bot->on_ready([](const dpp::ready_t& event) {
        if (dpp::run_once<struct clear_bot_commands>()) {
            if (auto cm = CommandManager::get()) cm->clearFromDiscord(server::id);
        };

        if (dpp::run_once<struct register_bot_commands>()) {
            if (auto cm = CommandManager::get()) cm->registerToDiscord(server::id);
        };
    });

    bot->start(dpp::st_wait);
};