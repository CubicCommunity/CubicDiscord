#pragma once

#include <dpp/dpp.h>

#include <Cubic/base/Singleton.hpp>

namespace cubic {
    namespace cluster {
        class Bot final : public base::Singleton<Bot> {
        private:
            dpp::cluster m_bot;

        protected:
            Bot();

        public:
            dpp::cluster* operator->() {
                return &m_bot;
            };

            dpp::cluster& operator*() {
                return m_bot;
            };
        };
    };

    namespace bot {
        cluster::Bot& get();
    };
};