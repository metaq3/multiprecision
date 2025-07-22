#ifndef MP_INT_IO_HPP
#define MP_INT_IO_HPP

/**
 * io.hpp
 *
 * Handling streams: printing int_t to output stream and reading int_t from
 * input stream. Also, there's int-to-string conversion.
 */

#include "mpint.hpp"
#include <iostream>

namespace multiprecision {

template <size_t bits> constexpr std::string int_t<bits>::toString() const {
    int_t<bits> number = *this;

    if (number == 0) {
        return "0";
    }

    const bool isNegative = number < 0;

    if (isNegative) {
        number = -number;
    }

    std::string out;

    constexpr uint64_t kChunkSize = 1e16;

    // A little speed up: parse number by 64-bit chunks with
    // 16 decimal digits each. So, most of the time CPU will
    // operate with 64-bit registers.
    while (number >= kChunkSize) {
        uint64_t remainder = uint64_t(number % kChunkSize);
        remainder += kChunkSize;

        while (remainder != 1) {
            out += '0' + int8_t(remainder % 10);

            remainder /= 10;
        }

        number /= kChunkSize;
    }

    {
        uint64_t remainder = uint64_t(number);

        while (remainder > 0) {
            out += '0' + int64_t(remainder % 10);

            remainder /= 10;
        }
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