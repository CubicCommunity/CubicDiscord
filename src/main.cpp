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

    bot.on_slashcommand([](dpp::slashcommand_t const& ev) -> dpp::task<void> {
        if (auto cm = CommandManager::get()) co_await cm->handleCommand(ev);
        co_return;
    });

    bot.on_ready([&bot](dpp::ready_t const& ev) -> dpp::task<void> {
        log::trace("Ready event called");

        if (dpp::run_once<struct register_bot_commands>()) {
            auto const getUsername = [&bot, &ev]() {
                auto evName = ev.owner->me.username;
                return evName.empty() ? bot.me.username : std::move(evName);
            };

            auto const getAvatarURL = [&bot, &ev](dpp::image_type format = dpp::i_png) {
                auto evUrl = ev.owner->me.get_avatar_url(512, format);
                return evUrl.empty() ? bot.me.get_avatar_url(512, format) : std::move(evUrl);
            };

            log::info("Registering all commands");
            if (auto cm = CommandManager::get()) cm->registerAll(*ev.owner, server::id);

            log::info("Initializing all event listeners");
            for (auto const& e : base::EventHandler::getAll()) e->init(*ev.owner);

            auto wh = Bot::getDevWebhook();
            wh.avatar_url = getAvatarURL();

#ifdef CUBIC_LOCAL_BUILD
#define CUBIC_AVATAR_FORMAT dpp::i_png
#else  // the test bot's avatar isn't animated lol
#define CUBIC_AVATAR_FORMAT dpp::i_gif
#endif

            co_await ev.owner->co_execute_webhook(
                wh,
                dpp::message()
                    .add_embed(
                        dpp::embed()
                            .set_author("Service Status", "", "")
                            .set_description(fmt::format(":white_check_mark: **{}** is now __online__", getUsername()))
                            .set_color(theme::colors::primary)
                            .set_footer(getUsername(), getAvatarURL(CUBIC_AVATAR_FORMAT))));
        };

        co_return;
    });

    bot.start(dpp::st_wait);
};