#include "../String.hpp"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

template <typename T>
std::string doJoin(std::span<T const> strs, std::string_view separator) {
    if (strs.empty()) return "";
    if (strs.size() == 1) return std::string(strs[0]);

    std::string res;

    size_t size = 0;
    for (auto const& str : strs) size += str.size() + separator.size();
    res.reserve(size);

    res.append(strs[0]);
    for (size_t i = 1; i < strs.size(); i++) {
        res.append(separator);
        res.append(strs[i]);
    };

    return res;
};

std::string util::string::join(std::span<std::string const> strs, std::string_view separator) {
    return doJoin<std::string>(strs, separator);
};

std::string util::string::join(std::span<std::string_view const> strs, std::string_view separator) {
    return doJoin<std::string_view>(strs, separator);
};