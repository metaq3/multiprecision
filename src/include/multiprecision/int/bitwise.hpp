#ifndef MP_INT_BITWISE_HPP
#define MP_INT_BITWISE_HPP

/**
 * bitwise.hpp
 *
 * Bitwise operators: AND, OR, XOR, NOT and bit shifts.
 */

#include "mpint.hpp"

namespace multiprecision {

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &int_t<bits>::operator|=(const int_t<other_bits> &other) {
    for (size_t i = 0; i < kComponentsCount && i < other.kComponentsCount;
         ++i) {
        _components[i] |= other._components[i];
    }

    return *this;
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &int_t<bits>::operator&=(const int_t<other_bits> &other) {
    for (size_t i = 0; i < kComponentsCount && i < other.kComponentsCount;
         ++i) {
        _components[i] &= other._components[i];
    }

    return *this;
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &int_t<bits>::operator^=(const int_t<other_bits> &other) {
    for (size_t i = 0; i < kComponentsCount && i < other.kComponentsCount;
         ++i) {
        _components[i] ^= other._components[i];
    }

    return *this;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator|=(uint64_t other) {
    *(uint64_t *)_components |= other;

    return *this;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator&=(uint64_t other) {
    *(uint64_t *)_components &= other;

    std::fill(_components + sizeof(uint64_t) / sizeof(IntegralType),
              _components + kComponentsCount, 0);

    return *this;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator^=(uint64_t other) {
    *(uint64_t *)_components ^= other;

    return *this;
}

template <size_t bits> constexpr int_t<bits> int_t<bits>::operator~() const {
    int_t<bits> result = *this;

    for (size_t i = 0; i < kComponentsCount; ++i) {
        result._components[i] = ~result._components[i];
    }

    return result;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator<<=(uint64_t shift) {
    if (shift >= bits) {
        return *this = 0;
    }

    if (shift == 0) {
        return *this;
    }

    const IntegralType componentShift = shift % kComponentSize;
    const IntegralType inverseBitshift = kComponentSize - componentShift;

    for (size_t i = kComponentsCount - 1; i > shift / kComponentSize; --i) {
        _components[i] = _components[i - shift / kComponentSize];
        _components[i] <<= componentShift;
        _components[i] |=
            _components[i - shift / kComponentSize - 1] >> inverseBitshift;
    }

    _components[shift / kComponentSize] = _components[0];
    _components[shift / kComponentSize] <<= componentShift;

    for (size_t i = 0; i < shift / kComponentSize; ++i) {
        _components[i] = 0;
    }

    return *this;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator>>=(uint64_t shift) {
    if (shift >= bits) {
        return *this = 0;
    }

    if (shift == 0) {
        return *this;
    }

    const IntegralType inverseBitshift = (bits - shift) % kComponentSize;
    const IntegralType componentShift = shift % kComponentSize;
    const IntegralType inverseBitshiftMask =
        (IntegralType(1) << inverseBitshift) - IntegralType(1);

    const size_t lastComponent = kComponentsCount - shift / kComponentSize - 1;

    for (size_t i = 0; i < lastComponent; ++i) {
        _components[i] = _components[i + shift / kComponentSize];
        _components[i] >>= componentShift;
        _components[i] |= _components[i + shift / kComponentSize + 1]
                          << inverseBitshift;
    }

    _components[lastComponent] = _components[kComponentsCount - 1];
    _components[lastComponent] >>= componentShift;

    for (size_t i = lastComponent + 1; i < kComponentsCount; ++i) {
        _components[i] = 0;
    }

    return *this;
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &
int_t<bits>::operator<<=(const int_t<other_bits> &other) {
    // This is not the best way to reduce to shift with uint64_t, but shifting
    // over 18446744073709551616 is senseless either.
    const uint64_t shift = *reinterpret_cast<uint64_t *>(other._components);

    return *this <<= shift;
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &
int_t<bits>::operator>>=(const int_t<other_bits> &other) {
    // This is not the best way to reduce to shift with uint64_t, but shifting
    // over 18446744073709551616 is senseless either.
    const uint64_t shift = *reinterpret_cast<uint64_t *>(other._components);

    return *this >>= shift;
}

template <size_t bits>
constexpr int_t<bits> operator<<(int_t<bits> first, uint64_t second) {
    return first <<= second;
}

template <size_t bits>
constexpr int_t<bits> operator>>(int_t<bits> first, uint64_t second) {
    return first >>= second;
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator<<(int_t<bits> first,
                                 const int_t<other_bits> &second) {
    return first <<= second;
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator>>(int_t<bits> first,
                                 const int_t<other_bits> &second) {
    return first >>= second;
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator|(int_t<bits> first,
                                const int_t<other_bits> &second) {
    return first |= second;
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator&(int_t<bits> first,
                                const int_t<other_bits> &second) {
    return first &= second;
}

template <size_t bits, size_t other_bits>
constexpr int_t<bits> operator^(int_t<bits> first,
                                const int_t<other_bits> &second) {
    return first ^= second;
}

template <size_t bits>
constexpr int_t<bits> operator|(int_t<bits> first, uint64_t second) {
    return first |= second;
}

template <size_t bits>
constexpr int_t<bits> operator&(int_t<bits> first, uint64_t second) {
    return first &= second;
}

template <size_t bits>
constexpr int_t<bits> operator^(int_t<bits> first, uint64_t second) {
    return first ^= second;
}

} // namespace multiprecision

#endif