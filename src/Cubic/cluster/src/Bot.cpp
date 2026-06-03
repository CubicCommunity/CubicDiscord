#include "../Bot.hpp"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

dpp::cluster Bot::m_bot = dpp::cluster(env::get("TOKEN").value_or(""), dpp::i_default_intents | dpp::i_guild_members | dpp::i_message_content);