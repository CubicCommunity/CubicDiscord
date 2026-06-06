#include "Version.h"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

asp::SystemTime const Version::s_uptime = asp::SystemTime::now();

std::string Version::getPlatformDomain() const noexcept {
    switch (m_platform) {
        default: [[fallthrough]];

        case GitPlatform::GitHub: return "github.com";
        case GitPlatform::GitGay: return "git.gay";
        case GitPlatform::Codeberg: return "codeberg.org";
        case GitPlatform::GitLab: return "gitlab.com";
    };
};

uint8_t Version::getMajor() const noexcept {
    return m_major;
};

uint8_t Version::getMinor() const noexcept {
    return m_minor;
};

uint8_t Version::getPatch() const noexcept {
    return m_patch;
};

std::string_view Version::getBuildTag() const noexcept {
    return m_buildTag;
};

uint8_t Version::getBuildNumber() const noexcept {
    return m_buildNumber;
};

std::string Version::getVersionString(bool v, bool build) const {
    auto base = fmt::format("{}.{}.{}", getMajor(), getMinor(), getPatch());

    if (build && !getBuildTag().empty()) {
        base = fmt::format("{}-{}", base, getBuildTag().empty() ? "" : fmt::format("{}", getBuildNumber() > 0 ? fmt::format("{}.{}", getBuildTag(), getBuildNumber()) : getBuildTag()));
        if (!v) return base;
    };

    if (!v && !build) return base;

    return fmt::format("v{}", base);
};

asp::Duration Version::getUptime() noexcept {
    return s_uptime.elapsed();
};

uint64_t Version::getStartupUnix() noexcept {
    return s_uptime.timeSinceEpoch().seconds();
};