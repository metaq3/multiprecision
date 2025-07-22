#ifndef MPINT_HPP
#define MPINT_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <string>

#ifndef DEFAULT_INTEGRAL_TYPE
#define DEFAULT_INTEGRAL_TYPE uint64_t
#endif

namespace multiprecision {

/**
 * @brief Multiprecision integer.
 *
 * @tparam bits - must be multiple of 64.
 */
template <size_t bits> class int_t {
 public:
    using IntegralType = DEFAULT_INTEGRAL_TYPE;

    constexpr int_t();
    constexpr int_t(int64_t x);

    template <size_t other_bits> constexpr int_t(const int_t<other_bits> &x);

    template <size_t other_bits>
    constexpr int_t<bits> &operator=(const int_t<other_bits> &other);
    constexpr int_t<bits> &operator=(int64_t other);

    constexpr std::string toString() const;

    template <size_t other_bits>
    constexpr int_t<bits> &operator+=(const int_t<other_bits> &other);

    template <size_t other_bits>
    constexpr int_t<bits> &operator-=(const int_t<other_bits> &other);

    template <size_t other_bits>
    constexpr int_t<bits> &operator*=(const int_t<other_bits> &other);

    template <size_t other_bits>
    constexpr int_t<bits> &operator/=(const int_t<other_bits> &other);

    template <size_t other_bits>
    constexpr int_t<bits> &operator%=(const int_t<other_bits> &other);

    /**
     * @brief Provides left bit shifting. Take note that only first 64 bits are
     * taken as shift value.
     *
     * @tparam other_bits
     * @param shift
     * @return int_t<bits>&
     */
    template <size_t other_bits>
    constexpr int_t<bits> &operator<<=(const int_t<other_bits> &shift);

    /**
     * @brief Provides right bit shifting. Take note that only first 64 bits are
     * taken as shift value.
     *
     * @tparam other_bits
     * @param shift
     * @return int_t<bits>&
     */
    template <size_t other_bits>
    constexpr int_t<bits> &operator>>=(const int_t<other_bits> &shift);

    constexpr int_t<bits> &operator+=(int64_t other);
    constexpr int_t<bits> &operator-=(int64_t other);
    constexpr int_t<bits> &operator*=(int64_t other);
    constexpr int_t<bits> &operator/=(int64_t other);
    constexpr int_t<bits> &operator%=(int64_t other);
    constexpr int_t<bits> &operator<<=(uint64_t other);
    constexpr int_t<bits> &operator>>=(uint64_t other);

    constexpr int_t<bits> &operator++();
    constexpr int_t<bits> operator++(int);

    constexpr int_t<bits> &operator--();
    constexpr int_t<bits> operator--(int);

    constexpr int_t<bits> operator-() const;

    constexpr auto operator<=>(const int_t<bits> &other) const;
    constexpr bool operator==(const int_t<bits> &other) const;
    constexpr bool operator!=(const int_t<bits> &other) const;

    constexpr auto operator<=>(int64_t x) const;
    constexpr bool operator==(int64_t x) const;
    constexpr bool operator!=(int64_t x) const;

    template <size_t other_bits>
    constexpr int_t<bits> &operator|=(const int_t<other_bits> &other);

    template <size_t other_bits>
    constexpr int_t<bits> &operator&=(const int_t<other_bits> &other);

    template <size_t other_bits>
    constexpr int_t<bits> &operator^=(const int_t<other_bits> &other);

    constexpr int_t<bits> &operator|=(uint64_t other);
    constexpr int_t<bits> &operator&=(uint64_t other);
    constexpr int_t<bits> &operator^=(uint64_t other);

    constexpr int_t<bits> operator~() const;

    constexpr bool isNegative() const;

    // Making theese implicit can cause ambigiousity with comparision operators
    explicit operator int64_t() const;
    explicit operator int32_t() const;
    explicit operator int16_t() const;
    explicit operator int8_t() const;
    explicit operator uint64_t() const;
    explicit operator uint32_t() const;
    explicit operator uint16_t() const;
    explicit operator uint8_t() const;

    // I would like to place __int128 conversion here, but MSVC does not have
    // this type. MSVC...

 protected:
    constexpr void negate();

 private:
    static constexpr size_t kComponentSize = sizeof(IntegralType) * 8;
    static constexpr size_t kComponentsCount =
        (bits + kComponentSize - 1) / kComponentSize;

    alignas(8) IntegralType _components[kComponentsCount];

    static_assert((bits % 64) == 0, "Operating ints with bitness not multiple "
                                    "of 64 is unsupported.");

    template <size_t other_bits> friend class int_t;
};

template <size_t bits> constexpr int_t<bits>::int_t() {
    std::fill(_components, _components + kComponentsCount, 0);
}

template <size_t bits> constexpr int_t<bits>::int_t(int64_t x) : int_t() {
    if (x < 0) {
        x = -x;

        std::copy(reinterpret_cast<IntegralType *>(&x),
                  reinterpret_cast<IntegralType *>(&x + 1), _components);

        negate();

        return;
    }

    std::copy(reinterpret_cast<IntegralType *>(&x),
              reinterpret_cast<IntegralType *>(&x + 1), _components);
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits>::int_t(const int_t<other_bits> &x) {
    for (size_t i = 0; i < kComponentsCount && i < x.kComponentsCount; ++i) {
        _components[i] = x._components[i];
    }
}

template <size_t bits>
template <size_t other_bits>
constexpr int_t<bits> &int_t<bits>::operator=(const int_t<other_bits> &other) {
    for (size_t i = 0; i < kComponentsCount && i < other.kComponentsCount;
         ++i) {
        _components[i] = other._components[i];
    }

    return *this;
}

template <size_t bits>
constexpr int_t<bits> &int_t<bits>::operator=(int64_t other) {
    std::fill(_components, _components + kComponentsCount, 0);

    if (other < 0) {
        other = -other;

        std::copy(reinterpret_cast<IntegralType *>(&other),
                  reinterpret_cast<IntegralType *>(&other + 1), _components);

        negate();

        return *this;
    }

    std::copy(reinterpret_cast<IntegralType *>(&other),
              reinterpret_cast<IntegralType *>(&other + 1), _components);

    return *this;
}

template <size_t bits> int_t<bits>::operator int64_t() const {
    const int64_t *const result =
        reinterpret_cast<int64_t const *>(_components);

    return *result;
}

template <size_t bits> int_t<bits>::operator int32_t() const {
    const int32_t *const result =
        reinterpret_cast<int32_t const *>(_components);

    return *result;
}

template <size_t bits> int_t<bits>::operator int16_t() const {
    const int16_t *const result =
        reinterpret_cast<int16_t const *>(_components);

    return *result;
}

template <size_t bits> int_t<bits>::operator int8_t() const {
    const int8_t *const result = reinterpret_cast<int8_t const *>(_components);

    return *result;
}

template <size_t bits> int_t<bits>::operator uint64_t() const {
    const uint64_t *const result =
        reinterpret_cast<uint64_t const *>(_components);

    return *result;
}

template <size_t bits> int_t<bits>::operator uint32_t() const {
    const uint32_t *const result =
        reinterpret_cast<uint32_t const *>(_components);

    return *result;
}

template <size_t bits> int_t<bits>::operator uint16_t() const {
    const uint16_t *const result =
        reinterpret_cast<uint16_t const *>(_components);

    return *result;
}

template <size_t bits> int_t<bits>::operator uint8_t() const {
    const uint8_t *const result =
        reinterpret_cast<uint8_t const *>(_components);

    return *result;
}

template <size_t bits> constexpr int_t<bits> int_t<bits>::operator-() const {
    int_t<bits> result = *this;

    result.negate();

    return result;
}

}; // namespace multiprecision

#endif