#pragma once

#include <dpp/dpp.h>

#include <Cubic/std.hpp>

#include <Cubic/util/Macros.h>

namespace cubic {
    namespace base {
        class EventHandler {
        public:
            EventHandler() { getAll().emplace_back(this); };
            virtual ~EventHandler() = default;

            virtual CUBIC_EV_FUNC_INIT_SIGN = 0;

            static std::vector<EventHandler*>& getAll() {
                static std::vector<EventHandler*> evs;
                return evs;
            };
        };
    };
};