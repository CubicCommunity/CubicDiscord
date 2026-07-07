#include <Cubic/Cubic.h>

using namespace cubic::prelude;

class PingCommand final : public base::Command {
public:
    CUBIC_CMD_FUNC_NAME {
        return "ping";
    };

    CUBIC_CMD_FUNC_BUILD {
        return dpp::slashcommand()
            .set_name(name())
            .set_description("Ping the bot to test its speed.")
            .set_type(dpp::slashcommand_contextmenu_type::ctxm_chat_input)
            .set_interaction_contexts({
                dpp::interaction_context_type::itc_bot_dm,
                dpp::interaction_context_type::itc_guild,
            });
    };

    CUBIC_CMD_FUNC_HANDLE {
        auto const start = asp::Instant::now();

        auto res = co_await ev.co_reply(dpp::message(":ping_pong:"));
        if (res.is_error()) {
            log::error("Failed to send ping message: {}", res.get_error().message);
            co_await ev.co_reply(
                dpp::message()
                    .add_embed(
                        dpp::embed()
                            .set_description(":x: Unknown error.")
                            .set_color(theme::colors::secondary)));

            co_return;
        };

        auto const end = asp::Instant::now();
        auto dur = end.durationSince(start).millis();

        log::trace("Ping command latency: {}ms", dur);

        co_await ev.co_edit_original_response(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_title("Ping")
                        .set_color(theme::colors::primary)
                        .add_field("Latency", fmt::format("{}ms", dur))
                        .add_field("API Latency", fmt::format("{}ms", static_cast<uint64_t>(ev.owner->rest_ping * 1000.0)))));

        co_return;
    };
};

CUBIC_INIT(PingCommand);