#include <Cubic/Cubic.h>

using namespace cubic::prelude;

class WelcomeEvent final : public base::EventHandler {
public:
    void init(dpp::cluster& bot) override {
        bot.on_guild_member_update([&bot](dpp::guild_member_update_t const& ev) -> dpp::task<void> {
            if (ev.updating_guild.id != server::id) co_return;

            auto newM = ev.updated;

            auto oldMRes = co_await bot.co_guild_get_member(ev.updating_guild.id, newM.user_id);
            if (oldMRes.is_error()) {
                log::error("Failed to fetch old member data for '{}' in guild '{}': {}",
                    newM.get_user()->global_name,
                    ev.updating_guild.id,
                    oldMRes.get_error().message);
                co_return;
            };

            auto oldM = oldMRes.get<dpp::guild_member>();

            if (newM.has_completed_onboarding() && !oldM.has_completed_onboarding()) {
                log::info("User {} has completed onboarding!", newM.get_user()->global_name);
            };

            co_return;
        });
    };
};

static WelcomeEvent ev;