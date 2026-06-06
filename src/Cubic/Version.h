#pragma once

#include <Cubic/base/Singleton.hpp>

namespace cubic {
    enum class GitPlatform : uint8_t {
        GitHub = 0,
        GitGay = 1,
        Codeberg = 2,
        GitLab = 3,
    };

    class Version final : public base::Singleton<Version> {
    private:
        static asp::SystemTime const s_uptime;

        uint8_t const m_major = 1;
        uint8_t const m_minor = 0;
        uint8_t const m_patch = 0;

        std::string const m_buildTag = "alpha";
        uint8_t const m_buildNumber = 1;

        GitPlatform const m_platform = GitPlatform::GitHub;
        std::string const m_repository = "CubicCommunity/CubicDiscord";

    protected:
        std::string getPlatformDomain() const noexcept;

    public:
        static asp::Duration getUptime() noexcept;
        static uint64_t getStartupUnix() noexcept;

        uint8_t getMajor() const noexcept;
        uint8_t getMinor() const noexcept;
        uint8_t getPatch() const noexcept;

        std::string_view getBuildTag() const noexcept;
        uint8_t getBuildNumber() const noexcept;

        std::string getVersionString(bool v = true, bool build = true) const;
    };
};