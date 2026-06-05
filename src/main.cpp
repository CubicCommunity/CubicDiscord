#include <Cubic/Cubic.h>

using namespace cubic::prelude;

int main() {
    auto& bot = Bot::get();

    bot.on_log(dpp::utility::cout_logger());

#ifdef CUBIC_LOCAL_BUILD
    log::warn("Bot is running on local test build.");
#else
    log::debug("Bot is running on production build.");
#endif

    bot.on_slashcommand([](dpp::slashcommand_t const& event) -> dpp::task<void> {
        if (auto cm = CommandManager::get()) co_await cm->handleCommand(event);
        co_return;
    });

    bot.on_ready([](const dpp::ready_t& event) {
        log::debug("Ready event called");

        if (dpp::run_once<struct register_bot_commands>()) {
            log::info("Registering all commands");
            if (auto cm = CommandManager::get()) cm->registerAll(server::id);

            for (auto const& ev : base::EventHandler::getAll()) ev->init(Bot::get());
        };
    });

    bot.start(dpp::st_wait);
};