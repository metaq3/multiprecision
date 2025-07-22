#include <random>
#define DEFAULT_INTEGRAL_TYPE uint16_t

#include <gtest/gtest.h>
#include <multiprecision.hpp>

TEST(Bitwise, LeftShift) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    mt.seed(38275);

    std::uniform_int_distribution<int64_t> numberGenerator(0, 0x7fffffff);
    std::uniform_int_distribution<int64_t> shiftGenerator(0, 32);

    for (size_t i = 0; i < 100; ++i) {
        const int64_t number = numberGenerator(mt);
        const int64_t shift = shiftGenerator(mt);

        const multiprecision::int_t<64> numberInt(number);

        const int64_t correctResult = number << shift;
        const multiprecision::int_t<64> result = numberInt << shift;

        EXPECT_EQ(result, multiprecision::int_t<64>(correctResult));
    }
}

TEST(Bitwise, RightShift) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    mt.seed(38275);

    std::uniform_int_distribution<int64_t> numberGenerator(0, 0x7fffffff);
    std::uniform_int_distribution<int64_t> shiftGenerator(0, 32);

    for (size_t i = 0; i < 100; ++i) {
        const int64_t number = numberGenerator(mt);
        const int64_t shift = shiftGenerator(mt);

        const multiprecision::int_t<64> numberInt(number);

        const int64_t correctResult = number >> shift;
        const multiprecision::int_t<64> result = numberInt >> shift;

        EXPECT_EQ(result, multiprecision::int_t<64>(correctResult));
    }
}

TEST(Bitwise, LogicAndOrXor) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    mt.seed(38275);

    std::uniform_int_distribution<int64_t> numberGenerator(0,
                                                           0x7fffffffffffffff);

    for (size_t i = 0; i < 100; ++i) {
        const uint64_t first = numberGenerator(mt);
        const uint64_t second = numberGenerator(mt);

        const multiprecision::int_t<64> firstInt(first);
        const multiprecision::int_t<64> secondInt(second);

        EXPECT_EQ(firstInt & secondInt,
                  multiprecision::int_t<64>(first & second));
        EXPECT_EQ(firstInt | secondInt,
                  multiprecision::int_t<64>(first | second));
        EXPECT_EQ(firstInt ^ secondInt,
                  multiprecision::int_t<64>(first ^ second));
        EXPECT_EQ(~firstInt, multiprecision::int_t<64>(~first));
    }
}