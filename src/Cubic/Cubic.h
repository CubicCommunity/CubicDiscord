#pragma once

#include <dpp/dpp.h>

#include <fmt/core.h>

#include <Cubic/std.hpp>
#include <Cubic/Version.h>

#include <Cubic/util/Macros.h>
#include <Cubic/util/Include.h>

#include <Cubic/base/Include.h>

#include <Cubic/cluster/Include.h>

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
#else
        inline constexpr dpp::snowflake id = 460081436637134859;
#endif
    };

    namespace message {
        namespace dm {
            inline auto welcome() {
                using namespace dpp;

                auto const embed = dpp::embed()
                                       .set_title("Welcome to Cubic Studios")
                                       .set_description("Your home for entertainment.\n\nHello! Welcome to the official server of Cubic Studios, home of many awesome creative works! Thanks for coming by!")
                                       .add_field(
                                           "Meet the Community",
                                           "Chat with others at <#827884790140108830>!")
                                       .set_thumbnail("https://i.imgur.com/5q75iQC.gif")
                                       .set_color(theme::colors::primary);

                auto const row = dpp::component()
                                     .set_type(cot_action_row)
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
                                                 1408088493967671406))
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

        inline std::vector<dpp::snowflake> extractChannels(const std::string& content) {
            std::vector<dpp::snowflake> ids;

            static const RE2 channel_regex("<#([0-9]+)>");

            re2::StringPiece input(content);
            std::string id_str;

            while (RE2::FindAndConsume(&input, channel_regex, &id_str)) {
                if (!id_str.empty() && std::all_of(id_str.begin(), id_str.end(), ::isdigit)) ids.emplace_back(static_cast<uint64_t>(std::stoull(id_str)));
            };

            return ids;
        };
    };

    namespace prelude {
        using namespace ::cubic;
        using namespace ::cubic::util;
        using namespace ::cubic::cluster;
    };
};