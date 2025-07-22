#ifndef MP_COMPARATION_HPP
#define MP_COMPARATION_HPP

/**
 * comparation.hpp
 *
 * All comparation operators.
 */

#include "mpint.hpp"
#include <compare>

namespace multiprecision {

template <size_t bits>
auto int_t<bits>::operator<=>(const int_t<bits> &other) const {
    if (isNegative() && !other.isNegative()) {
        return std::strong_ordering::less;
    }

    if (!isNegative() && other.isNegative()) {
        return std::strong_ordering::greater;
    }

    for (ssize_t i = kComponentsCount - 1; i >= 0; --i) {
        if (_components[i] == other._components[i])
            continue;

        if (_components[i] < other._components[i]) {
            return std::strong_ordering::less;
        }

        return std::strong_ordering::greater;
    }

    return std::strong_ordering::equal;
}

template <size_t bits>
bool int_t<bits>::operator==(const int_t<bits> &other) const {
    for (size_t i = 0; i < kComponentsCount; ++i) {
        if (_components[i] != other._components[i]) {
            return false;
        }
    }

    return true;
}

template <size_t bits>
bool int_t<bits>::operator!=(const int_t<bits> &other) const {
    return !((*this) == other);
}

template <size_t bits> auto int_t<bits>::operator<=>(int64_t x) const {
    return (*this) <=> int_t<bits>(x);
}

template <size_t bits> bool int_t<bits>::operator==(int64_t x) const {
    return (*this) == int_t<bits>(x);
}

template <size_t bits> bool int_t<bits>::operator!=(int64_t x) const {
    return !((*this) == x);
}

template <size_t bits> bool int_t<bits>::isNegative() const {
    static constexpr IntegralType kMask = IntegralType(1)
                                          << IntegralType(kComponentSize - 1);

    return _components[kComponentsCount - 1] & kMask;
}

} // namespace multiprecision

#endif