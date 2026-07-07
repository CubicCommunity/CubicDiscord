#include <Cubic/Cubic.h>

using namespace cubic::prelude;

#ifdef CUBIC_LOCAL_BUILD
#define CUBIC_INTRO_SNOWFLAKE 1512802853784260638
#else
#define CUBIC_INTRO_SNOWFLAKE 937047909122314290
#endif

static constexpr dpp::snowflake g_introChannel = CUBIC_INTRO_SNOWFLAKE;

class IntroductionsEvent final : public base::EventHandler {
public:
    void init(dpp::cluster& bot) {
        bot.on_message_create([](dpp::message_create_t const& ev) -> dpp::task<void> {
            if (ev.msg.author.is_bot() || ev.msg.channel_id != g_introChannel) co_return;

            auto const msgsRes = co_await ev.owner->co_messages_get(g_introChannel, 0, 0, 0, 100);
            if (msgsRes.is_error()) {
                log::error("Failed to fetch messages from introductions channel: {}", msgsRes.get_error().message);
                co_return;
            };

            auto const msgs = msgsRes.get<std::unordered_map<dpp::snowflake, dpp::message>>();

            for (auto const& [sf, msg] : msgs) {
                if (ev.msg.author.id == msg.author.id && ev.msg.id != msg.id) {
                    ev.owner->message_delete(ev.msg.id, ev.msg.channel_id);

                    co_await ev.owner->co_direct_message_create(
                        ev.msg.author.id,
                        dpp::message()
                            .add_embed(
                                dpp::embed()
                                    .set_description(fmt::format(":exclamation: Looks like you've already introduced yourself here. If you'd like to update your introduction, **edit the [message]({})** you initially sent of it.", msg.get_url()))
                                    .set_color(theme::colors::secondary)));

                    break;
                };
            };

            co_return;
        });
    };
};

CUBIC_INIT(IntroductionsEvent);