#include <Cubic/Cubic.h>

using namespace cubic::prelude;

#ifdef CUBIC_LOCAL_BUILD
#define CUBIC_WELCOME_SNOWFLAKE 1512802838026260602
#else
#define CUBIC_WELCOME_SNOWFLAKE 1412493998790021171
#endif

static constexpr dpp::snowflake g_welcomeChannel = CUBIC_WELCOME_SNOWFLAKE;

class WelcomeEvent final : public base::EventHandler {
public:
    void init(dpp::cluster& bot) override {
        bot.on_guild_member_update([&bot](dpp::guild_member_update_t const& ev) -> dpp::task<void> {
            if (ev.updating_guild.id != server::id) co_return;

            auto newM = ev.updated;

            auto const oldMRes = co_await bot.co_guild_get_member(ev.updating_guild.id, newM.user_id);
            if (oldMRes.is_error()) {
                log::error("Failed to fetch old member data for '{}' in guild '{}': {}",
                    newM.get_user()->global_name,
                    ev.updating_guild.id,
                    oldMRes.get_error().message);

                co_return;
            };

            auto const oldM = oldMRes.get<dpp::guild_member>();

            if (newM.has_completed_onboarding() && !oldM.has_completed_onboarding()) {
                log::info("User {} has completed onboarding!", newM.get_user()->global_name);

                auto const dmRes = co_await bot.co_direct_message_create(
                    newM.user_id,
                    message::dm::welcome());
                if (dmRes.is_error()) {
                    log::error("Failed to send welcome DM to '{}': {}",
                        newM.get_user()->global_name,
                        dmRes.get_error().message);

                    co_return;
                };

                auto const msgRes = co_await bot.co_message_create(dpp::message(g_welcomeChannel, fmt::format("Welcome to Cubic Studios's community server, {}! Please check your DMs for important information about the server and its channels.", newM.get_user()->global_name)));
                if (msgRes.is_error()) {
                    log::error("Failed to send welcome message to '{}': {}", newM.get_user()->global_name, msgRes.get_error().message);
                    co_return;
                };

                co_return;
            };

            co_return;
        });
    };
};

static WelcomeEvent ev;