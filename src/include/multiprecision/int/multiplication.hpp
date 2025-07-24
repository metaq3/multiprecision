#ifndef MP_MULTIPLICATION_HPP
#define MP_MULTIPLICATION_HPP

/**
 * multiplication.hpp
 *
 * Multiplication operations.
 */

#include "mpint.hpp"
#include <cstring>

namespace multiprecision {

template <size_t segments_count>
constexpr void multiplySegments(uint32_t first[segments_count],
                                uint32_t const second[segments_count],
                                uint32_t result[segments_count]) {
    // Don't make this static if you don't want to break multithread
    // applications.
    uint64_t noOverflowResult[segments_count + 1];

    std::fill(noOverflowResult, noOverflowResult + segments_count, 0);

    // Multiplying two n-bit integers, represented as 32-bit integer arrays.
    // To make it simpler and don't care about complex multiplication overflow
    // carrying among segments, we're multiplying in 64-bit integers, storing
    // overflow in special 64-bit integer array.
    for (size_t i = 0; i < segments_count; ++i) {
        for (size_t j = 0; i + j < segments_count; ++j) {
            const uint64_t segmentMultiplication =
                uint64_t(first[i]) * uint64_t(second[j]);

            noOverflowResult[i + j + 1] += segmentMultiplication >> 32;

            // Results are stored here, in last builtin arg.
            noOverflowResult[i + j + 1] += __builtin_add_overflow(
                segmentMultiplication & 0xffffffff, noOverflowResult[i + j],
                &noOverflowResult[i + j]);
        }
    }

    // We can't really cast from uint64_t to uint32_t. This is UB and provides
    // obscure bugs in GCC.
    uint32_t remainder[segments_count * 2 + 2];

    memcpy(remainder, noOverflowResult,
           (segments_count + 1) * sizeof(uint64_t));

    // Now accurately add overflow to the corresponding segments. Basically,
    // this is a simple addition operation of two large integers.

    result[0] = remainder[0];

    for (size_t i = 1; i < segments_count; ++i) {
        result[i] = remainder[i * 2 - 1];
        noOverflowResult[i + 1] +=
            __builtin_add_overflow(remainder[i * 2], result[i], &result[i]);

        memcpy(&remainder[i * 2 + 2], &noOverflowResult[i + 1],
               sizeof(uint64_t));
    }
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &int_t<bits>::operator*=(const int_t<other_bits> &other) {
    constexpr size_t k32BitComponentsCount = std::min(bits, other_bits) / 32;

    uint32_t components[k32BitComponentsCount];
    uint32_t otherComponents[k32BitComponentsCount];

    // Safe way to "cast" array types.
    memcpy(components, _components, k32BitComponentsCount * sizeof(uint32_t));
    memcpy(otherComponents, other._components,
           k32BitComponentsCount * sizeof(uint32_t));

    multiplySegments<k32BitComponentsCount>(components, otherComponents,
                                            components);

    memcpy(_components, components, k32BitComponentsCount * sizeof(uint32_t));

    return *this;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator*=(int64_t other) {
    return *this *= int_t<bits>(other);
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator*(int_t<bits> first,
                                const int_t<other_bits> &second) {
    return first *= second;
}

template <size_t bits>
constexpr int_t<bits> operator*(int_t<bits> first, int64_t other) {
    return first *= other;
}

}; // namespace multiprecision

#endif