#include <Cubic/Cubic.h>

using namespace cubic::prelude;

#ifdef CUBIC_LOCAL_BUILD
#define CUBIC_HONEYPOT_SNOWFLAKE 1514450472365330584
#else
#define CUBIC_HONEYPOT_SNOWFLAKE 1491247377447780503
#endif

static constexpr dpp::snowflake g_honeypotChannel = CUBIC_HONEYPOT_SNOWFLAKE;

class HoneypotEvent final : public base::EventHandler {
public:
    void init(dpp::cluster& bot) {
        bot.on_message_create([](dpp::message_create_t const& ev) -> dpp::task<void> {
            auto const& msg = ev.msg;

            if (msg.channel_id != g_honeypotChannel) co_return;

            log::warn("Handling message sent in honeypot channel");

            auto const channelRes = co_await ev.owner->co_channel_get(g_honeypotChannel);
            if (channelRes.is_error()) {
                log::error("Failed to find honeypot channel: {}", channelRes.get_error().message);
                co_return;
            };

            auto const channel = channelRes.get<dpp::channel>();

            auto const perms = channel.get_user_permissions(msg.member);

            if (!perms.has_any(dpp::p_manage_messages, dpp::p_bypass_slowmode, dpp::p_pin_messages)) {
                log::info("Suspicious user caught in honeypot");

                co_await ev.owner->co_message_delete(msg.id, msg.channel_id);

                auto const banRes = co_await ev.owner->co_guild_ban_add(msg.guild_id, msg.author.id, 604800);
                if (banRes.is_error()) {
                    log::error("Failed to ban honeypot catch: {}", banRes.get_error().message);
                    co_return;
                };

                co_await ev.owner->co_guild_ban_delete(msg.guild_id, msg.author.id);
            } else {
                log::debug("User caught in honeypot is staff");
            };

            co_return;
        });
    };
};

static HoneypotEvent ev;