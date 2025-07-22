#include <random>
#define DEFAULT_INTEGRAL_TYPE uint16_t

#include <gtest/gtest.h>
#include <multiprecision.hpp>

TEST(Arithmetic, Multiplication) {
    {
        const int64_t first = 1 << 31;
        const int64_t second = 1 << 31;

        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first * second;
        const multiprecision::int_t<64> result = a * b;

        EXPECT_EQ(result, correctResult);
    }
}

TEST(Arithmetic, MultiplicationStress) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x7fffffff,
                                                           0x7fffffff);

    for (size_t i = 0; i < 1000000; ++i) {
        const int64_t first = randomGenerator(mt);
        const int64_t second = randomGenerator(mt) >> 4;

        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first * second;
        const multiprecision::int_t<64> result = a * b;

        EXPECT_EQ(result, correctResult);
    }
}