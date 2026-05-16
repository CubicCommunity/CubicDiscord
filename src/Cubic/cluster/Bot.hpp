#pragma once

#include <dpp/dpp.h>

#include <Cubic/base/Singleton.hpp>

namespace cubic {
    namespace cluster {
        class Bot final {
        private:
            static dpp::cluster m_bot;

        public:
            inline static dpp::cluster& get() noexcept {
                return m_bot;
            };
        };
    };
};