#pragma once

#ifdef CUBIC_LOCAL_BUILD
#define CUBIC_AVATAR_FORMAT dpp::i_webp
#else  // the test bot's avatar isn't animated lol
#define CUBIC_AVATAR_FORMAT dpp::i_gif
#endif

#define CUBIC_INIT(T) \
    static T t

#define CUBIC_CMD_FUNC_NAME_SIGN std::string name() const noexcept
#define CUBIC_CMD_FUNC_BUILD_SIGN dpp::slashcommand build() const
#define CUBIC_CMD_FUNC_HANDLE_SIGN dpp::task<void> handle(dpp::slashcommand_t const& ev)

#define CUBIC_CMD_FUNC_NAME CUBIC_CMD_FUNC_NAME_SIGN override
#define CUBIC_CMD_FUNC_BUILD CUBIC_CMD_FUNC_BUILD_SIGN override
#define CUBIC_CMD_FUNC_HANDLE CUBIC_CMD_FUNC_HANDLE_SIGN override

#define CUBIC_EV_FUNC_INIT_SIGN void init(dpp::cluster& bot)

#define CUBIC_EV_FUNC_INIT CUBIC_EV_FUNC_INIT_SIGN override