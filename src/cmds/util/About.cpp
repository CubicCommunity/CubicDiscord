#include <Cubic/Cubic.h>

using namespace cubic::prelude;

class AboutCommand final : public base::Command {
public:
    std::string name() const noexcept override {
        return "about";
    };

    dpp::slashcommand build() const override {
        return dpp::slashcommand()
            .set_name(name())
            .set_description("Receive information about the current build of this bot.")
            .set_type(dpp::slashcommand_contextmenu_type::ctxm_chat_input)
            .set_interaction_contexts({
                dpp::interaction_context_type::itc_bot_dm,
                dpp::interaction_context_type::itc_guild,
            });
    };

    dpp::task<void> handle(dpp::slashcommand_t const& ev) override {
        auto const& bot = *ev.owner;

        co_await ev.co_reply(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_author("About", "", "")
                        .set_title(fmt::format("CubicDiscord `{}`", Version::get()->getVersionString()))
                        .set_description(fmt::format("Running as Discord bot client **`{}`**`#{}` (`{}`) on shard **#{}**", bot.me.username, bot.me.discriminator, bot.me.id, ev.shard))
                        .set_color(theme::colors::primary)
                        .add_field(
                            "C++ Standard",
                            "**`23`**",
                            true)
                        .add_field(
                            "Uptime",
                            fmt::format("{} • <t:{}:R>", Version::getUptime().toHumanString(), Version::getStartupUnix()),
                            true)
                        .add_field(
                            "Help Development",
                            "<:Kofi:1512876181224161343> **[Support Cheeseworks on Ko-fi](https://kofi.cheeseworks.gay/)!**")
                        .set_footer(bot.me.username, bot.me.get_avatar_url(512, CUBIC_AVATAR_FORMAT))));

        co_return;
    };
};

static AboutCommand ev;