#pragma once

#include <dpp/dpp.h>

#include <Cubic/std.hpp>

namespace cubic {
    namespace base {
        class EventHandler {
        public:
            EventHandler() { getAll().emplace_back(this); };
            virtual ~EventHandler() = default;

            virtual void init(dpp::cluster& bot) = 0;

            static std::vector<EventHandler*>& getAll() {
                static std::vector<EventHandler*> evs;
                return evs;
            };
        };
    };
};