#include "../Log.hpp"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

void log::internal::write(dpp::loglevel level, std::string const& str) {
    Bot::get().log(level, str);
};