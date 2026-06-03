#pragma once

#include <dpp/dpp.h>

#include <fmt/core.h>

#include <Cubic/std.hpp>

#include <Cubic/base/Include.h>
#include <Cubic/cluster/Include.h>
#include <Cubic/util/Include.h>

namespace cubic {
    namespace fs = asp::fs;

    namespace env {
        inline std::optional<std::string> get(const char* name) {
#ifdef _WIN32
            char* value = nullptr;
            size_t len = 0;

            if (_dupenv_s(&value, &len, name) == 0 && value) {
                std::string result(value);
                free(value);
                return result;
            };

            return std::nullopt;
#else
            if (auto value = std::getenv(name)) return std::string(value);
            return std::nullopt;
#endif
        };
    };

    namespace server {
#ifdef CUBIC_LOCAL_BUILD
        inline constexpr dpp::snowflake id = 1330925786634522787;
        inline constexpr dpp::snowflake welcomeChannel = 1511704877481332806;
#else
        inline constexpr dpp::snowflake id = 460081436637134859;
        inline constexpr dpp::snowflake welcomeChannel = 1412493998790021171;
#endif
    };

    namespace message {
        namespace dm {
            inline auto welcome() {
                using namespace dpp;

                return dpp::message()
                    .add_component_v2(
                        component()
                            .set_type(component_type::cot_container)
                            .set_accent(theme::colors::primary)
                            .add_component_v2(
                                component()
                                    .set_type(component_type::cot_section)
                                    .add_component_v2(
                                        component()
                                            .set_type(component_type::cot_text_display)
                                            .set_content("# Cubic Studios"))
                                    .add_component_v2(
                                        component()
                                            .set_type(component_type::cot_text_display)
                                            .set_content("Your home for entertainment."))
                                    .set_accessory(
                                        component()
                                            .set_type(component_type::cot_thumbnail)
                                            .set_url("https://i.imgur.com/5q75iQC.gif")))
                            .add_component_v2(
                                component()
                                    .set_type(component_type::cot_separator)
                                    .set_divider(true))
                            .add_component_v2(
                                component()
                                    .set_type(component_type::cot_text_display)
                                    .set_content("Hello! Welcome to the official server of Cubic Studios. Thanks for coming by!\n\nStart hanging out with others at <#827884790140108830>!"))
                            .add_component_v2(
                                component()
                                    .set_type(component_type::cot_separator)
                                    .set_divider(true))
                            .add_component_v2(
                                component()
                                    .set_type(component_type::cot_action_row)
                                    .add_component(
                                        component()
                                            .set_type(component_type::cot_button)
                                            .set_label("Official Website")
                                            .set_style(component_style::cos_link)
                                            .set_url("https://www.cubicstudios.xyz/")
                                            .set_emoji(
                                                "code_block",
                                                827868673548222465))
                                    .add_component(
                                        component()
                                            .set_type(component_type::cot_button)
                                            .set_label("YouTube")
                                            .set_style(component_style::cos_link)
                                            .set_url("https://www.youtube.com/c/CubicCommunity")
                                            .set_emoji(
                                                "YouTube",
                                                1408088493967671406))
                                    .add_component(
                                        component()
                                            .set_type(component_type::cot_button)
                                            .set_label("X")
                                            .set_style(component_style::cos_link)
                                            .set_url("https://www.x.com/CubicCommunity/")
                                            .set_emoji(
                                                "X_",
                                                1511731692627431585))));
            };
        };
    };

    namespace prelude {
        using namespace ::cubic;
        using namespace ::cubic::cluster;
    };
};