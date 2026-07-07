#include <Cubic/Cubic.h>

using namespace cubic::prelude;

static constexpr auto g_responses = std::to_array({
    "hell nah twin",
    "yas queen",
    "idfk",
    "It is certain.",
    "It is decidedly so.",
    "Without a doubt.",
    "Yes - definitely.",
    "You may rely on it.",
    "As I see it, yes.",
    "Most likely.",
    "Outlook good.",
    "Yes.",
    "Signs point to yes.",
    "Reply hazy, try again.",
    "Ask again later.",
    "Better not tell you now.",
    "Cannot predict now.",
    "Concentrate and ask again.",
    "Don't count on it.",
    "My reply is no.",
    "My sources say no.",
    "Outlook not so good.",
    "Very doubtful.",
    "Not a snowball's chance in hell.",
    "When pigs fly.",
    "If you believe hard enough!",
    "Anything is possible.",
    "Get Cubic's Discord server to 100,000 members and maybe I'll tell you!",
});

class EightBallCommand final : public base::Command {
public:
    CUBIC_CMD_FUNC_NAME {
        return "8-ball";
    };

    CUBIC_CMD_FUNC_BUILD {
        return dpp::slashcommand()
            .set_name(name())
            .set_description("Ask the magic 8-ball a yes-or-no question.")
            .set_type(dpp::slashcommand_contextmenu_type::ctxm_chat_input)
            .set_interaction_contexts({
                dpp::interaction_context_type::itc_bot_dm,
                dpp::interaction_context_type::itc_guild,
            })
            .add_option(
                dpp::command_option(
                    dpp::co_string,
                    "question",
                    "The question to ask the 8-ball.",
                    true));
    };

    CUBIC_CMD_FUNC_HANDLE {
        auto const q = std::get<std::string>(ev.get_parameter("question"));
        log::trace("Asked the 8-ball: {}", q);

        co_await ev.co_edit_original_response(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_title(":8ball: Magic 8-Ball")
                        .set_color(theme::colors::primary)
                        .add_field("Question", fmt::format("*{}*", q))
                        .add_field("Answer", fmt::format("**{}**", g_responses[random::get(g_responses.size() - 1)]))));

        co_return;
    };

    constexpr bool needsThinking() const noexcept override { return true; };
};

CUBIC_INIT(EightBallCommand);