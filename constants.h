#ifndef _REGEX_CHARACTER_CONSTANTS_INCLUDED_
#define _REGEX_CHARACTER_CONSTANTS_INCLUDED_

#include <array>
#include <utility>

constexpr std::array alphanum = {
    std::make_pair('0', '9'),
    std::make_pair('A', 'Z'),
    std::make_pair('a', 'z')
};

constexpr std::array antiAlphanum = {
    std::make_pair(0, '0' - 1),
    std::make_pair('9' + 1, 'A' - 1),
    std::make_pair('Z' + 1, 'a' - 1),
    std::make_pair('z' + 1, 255)
};

constexpr std::array whiteSpace = {
    std::make_pair(' ', ' '),
    std::make_pair('\n', '\n'),
    std::make_pair('\t', '\t')
};

constexpr std::array antiWhiteSpace = {
    std::make_pair(0, 8),
    std::make_pair(11, 31),
    std::make_pair(33, 255)
};

constexpr std::array digits = {
    std::make_pair('0', '9')
};

constexpr std::array antiDigits = {
    std::make_pair(0, '0' - 1),
    std::make_pair('9' + 1, 255)
};

#endif
