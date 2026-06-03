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

                dpp::embed embed;
                embed.set_title("Cubic Studios")
                    .set_description("Your home for entertainment.\n\nHello! Welcome to the official server of Cubic Studios. Thanks for coming by!\n\nStart hanging out with others at <#827884790140108830>!")
                    .set_thumbnail("https://i.imgur.com/5q75iQC.gif")
                    .set_color(0x5865F2);  // Discord blurple

                dpp::component row;
                row.set_type(cot_action_row)
                    .add_component(
                        component()
                            .set_type(cot_button)
                            .set_label("Official Website")
                            .set_style(cos_link)
                            .set_url("https://www.cubicstudios.xyz/")
                            .set_emoji(
                                "code_block",
                                827868673548222465))
                    .add_component(
                        component()
                            .set_type(cot_button)
                            .set_label("YouTube")
                            .set_style(cos_link)
                            .set_url("https://www.youtube.com/c/CubicCommunity")
                            .set_emoji(
                                "YouTube",
                                1511731692627431585))
                    .add_component(
                        component()
                            .set_type(cot_button)
                            .set_label("X")
                            .set_style(cos_link)
                            .set_url("https://www.x.com/CubicCommunity/")
                            .set_emoji(
                                "X_",
                                1511731692627431585));

                return dpp::message()
                    .add_embed(embed)
                    .add_component(row);
            };
        };
    };

    namespace prelude {
        using namespace ::cubic;
        using namespace ::cubic::cluster;
    };
};