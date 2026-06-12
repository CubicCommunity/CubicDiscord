#include <Cubic/Cubic.h>

using namespace cubic::prelude;

#ifdef CUBIC_LOCAL_BUILD
#define CUBIC_WELCOME_SNOWFLAKE 1512802838026260602
#else
#define CUBIC_WELCOME_SNOWFLAKE 1412493998790021171
#endif

static constexpr dpp::snowflake g_welcomeChannel = CUBIC_WELCOME_SNOWFLAKE;

class WelcomeEvent final : public base::EventHandler {
private:
    std::unordered_map<dpp::snowflake, dpp::guild_member> m_cache;

public:
    void init(dpp::cluster& bot) override {
        bot.on_guild_member_add([this](dpp::guild_member_add_t const& ev) {
            if (ev.adding_guild.id != server::id) return;

            if (!ev.added.has_completed_onboarding()) m_cache[ev.added.user_id] = ev.added;
        });

        bot.on_guild_member_remove([this](dpp::guild_member_remove_t const& ev) {
            if (ev.removing_guild.id != server::id) return;

            auto const it = m_cache.find(ev.removed.id);
            if (it == m_cache.end()) return;

            m_cache.erase(it);
        });

        bot.on_guild_member_update([this](dpp::guild_member_update_t const& ev) -> dpp::task<void> {
            if (ev.updating_guild.id != server::id) co_return;

            auto const& newM = ev.updated;

            auto const it = m_cache.find(ev.updated.user_id);
            if (it == m_cache.end()) co_return;

            auto const& oldM = it->second;

            if (newM.has_completed_onboarding() && !oldM.has_completed_onboarding()) {
                log::info("User {} has completed onboarding!", newM.get_user()->global_name);

                auto const dmRes = co_await ev.owner->co_direct_message_create(
                    newM.user_id,
                    message::dm::welcome());
                if (dmRes.is_error()) {
                    log::error("Failed to send welcome DM to '{}': {}",
                        newM.get_user()->global_name,
                        dmRes.get_error().message);
                };

                auto const msgRes = co_await ev.owner->co_message_create(dpp::message(g_welcomeChannel, fmt::format("<a:a_hi:829327870940938251> **Welcome to Cubic Studios's community server, <@{}>**! Be sure to read our https://discord.com/channels/460081436637134859/460082070673424386/882029054033793025 before engaging. We hope you enjoy your time here!", newM.get_user()->id)));
                if (msgRes.is_error()) {
                    m_cache.erase(it);

                    log::error("Failed to send welcome message to '{}': {}", newM.get_user()->global_name, msgRes.get_error().message);
                    co_return;
                };
            };

            m_cache.erase(it);

            co_return;
        });
    };
};

static WelcomeEvent ev;