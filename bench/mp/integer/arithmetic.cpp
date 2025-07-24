#include <gtest/gtest.h>
#include <multiprecision.hpp>
#include <nanobench.h>
#include <random>

TEST(Arithmetic, Increment) {
    multiprecision::int_t<256> x(0);

    ankerl::nanobench::Bench().epochIterations(10000000).run(
        "256 bit: ++x", [&]() {
            ++x;

            ankerl::nanobench::detail::doNotOptimizeAway(x);
        });
}

TEST(Arithmetic, Decrement) {
    multiprecision::int_t<256> x(100);

    ankerl::nanobench::Bench().epochIterations(10000000).run(
        "256 bit: --x", [&]() {
            --x;

            ankerl::nanobench::detail::doNotOptimizeAway(x);
        });
}

TEST(Arithmetic, Addition) {
    std::random_device rd;
    std::mt19937_64 mt(rd());

    std::uniform_int_distribution<int64_t> randomGenerator(-0x0fffffffffffffff,
                                                           0x0fffffffffffffff);

    auto bench = ankerl::nanobench::Bench().epochIterations(1000000).epochs(40);

    {
        multiprecision::int_t<256> a = randomGenerator(mt),
                                   b = randomGenerator(mt);

        bench.run("256 bit: a + b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a + b);
        });
    }

    {
        multiprecision::int_t<1024> a = randomGenerator(mt),
                                    b = randomGenerator(mt);

        bench.run("1024 bit: a + b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a + b);
        });
    }

    bench.epochIterations(100000).epochs(20);

    {
        multiprecision::int_t<16384> a = randomGenerator(mt),
                                     b = randomGenerator(mt);

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

    auto bench = ankerl::nanobench::Bench().epochIterations(1000000).epochs(40);

    {
        multiprecision::int_t<256> a = randomGenerator(mt),
                                   b = randomGenerator(mt);

        bench.run("256 bit: a * b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a * b);
        });
    }

    {
        multiprecision::int_t<1024> a = randomGenerator(mt),
                                    b = randomGenerator(mt);

        bench.run("1024 bit: a * b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a * b);
        });
    }

    bench.epochIterations(100000).epochs(10);

    {
        multiprecision::int_t<16384> a = randomGenerator(mt),
                                     b = randomGenerator(mt);

        bench.run("16384 bit: a *= b", [&]() {
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
        multiprecision::int_t<256> a = randomGenerator(mt),
                                   b = randomGenerator(mt);

        bench.run("256 bit: a / b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a / b);
        });
    }

    {
        multiprecision::int_t<1024> a = randomGenerator(mt),
                                    b = randomGenerator(mt);

        bench.run("1024 bit: a / b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a / b);
        });
    }

    bench.epochIterations(10000).epochs(30);

    {
        multiprecision::int_t<16384> a = randomGenerator(mt),
                                     b = randomGenerator(mt);

        bench.run("16384 bit: a / b", [&]() {
            ankerl::nanobench::detail::doNotOptimizeAway(a / b);
        });
    }
}