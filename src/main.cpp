#include <Cubic/Cubic.h>

#include <openssl/bn.h>

using namespace cubic::prelude;

static auto cm = CommandManager::get();  // static init

void dpp::bignumber::bn_deleter::operator()(struct dpp::openssl_bignum* bn) const noexcept {
    if (bn) BN_free(reinterpret_cast<BIGNUM*>(bn));
};

int main() {
    auto& bot = Bot::get();

    bot.on_log(dpp::utility::cout_logger());

#ifdef CUBIC_LOCAL_BUILD
    log::warn("Bot is running on local test build.");
#else
    log::debug("Bot is running on production build.");
#endif

    bot.on_slashcommand([](dpp::slashcommand_t const& event) {
        if (auto cm = CommandManager::get()) cm->handleCommand(event);
    });

    bot.on_ready([](const dpp::ready_t& event) {
        log::debug("Ready event called");

        if (dpp::run_once<struct clear_bot_commands>()) {
            log::info("Clearing all commands");
            if (auto cm = CommandManager::get()) cm->clearFromDiscord(server::id);
        };

        if (dpp::run_once<struct register_bot_commands>()) {
            log::info("Registering all commands");
            if (auto cm = CommandManager::get()) cm->registerToDiscord(server::id);
        };
    });

    bot.start(dpp::st_wait);
};