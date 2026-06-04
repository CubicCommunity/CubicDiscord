#include <Cubic/Cubic.h>

using namespace cubic::prelude;

static constexpr auto crosspostingChannels = std::to_array<dpp::snowflake>({
#ifdef CUBIC_LOCAL_BUILD
    1412493998790021171,
#else
    942554670201720852,
#endif
});

class CrosspostEvent final : public base::EventHandler {
private:
    dpp::webhook m_webhook = dpp::webhook(env::get("CROSSPOST_WEBHOOK").value_or(""));

public:
    void init(dpp::cluster& bot) override {
        bot.on_message_create([this, &bot](dpp::message_create_t const& ev) -> dpp::task<void> {
            dpp::message const& msg = ev.msg;

            if (!(string::startsWith(msg.content, "<@!") || string::startsWith(msg.content, "<#"))) co_return;

            if (msg.author.id == bot.me.id) co_return;
            if (std::find(crosspostingChannels.begin(), crosspostingChannels.end(), msg.channel_id) == crosspostingChannels.end()) co_return;

            auto chnlMentions = message::extractChannels(msg.content);

            auto const content = [&msg]() {
                auto strs = asp::iter::split(msg.content, " ")
                                .mapCast<std::string>()
                                .collect();
                strs.erase(strs.begin());

                return string::join(std::move(strs), " ");
            };

            if (!chnlMentions.empty()) {
                auto channel = chnlMentions.front();
                auto const m = dpp::message(channel, content());

                co_await bot.co_message_create(m);
            } else if (!msg.mentions.empty()) {
                auto user = msg.mentions.front().second.user_id;
                auto const dm = dpp::message(content());

                co_await bot.co_direct_message_create(user, dm);
            } else {
                auto const m = dpp::message(content());
                co_await bot.co_execute_webhook(m_webhook, m);
            };
        });
    };
};

static CrosspostEvent ev;