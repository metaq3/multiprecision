#ifndef MP_DIVISION_HPP
#define MP_DIVISION_HPP

/**
 * division.hpp
 *
 * Division and modulo operations.
 */

#include "mpint.hpp"

namespace multiprecision {

template <size_t bits, size_t other_bits>
int_t<bits> operator/(const int_t<bits> &first,
                      const int_t<other_bits> &second) {
    // Algo below is unable to operate with negative numbers. So we check for
    // number sign and negate result if needed.
    if (second.isNegative() || first.isNegative()) {
        int_t<bits> newFirst = first.isNegative() ? -first : first;
        int_t<bits> newSecond = second.isNegative() ? -second : second;

        const bool negateResult = first.isNegative() != second.isNegative();

        const int_t<bits> result = newFirst / newSecond;

        return negateResult ? -result : result;
    }

    // Second should be less than first. If it's not, see [1].
    int_t<bits> left = second, right = first;

    // Specify binsearch limits more precisely to avoid overflow.

    left >>= 1;

    // [1]: second > first case is handled here, as well as overflow.
    while (left != 0) {
        left >>= 1;
        right >>= 1;
    }

    ++right;

    // Now do binsearch algo.
    while (left < right) {
        const int_t<bits> mid = (left + right) >> 1;

        const int_t<bits> result = mid * second;

        if (result == first) {
            return mid;
        } else if (result < first) {
            left = mid;
            ++left;
        } else {
            right = mid;
        }
    }

    --left;

    return left;
}

template <size_t bits>
template <size_t other_bits>
int_t<bits> &int_t<bits>::operator/=(const int_t<other_bits> &other) {
    return *this = *this / other;
}

template <size_t bits>
int_t<bits> operator/(const int_t<bits> &first, int64_t other) {
    return first / int_t<bits>(other);
}

template <size_t bits> int_t<bits> &int_t<bits>::operator/=(int64_t other) {
    return *this = *this / int_t<bits>(other);
}

template <size_t bits, size_t other_bits>
int_t<bits> operator%(const int_t<bits> &first,
                      const int_t<other_bits> &second) {
    return first - (first / second) * second;
}

template <size_t bits>
int_t<bits> operator%(const int_t<bits> &first, int64_t second) {
    return first % int_t<bits>(second);
}

template <size_t bits>
template <size_t other_bits>
int_t<bits> &int_t<bits>::operator%=(const int_t<other_bits> &other) {
    return *this = *this % other;
}

template <size_t bits> int_t<bits> &int_t<bits>::operator%=(int64_t other) {
    return *this %= int_t<bits>(other);
}

}; // namespace multiprecision

#endif