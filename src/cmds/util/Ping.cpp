#include <Cubic/Cubic.h>

using namespace cubic::prelude;

class PingCommand : public base::Command {
public:
    std::string name() const override {
        return "ping";
    };

    dpp::slashcommand build() const override {
        return dpp::slashcommand()
            .set_name("ping")
            .set_description("Ping the bot to test its speed.")
            .set_type(dpp::slashcommand_contextmenu_type::ctxm_chat_input)
            .set_interaction_contexts({
                dpp::interaction_context_type::itc_bot_dm,
                dpp::interaction_context_type::itc_guild,
            });
    };

    dpp::task<void> handle(dpp::slashcommand_t const& ev) override {
        auto const start = asp::Instant::now();

        co_await ev.co_reply(dpp::message(":ping_pong:"));

        auto const end = asp::Instant::now();
        auto dur = end.durationSince(start).millis();

        co_await ev.co_edit_original_response(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_title("Ping")
                        .set_color(theme::colors::primary)
                        .add_field(
                            "Latency",
                            fmt::format("{}ms", dur))
                        .add_field(
                            "API Latency",
                            fmt::format("{}ms", Bot::get().get_shard(0)->websocket_ping))));
    };
};

CUBIC_REGISTER_COMMAND(PingCommand);