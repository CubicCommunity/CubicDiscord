#include <Cubic/Cubic.h>

using namespace cubic::prelude;

#ifdef CUBIC_LOCAL_BUILD
#define CUBIC_CROSSPOST_SNOWFLAKE 1412493998790021171
#else
#define CUBIC_CROSSPOST_SNOWFLAKE 942554670201720852
#endif

static constexpr dpp::snowflake g_crosspostingChannel = CUBIC_CROSSPOST_SNOWFLAKE;

class CrosspostEvent final : public base::EventHandler {
private:
    dpp::webhook m_webhook = dpp::webhook(env::get("CROSSPOST_WEBHOOK").value_or(""));

    std::string getContent(dpp::message const& msg) const {
        auto strs = asp::iter::split(msg.content, " ")
                        .mapCast<std::string>()
                        .collect();
        strs.erase(strs.begin());

        return string::join(std::move(strs), " ");
    };

public:
    void init(dpp::cluster& bot) override {
        bot.on_message_create([this, &bot](dpp::message_create_t const& ev) -> dpp::task<void> {
            auto const& msg = ev.msg;

            if (msg.author.id == bot.me.id) co_return;
            if (ev.msg.channel_id != g_crosspostingChannel) co_return;

            if (string::startsWith(msg.content, "<@") || string::startsWith(msg.content, "<#")) {
                log::debug("Scanning crosspost message by #{} ({})", msg.author.username, msg.author.id);

                auto const chnlMentions = message::extractChannels(msg.content);
                if (!chnlMentions.empty()) {
                    auto channel = chnlMentions.front();
                    auto const m = dpp::message(channel, getContent(msg));

                    log::info("Crosspost message by #{} ({}) contains mention for channel of ID {}", msg.author.username, msg.author.id, channel);

                    co_await bot.co_message_create(m);
                } else if (!msg.mentions.empty()) {
                    auto user = msg.mentions.front().second.user_id;
                    auto const dm = dpp::message(getContent(msg));

                    log::info("Crosspost message by #{} ({}) contains mention for user of ID {}", msg.author.username, msg.author.id, user);

                    co_await bot.co_direct_message_create(user, dm);
                } else {
                    log::error("Crosspost message by #{} ({}) contained no mentions", msg.author.username, msg.author.id);
                };
            } else {
                log::warn("Crosspost message by @{} ({}) contains no mention at first argument", msg.author.username, msg.author.id);

                auto const m = dpp::message(msg.content);
                co_await bot.co_execute_webhook(m_webhook, m);
            };
        });
    };
};

static CrosspostEvent ev;