#include <Cubic/Cubic.h>

using namespace cubic::prelude;

class WelcomeTestCommand final : public base::Command {
public:
    CUBIC_CMD_FUNC_NAME {
        return "test-welcome";
    };

    CUBIC_CMD_FUNC_BUILD {
        return dpp::slashcommand()
            .set_name(name())
            .set_description("Test the server's welcome DM.")
            .set_type(dpp::slashcommand_contextmenu_type::ctxm_chat_input)
            .set_interaction_contexts({
                dpp::interaction_context_type::itc_guild,
            })
            .set_default_permissions(dpp::permissions::p_manage_messages);
    };

    CUBIC_CMD_FUNC_HANDLE {
        auto res = co_await ev.owner->co_direct_message_create(
            ev.command.usr.id,
            message::dm::welcome());

        if (res.is_error()) {
            log::error("Failed to send welcome DM: {}", res.get_error().message);
            co_await ev.co_reply(
                dpp::message()
                    .set_flags(dpp::message_flags::m_ephemeral)
                    .add_embed(
                        dpp::embed()
                            .set_description(":x: Failed to send welcome DM.")
                            .set_color(theme::colors::secondary)));

            co_return;
        };

        co_await ev.co_reply(
            dpp::message()
                .add_embed(
                    dpp::embed()
                        .set_description(":white_check_mark: The test welcome DM has been sent.")
                        .set_color(theme::colors::primary)));

        co_return;
    };
};

CUBIC_INIT_COMMAND(WelcomeTestCommand);