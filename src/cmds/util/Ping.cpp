#include <Cubic/Cubic.h>

using namespace cubic::prelude;

class PingCommand final : public base::Command {
public:
    std::string name() const noexcept override {
        return "ping";
    };

    dpp::slashcommand build() const override {
        return dpp::slashcommand()
            .set_name(name())
            .set_description("Ping the bot to test its speed.")
            .set_type(dpp::slashcommand_contextmenu_type::ctxm_chat_input)
            .set_interaction_contexts({
                dpp::interaction_context_type::itc_bot_dm,
                dpp::interaction_context_type::itc_guild,
            });
    };

    dpp::task<void> handle(dpp::slashcommand_t const& ev) override {
        auto const start = asp::Instant::now();

        auto res = co_await ev.co_reply(dpp::message(":ping_pong:"));
        if (res.is_error()) {
            log::error("Failed to send ping message: {}", res.get_error().message);
            ev.reply(
                dpp::message()
                    .add_embed(
                        dpp::embed()
                            .set_description(":x: Unknown error.")
                            .set_color(theme::colors::secondary)));

            co_return;
        };

        auto const end = asp::Instant::now();
        auto dur = end.durationSince(start).millis();

        log::debug("Ping command latency: {}ms", dur);

        ev.edit_original_response(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_title("Ping")
                        .set_color(theme::colors::primary)
                        .add_field("Latency", fmt::format("{}ms", dur))
                        .add_field("API Latency", fmt::format("{}ms", static_cast<int>(Bot::get().get_shard(0)->websocket_ping * 1000.0)))));

        co_return;
    };
};

static PingCommand cmd;