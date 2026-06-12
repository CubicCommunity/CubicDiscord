#include <Cubic/Cubic.h>

using namespace cubic::prelude;

int main() {
    auto& bot = Bot::get();

    bot.on_log(dpp::utility::cout_logger());
    bot.on_log([](dpp::log_t const& ev) {
        if (ev.owner && ev.severity >= dpp::loglevel::ll_error) {
            auto now = asp::SystemTime::now().timeSinceEpoch().seconds();

            auto web = Bot::getDevWebhook();
            web.avatar_url = ev.owner->me.get_avatar_url(512);

            ev.owner->execute_webhook(
                web,
                dpp::message()
                    .add_embed(
                        dpp::embed()
                            .set_author("Shard Error", "", "")
                            .set_description(fmt::format("```{}```", ev.message))
                            .set_color(theme::colors::secondary)
                            .add_field(
                                "Shard",
                                fmt::format("**`{}`**", ev.shard))
                            .add_field(
                                "Time of Error",
                                fmt::format("<t:{}:F> • <t:{}:R>", now, now))
                            .set_footer(ev.owner->me.username, web.avatar_url = ev.owner->me.get_avatar_url(512, CUBIC_AVATAR_FORMAT))));
        };
    });

#ifdef CUBIC_LOCAL_BUILD
    log::warn("Bot is running on local test build.");
#else
    log::info("Bot is running on production build.");
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

            log::debug("Registering all commands");
            if (auto cm = CommandManager::get()) cm->registerAll(*ev.owner, server::id);

            log::debug("Initializing all event listeners");
            for (auto const& e : base::EventHandler::getAll()) e->init(*ev.owner);

            log::info("Cubic bot operating under {} is now online!", ev.owner->me.format_username());

            auto web = Bot::getDevWebhook();
            web.avatar_url = getAvatarURL();

            co_await ev.owner->co_execute_webhook(
                web,
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