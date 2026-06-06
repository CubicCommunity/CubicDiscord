#pragma once

#include <dpp/dpp.h>

#include <Cubic/base/Singleton.hpp>

namespace cubic {
    namespace cluster {
        class Bot final {
        private:
            static dpp::cluster s_bot;
            static dpp::webhook s_devWebhook;

        public:
            inline static dpp::cluster& get() noexcept { return s_bot; };
            inline static dpp::webhook const& getDevWebhook() noexcept { return s_devWebhook; };
        };
    };
};