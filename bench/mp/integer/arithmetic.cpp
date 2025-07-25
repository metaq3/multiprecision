#include <boost/multiprecision/integer.hpp>
#include <gtest/gtest.h>
#include <multiprecision.hpp>
#include <nanobench.h>
#include <random>

#if 0
template <size_t bits>
using int_t =
    boost::multiprecision::number<boost::multiprecision::cpp_int_backend<
        bits, bits, boost::multiprecision::signed_magnitude,
        boost::multiprecision::unchecked, void>>;
#else
template <size_t bits> using int_t = multiprecision::int_t<bits>;
#endif

template <size_t bits, class Distribution, class Device>
int_t<bits> generateRandomNumber(Distribution &distribution, Device &device) {
    int_t<bits> result = 0;

    for (size_t i = 0; i < bits; i += 64) {
        result += distribution(device);

        result <<= 64;
    }

    return result;
}

TEST(Arithmetic, Increment) {
    int_t<256> x(0);

    ankerl::nanobench::Bench().epochIterations(10000000).run(
        "256 bit: ++x", [&]() {
            ++x;

            ankerl::nanobench::detail::doNotOptimizeAway(x);
        });
}

TEST(Arithmetic, Decrement) {
    int_t<256> x(100);

    ankerl::nanobench::Bench().epochIterations(10000000).run(
        "256 bit: --x", [&]() {
            --x;

            ankerl::nanobench::detail::doNotOptimizeAway(x);
        });
}

TEST(Arithmetic, Addition) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(0,
                                                           0x7fffffffffffffff);

    auto bench = ankerl::nanobench::Bench().epochIterations(1000000).epochs(40);

    {
        int_t<256> a = generateRandomNumber<256>(randomGenerator, mt),
                   b = generateRandomNumber<256>(randomGenerator, mt);

        bench.run("256 bit: a + b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a + b);
        });
    }

    {
        int_t<1024> a = generateRandomNumber<1024>(randomGenerator, mt),
                    b = generateRandomNumber<1024>(randomGenerator, mt);

        bench.run("1024 bit: a + b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a + b);
        });
    }

    bench.epochIterations(100000).epochs(20);

    {
        int_t<16384> a = generateRandomNumber<16384>(randomGenerator, mt),
                     b = generateRandomNumber<16384>(randomGenerator, mt);

        bench.run("16384 bit: a + b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a + b);
        });
    }
}

TEST(Arithmetic, Mutliplication) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x0fffffffffffffff,
                                                           0x0fffffffffffffff);

    mt.seed(93726);

    auto bench = ankerl::nanobench::Bench().epochIterations(1000000).epochs(40);

    {
        int_t<256> a = generateRandomNumber<256>(randomGenerator, mt),
                   b = generateRandomNumber<256>(randomGenerator, mt);

        bench.run("256 bit: a * b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a * b);
        });
    }

    {
        int_t<1024> a = generateRandomNumber<1024>(randomGenerator, mt),
                    b = generateRandomNumber<1024>(randomGenerator, mt);

        bench.run("1024 bit: a * b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a * b);
        });
    }

    bench.epochIterations(100000).epochs(10);

    {
        int_t<16384> a = generateRandomNumber<16384>(randomGenerator, mt),
                     b = generateRandomNumber<16384>(randomGenerator, mt);

        bench.run("16384 bit: a * b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a * b);
        });
    }
}

TEST(Arithmetic, Division) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x0fffffffffffffff,
                                                           0x0fffffffffffffff);

    auto bench = ankerl::nanobench::Bench().epochIterations(100000).epochs(30);

    {
        int_t<256> a = generateRandomNumber<256>(randomGenerator, mt),
                   b = generateRandomNumber<128>(randomGenerator, mt);

        bench.run("256 bit: a / b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a / b);
        });
    }

    {
        int_t<1024> a = generateRandomNumber<1024>(randomGenerator, mt),
                    b = generateRandomNumber<128>(randomGenerator, mt);

        bench.run("1024 bit: a / b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a / b);
        });
    }

    bench.epochIterations(10000).epochs(30);

    {
        int_t<16384> a = generateRandomNumber<16384>(randomGenerator, mt),
                     b = generateRandomNumber<128>(randomGenerator, mt);

        bench.run("16384 bit: a / b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a / b);
        });
    }
}