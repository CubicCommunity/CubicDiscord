#include <Cubic/Cubic.h>

using namespace cubic::prelude;

static constexpr auto g_responses = std::to_array({
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
});

class EightBallCommand final : public base::Command {
public:
    std::string name() const noexcept override {
        return "8-ball";
    };

    dpp::slashcommand build() const override {
        return dpp::slashcommand()
            .set_name(name())
            .set_description("Ask the magic 8-ball a question.")
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

    dpp::task<void> handle(dpp::slashcommand_t const& ev) override {
        auto const q = std::get<std::string>(ev.get_parameter("question"));
        log::trace("Asked the 8-ball: {}", q);

        co_await ev.co_reply(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_title("Magic 8-Ball")
                        .set_color(theme::colors::primary)
                        .add_field("Question", q)
                        .add_field("Answer", g_responses[random::get(g_responses.size() - 1)])));

        co_return;
    };
};

static EightBallCommand cmd;