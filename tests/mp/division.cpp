#include <random>
#define DEFAULT_INTEGRAL_TYPE uint16_t

#include <gtest/gtest.h>
#include <multiprecision.hpp>

TEST(Arithmetic, Division) {
    const std::pair<int64_t, int64_t> pairs[] = {
        {1ll << 62ll, 1ll << 62ll}, {9, -3}, {0x7fffffffffffffff, 10}};

    for (auto [first, second] : pairs) {
        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first / second;
        const multiprecision::int_t<64> result = a / b;

        EXPECT_EQ(result, correctResult);
    }
}

TEST(Arithmetic, DivisionStress) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x7ffffffffffffff,
                                                           0x7ffffffffffffff);

    for (size_t i = 0; i < 100000; ++i) {
        const int64_t first = randomGenerator(mt);
        const int64_t second = randomGenerator(mt) >> 4;

        if (second == 0)
            continue;

        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first / second;
        const multiprecision::int_t<64> result = a / b;

        EXPECT_EQ(result, correctResult);
    }
}

TEST(Arithmetic, Modulo) {
    const std::pair<int64_t, int64_t> pairs[] = {
        {1ll << 62ll, 1ll << 62ll},
        {9, 3},
        {16, 15},
        {25, 19},
        {(1ll << 62ll) + 1ll, 1ll << 62ll}};

    for (auto [first, second] : pairs) {
        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first / second;
        const multiprecision::int_t<64> result = a / b;

        EXPECT_EQ(result, correctResult);
    }
}

TEST(Arithmetic, ModuloStress) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x7ffffffffffffff,
                                                           0x7ffffffffffffff);

    for (size_t i = 0; i < 100000; ++i) {
        const int64_t first = randomGenerator(mt);
        const int64_t second = randomGenerator(mt) >> 2;

        if (second == 0)
            continue;

        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first % second;
        const multiprecision::int_t<64> result = a % b;

        EXPECT_EQ(result, correctResult);
    }
}