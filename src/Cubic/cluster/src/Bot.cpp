#include "../Bot.hpp"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

Bot::Bot() : m_bot(env::get("TOKEN").value_or("")) {};

Bot& cubic::bot::get() {
    return *Bot::get();
};