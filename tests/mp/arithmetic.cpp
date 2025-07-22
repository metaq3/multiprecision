#include <random>
#define DEFAULT_INTEGRAL_TYPE uint16_t

#include <gtest/gtest.h>
#include <multiprecision.hpp>

TEST(Arithmetic, Increment) {
    multiprecision::int_t<64> int64(0);
    int64_t realInt64 = 0;

    for (size_t i = 0; i < (1 << 20); ++i) {
        ++int64;
        ++realInt64;

        EXPECT_EQ(int64, realInt64);
    }
}

TEST(Arithmetic, Decrement) {
    multiprecision::int_t<64> int64(10);
    int64_t realInt64 = 10;

    for (size_t i = 0; i < (1 << 20); ++i) {
        --int64;
        --realInt64;

        EXPECT_EQ(int64, realInt64);
    }
}

TEST(Arithmetic, Addition) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x0fffffffffffffff,
                                                           0x0fffffffffffffff);

    for (size_t i = 0; i < 1000000; ++i) {
        const int64_t first = randomGenerator(mt);
        const int64_t second = randomGenerator(mt);

        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first + second;
        const multiprecision::int_t<64> result = a + b;

        EXPECT_EQ(result, correctResult);
    }
}

TEST(Arithmetic, Subtraction) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x0fffffffffffffff,
                                                           0x0fffffffffffffff);

    for (size_t i = 0; i < 1000000; ++i) {
        const int64_t first = randomGenerator(mt);
        const int64_t second = randomGenerator(mt);

        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        const int64_t correctResult = first - second;
        const multiprecision::int_t<64> result = a - b;

        EXPECT_EQ(result, correctResult);
    }
}