#ifndef MP_ARITHMETIC_HPP
#define MP_ARITHMETIC_HPP

/**
 * arithmetic.hpp
 *
 * Increment, decrement, addition and subtraction operators.
 * Multiplication and division are in corresponding included headers.
 */

#include "division.hpp"
#include "mpint.hpp"
#include "multiplication.hpp"

namespace multiprecision {

template <size_t bits> int_t<bits> &int_t<bits>::operator++() {
    IntegralType toAdd = 1;

    for (size_t i = 0; i < kComponentsCount; ++i) {
        if (toAdd == 0)
            break;

        toAdd = __builtin_add_overflow(_components[i], toAdd, &_components[i]);
    }

    return *this;
}

template <size_t bits> int_t<bits> int_t<bits>::operator++(int) {
    int_t<bits> out = *this;

    ++(*this);

    return out;
}

template <size_t bits> int_t<bits> &int_t<bits>::operator--() {
    IntegralType toAdd = 1;

    for (size_t i = 0; i < kComponentsCount; ++i) {
        if (toAdd == 0)
            break;

        toAdd = __builtin_sub_overflow(_components[i], toAdd, &_components[i]);
    }

    return *this;
}

template <size_t bits> int_t<bits> int_t<bits>::operator--(int) {
    int_t<bits> out = *this;

    --(*this);

    return out;
}

template <size_t bits> void int_t<bits>::negate() {
    for (IntegralType &component : _components) {
        component = ~component;
    }

    ++(*this);
}

template <size_t bits>
template <size_t other_bits>
int_t<bits> &int_t<bits>::operator+=(const int_t<other_bits> &other) {
    IntegralType toAdd = 0;

    for (size_t i = 0; i < other.kComponentsCount && i < kComponentsCount;
         ++i) {
        toAdd = __builtin_add_overflow(_components[i], toAdd, &_components[i]);
        toAdd += __builtin_add_overflow(_components[i], other._components[i],
                                        &_components[i]);
    }

    return *this;
}

template <size_t bits>
template <size_t other_bits>
int_t<bits> &int_t<bits>::operator-=(const int_t<other_bits> &other) {
    return (*this) += -other;
}

template <size_t bits> int_t<bits> &int_t<bits>::operator+=(int64_t other) {
    return (*this) += int_t<bits>(other);
}

template <size_t bits> int_t<bits> &int_t<bits>::operator-=(int64_t other) {
    return (*this) += -other;
}

template <size_t bits, size_t other_bits>
int_t<bits> operator+(int_t<bits> first, const int_t<other_bits> &second) {
    return first += second;
}

template <size_t bits, size_t other_bits>
int_t<bits> operator-(int_t<bits> first, const int_t<other_bits> &second) {
    return first -= second;
}

template <size_t bits>
int_t<bits> operator+(int_t<bits> first, int64_t second) {
    return first += second;
}

template <size_t bits>
int_t<bits> operator-(int_t<bits> first, int64_t second) {
    return first -= second;
}

} // namespace multiprecision

#endif