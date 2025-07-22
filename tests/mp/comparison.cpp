#include <random>
#define DEFAULT_INTEGRAL_TYPE uint16_t

#include <gtest/gtest.h>
#include <multiprecision.hpp>

TEST(Comparison, Equality) {
    multiprecision::int_t<64> a(48);
    multiprecision::int_t<64> b(64);
    multiprecision::int_t<64> c(48);

    EXPECT_EQ(a, int64_t(48));
    EXPECT_NE(a, int64_t(49));
    EXPECT_NE(a, int64_t(38));
    EXPECT_NE(a, int64_t(0));
    EXPECT_NE(a, int64_t(-48));

    EXPECT_NE(b, int64_t(48));
    EXPECT_EQ(b, int64_t(64));
    EXPECT_NE(b, int64_t(38));
    EXPECT_NE(b, int64_t(0));
    EXPECT_NE(b, int64_t(-48));

    EXPECT_EQ(a, c);
    EXPECT_NE(b, a);
    EXPECT_NE(c, b);
}

TEST(Comparison, LessAndGreater) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x7fffffffffffffff,
                                                           0x7fffffffffffffff);

    for (size_t i = 0; i < 1000000; ++i) {
        const int64_t first = randomGenerator(mt);
        const int64_t second = randomGenerator(mt);

        const multiprecision::int_t<64> a(first);
        const multiprecision::int_t<64> b(second);

        EXPECT_EQ(first < second, a < b);
        EXPECT_EQ(first > second, a > b);
    }
}