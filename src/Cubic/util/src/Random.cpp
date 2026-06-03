#include "../Random.hpp"

#include <Cubic/Cubic.h>

using namespace cubic::prelude;

static char genHexChar() {
    return "0123456789abcdef"[random::internal::generate<size_t>(0, 16)];
};

random::internal::Generator& random::internal::_getGenerator() {
    static thread_local Generator generator;
    return generator;
};

std::string random::internal::generateString(size_t length, std::string_view alphabet) {
    std::string out;
    out.reserve(length);

    auto& gen = _getGenerator();
    for (size_t i = 0; i < length; i++) {
        out += alphabet[gen.generate<size_t>(0, alphabet.size())];
    };

    return out;
};

std::string random::internal::generateHexString(size_t length) {
    return generateString(length, "0123456789abcdef");
};

std::string random::internal::generateAlphanumericString(size_t length) {
    return generateString(length, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
};

std::string random::internal::generateUUID() {
    std::string out;
    out.reserve(36);

    auto pushChar = [&] { out += genHexChar(); };
    auto pushChars = [&](size_t n) {
        for (size_t i = 0; i < n; i++) {
            pushChar();
        };
    };

    pushChars(8);
    out += '-';
    pushChars(4);
    out += '-';
    out += '4';
    pushChars(3);
    out += '-';
    out += "89ab"[generate<size_t>(0, 4)];
    pushChars(3);
    out += '-';
    pushChars(12);

    return out;
};

void random::internal::fillBytes(void* buffer, size_t size) {
    auto& gen = _getGenerator();

    uint8_t* buf = (uint8_t*)buffer;

    // fill 8 bytes at a time for speed
    while (size >= 8) {
        uint64_t val = gen.next();
        std::memcpy(buf, &val, 8);
        buf += 8;
        size -= 8;
    };

    if (size > 0) {
        uint64_t val = gen.next();
        std::memcpy(buf, &val, size);
    };
};