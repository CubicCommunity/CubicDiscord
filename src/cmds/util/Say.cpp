#include <Cubic/Cubic.h>

using namespace cubic::prelude;

class SayCommand final : public base::Command {
private:
    dpp::message formMessage(std::string_view txt, std::optional<dpp::snowflake> channel = std::nullopt, bool embed = true) const {
        if (embed) {
            return channel.has_value()
                       ? dpp::message(
                             channel.value_or(0),
                             dpp::embed()
                                 .set_description(txt)
                                 .set_color(theme::colors::primary))
                             .set_allowed_mentions()
                       : dpp::message()
                             .set_allowed_mentions()
                             .add_embed(
                                 dpp::embed()
                                     .set_description(txt)
                                     .set_color(theme::colors::primary));
        };

        return channel.has_value()
                   ? dpp::message(channel.value_or(0), txt)
                         .set_allowed_mentions()
                   : dpp::message(txt)
                         .set_allowed_mentions();
    };

protected:
    dpp::task<void> sendMessage(dpp::slashcommand_t const& ev, dpp::message const& msg, std::optional<dpp::snowflake> channel = std::nullopt) const {
        if (channel.has_value()) {
            auto const res = co_await ev.owner->co_message_create(msg);
            if (res.is_error()) {
                log::error("Failed to send message for say command: {}", res.get_error().message);
                co_await ev.co_reply(
                    dpp::message()
                        .add_embed(
                            dpp::embed()
                                .set_description(":x: Failed to send message.")
                                .set_color(theme::colors::secondary)));

                co_return;
            };

            co_await ev.co_reply(
                dpp::message()
                    .set_flags((ev.command.channel_id == channel.value_or(0)) ? dpp::m_ephemeral : 0)
                    .add_embed(
                        dpp::embed()
                            .set_description(":white_check_mark: Message sent.")
                            .set_color(theme::colors::primary)));

            co_return;
        };

        co_await ev.co_reply(msg);

        co_return;
    };

public:
    CUBIC_CMD_FUNC_NAME {
        return "say";
    };

    CUBIC_CMD_FUNC_BUILD {
        return dpp::slashcommand()
            .set_name(name())
            .set_description("Send a message to a channel.")
            .set_type(dpp::slashcommand_contextmenu_type::ctxm_chat_input)
            .set_interaction_contexts({
                dpp::interaction_context_type::itc_guild,
            })
            .set_default_permissions(dpp::permissions::p_manage_messages)
            .add_option(
                dpp::command_option(
                    dpp::co_string,
                    "message",
                    "The message to send to the channel.",
                    true))
            .add_option(
                dpp::command_option(
                    dpp::co_channel,
                    "channel",
                    "The channel to send the message to."))
            .add_option(
                dpp::command_option(
                    dpp::co_boolean,
                    "embed",
                    "Whether to enclose the message text into an embed."));
    };

    CUBIC_CMD_FUNC_HANDLE {
        auto const m = std::get<std::string>(ev.get_parameter("message"));

        auto embed = false;

        if (auto const p = ev.get_parameter("embed"); std::holds_alternative<bool>(p)) embed = std::get<bool>(p);

        if (auto const p = ev.get_parameter("channel"); std::holds_alternative<dpp::snowflake>(p)) {
            auto const c = std::get<dpp::snowflake>(p);

            auto chnlRes = co_await ev.owner->co_channel_get(c);
            if (chnlRes.is_error()) {
                log::error("Failed to get channel for say command: {}", chnlRes.get_error().message);
                co_await ev.co_reply(
                    dpp::message()
                        .add_embed(
                            dpp::embed()
                                .set_description(":x: Failed to find channel.")
                                .set_color(theme::colors::secondary)));

                co_return;
            };

            co_await sendMessage(ev, formMessage(m, c, embed), c);

            co_return;
        };

        co_await sendMessage(ev, formMessage(m, std::nullopt, embed));

        co_return;
    };
};

CUBIC_INIT_COMMAND(SayCommand);