#include "../Bot.hpp"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

dpp::cluster Bot::s_bot = dpp::cluster(env::get("TOKEN").value_or(""), dpp::i_default_intents | dpp::i_guild_members | dpp::i_message_content);
dpp::webhook Bot::s_devWebhook = dpp::webhook(env::get("DEV_WEBHOOK").value_or(""));