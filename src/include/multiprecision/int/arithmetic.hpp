#ifndef MP_INT_ARITHMETIC_HPP
#define MP_INT_ARITHMETIC_HPP

/**
 * arithmetic.hpp
 *
 * Increment, decrement, addition and subtraction operators.
 * Multiplication and division are in corresponding included headers.
 */

#include "division.hpp"
#include "mpint.hpp"
#include "multiplication.hpp"
#include <iostream>

namespace multiprecision {

template <size_t bits> constexpr int_t<bits> &int_t<bits>::operator++() {
    IntegralType toAdd = 1;

    for (size_t i = 0; i < kComponentsCount; ++i) {
        if (toAdd == 0)
            break;

        toAdd = __builtin_add_overflow(_components[i], toAdd, &_components[i]);
    }

    return *this;
}

template <size_t bits> constexpr int_t<bits> int_t<bits>::operator++(int) {
    int_t<bits> out = *this;

    ++(*this);

    return out;
}

template <size_t bits> constexpr int_t<bits> &int_t<bits>::operator--() {
    IntegralType toAdd = 1;

    for (size_t i = 0; i < kComponentsCount; ++i) {
        if (toAdd == 0)
            break;

        toAdd = __builtin_sub_overflow(_components[i], toAdd, &_components[i]);
    }

    return *this;
}

template <size_t bits> constexpr int_t<bits> int_t<bits>::operator--(int) {
    int_t<bits> out = *this;

    --(*this);

    return out;
}

template <size_t bits> constexpr void int_t<bits>::negate() {
    for (IntegralType &component : _components) {
        component = ~component;
    }

    ++(*this);
}

template <size_t first_size, size_t second_size>
constexpr void sum(uint64_t *first, uint64_t *second) {
    uint64_t toAdd = 0;

    size_t i;

    constexpr size_t common_size = std::min(first_size, second_size);

    for (i = 0; i < common_size; ++i) {
        uint64_t newChunk = first[i] + toAdd;

        toAdd = ~newChunk | first[i];

        first[i] = newChunk + second[i];

        toAdd |= ~first[i] | newChunk;
        toAdd >>= 63;
    }

    for (; i < first_size; ++i) {
        if (toAdd == 0)
            break;

        uint64_t newChunk = first[i] + toAdd;

        toAdd = (~newChunk | first[i]) >> 63;

        first[i] = newChunk;
    }
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &int_t<bits>::operator+=(const int_t<other_bits> &other) {
    constexpr size_t componentsCount = std::min(
        int_t<bits>::kComponentsCount, int_t<other_bits>::kComponentsCount);

    size_t i;

    constexpr IntegralType lastComponentBit = sizeof(IntegralType) * 8 - 1;

    IntegralType toAdd = 0;

    size_t lastNonZero = componentsCount;

    for (; lastNonZero > 0; --lastNonZero) {
        if (other._components[lastNonZero - 1] != 0)
            break;
    }

    for (i = 0; i < lastNonZero; ++i) {
        if (((_components[i] | other._components[i]) &
             (IntegralType(1) << lastComponentBit)) == 0) {
            _components[i] += other._components[i] + toAdd;
            toAdd = 0;
            continue;
        }

        IntegralType newChunk = _components[i] + toAdd;

        toAdd = ~newChunk & _components[i];

        _components[i] = newChunk + other._components[i];

        // This is a genious trick to catch overflow. For some reason,
        // __builtin_add_overflow is slower even on x86 arch because of
        // jump instructions. This implementation is branchless. Maybe
        // this is the reason of improved perfomance.
        toAdd |= ~_components[i] & (newChunk | other._components[i]);
        toAdd |= newChunk & other._components[i];

        toAdd >>= lastComponentBit;
    }

    for (; i < kComponentsCount; ++i) {
        if (toAdd == 0)
            break;

        IntegralType newChunk = _components[i] + toAdd;

        toAdd = (~newChunk & _components[i]) >> lastComponentBit;

        _components[i] = newChunk;
    }

    return *this;
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &int_t<bits>::operator-=(const int_t<other_bits> &other) {
    return (*this) += -other;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator+=(int64_t other) {
    return (*this) += int_t<bits>(other);
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator-=(int64_t other) {
    return (*this) += -other;
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator+(int_t<bits> first,
                                const int_t<other_bits> &second) {
    return first += second;
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator-(int_t<bits> first,
                                const int_t<other_bits> &second) {
    return first -= second;
}

template <size_t bits>
constexpr int_t<bits> operator+(int_t<bits> first, int64_t second) {
    return first += second;
}

template <size_t bits>
constexpr int_t<bits> operator-(int_t<bits> first, int64_t second) {
    return first -= second;
}

} // namespace multiprecision

#endif