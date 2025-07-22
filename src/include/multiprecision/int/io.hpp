#ifndef MP_IO_HPP
#define MP_IO_HPP

/**
 * io.hpp
 *
 * Handling streams: printing int_t to output stream and reading int_t from
 * input stream. Also, there's int-to-string conversion.
 */

#include "mpint.hpp"
#include <iostream>

namespace multiprecision {

template <size_t bits> std::string int_t<bits>::toString() const {
    int_t<bits> number = *this;

    if (number == 0) {
        return "0";
    }

    const bool isNegative = number < 0;

    if (isNegative) {
        number = -number;
    }

    std::string out;

    while (number > 0) {
        out += '0' + int64_t(number % 10);

        number /= 10;
    }

    if (isNegative) {
        out += '-';
    }

    std::reverse(out.begin(), out.end());

    return out;
}

template <size_t bits>
std::ostream &operator<<(std::ostream &os, const int_t<bits> &integer) {
    return os << integer.toString();
}

template <size_t bits>
std::istream &operator>>(std::istream &is, int_t<bits> &integer) {
    std::string integerString;

    is >> integerString;

    integer = 0;

    for (char c : integerString) {
        if (c == '-' || c == '+')
            continue;
        if (c == '.' || c == ',')
            break;

        integer *= 10;
        integer += int64_t(c - '0');
    }

    return is;
}

} // namespace multiprecision

#endif