#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>

#include "quant/analytics.hpp"

#include <cmath>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

TEST_CASE("Returns match a hand-calculated gain and loss") {
    const auto result = quant::simple_returns({100.0, 110.0, 99.0});
    REQUIRE(result.size() == 2);
    CHECK(result[0] == doctest::Approx(0.1));
    CHECK(result[1] == doctest::Approx(-0.1));
}

TEST_CASE("Empty and singleton return series have no intervals") {
    CHECK(quant::simple_returns({}).empty());
    CHECK(quant::simple_returns({100.0}).empty());
}

TEST_CASE("Unchanged prices produce zero returns") {
    CHECK(quant::simple_returns({10.0, 10.0, 10.0}) == std::vector<double>{0.0, 0.0});
}

TEST_CASE("Non-positive prices are rejected by both functions") {
    for (const double invalid : {0.0, -1.0}) {
        CHECK_THROWS_AS(quant::simple_returns({100.0, invalid}), std::invalid_argument);
        CHECK_THROWS_AS(quant::simple_moving_average({100.0, invalid}, 2),
                        std::invalid_argument);
    }
}

TEST_CASE("Non-finite prices are rejected even in a singleton series") {
    const double infinity = std::numeric_limits<double>::infinity();
    for (const double invalid : {infinity, -infinity,
                                 std::numeric_limits<double>::quiet_NaN()}) {
        CHECK_THROWS_AS(quant::simple_returns({invalid}), std::invalid_argument);
        CHECK_THROWS_AS(quant::simple_moving_average({invalid}, 1),
                        std::invalid_argument);
    }
}

TEST_CASE("An unrepresentable return raises an explicit error") {
    CHECK_THROWS_AS(quant::simple_returns({std::numeric_limits<double>::min(),
                                          std::numeric_limits<double>::max()}),
                    std::overflow_error);
}

TEST_CASE("SMA emits complete trailing windows") {
    const auto result = quant::simple_moving_average({100.0, 102.0, 101.0, 105.0, 107.0}, 3);
    REQUIRE(result.size() == 3);
    CHECK(result[0] == doctest::Approx(101.0));
    CHECK(result[1] == doctest::Approx(308.0 / 3.0));
    CHECK(result[2] == doctest::Approx(313.0 / 3.0));
}

TEST_CASE("Window one returns each observation exactly") {
    const std::vector<double> prices{1.25, 2.75, 3.5};
    CHECK(quant::simple_moving_average(prices, 1) == prices);
}

TEST_CASE("A full-series window emits one mean") {
    const auto result = quant::simple_moving_average({1.0, 2.0, 6.0}, 3);
    REQUIRE(result.size() == 1);
    CHECK(result[0] == doctest::Approx(3.0));
}

TEST_CASE("Invalid windows and empty SMA input are rejected") {
    CHECK_THROWS_AS(quant::simple_moving_average({1.0, 2.0}, 0), std::invalid_argument);
    CHECK_THROWS_AS(quant::simple_moving_average({1.0, 2.0}, 3), std::invalid_argument);
    CHECK_THROWS_AS(quant::simple_moving_average({}, 1), std::invalid_argument);
}

TEST_CASE("Large equal prices do not overflow the moving average") {
    const double large = std::numeric_limits<double>::max();
    const auto result = quant::simple_moving_average({large, large, large}, 2);
    REQUIRE(result.size() == 2);
    for (const double average : result) {
        CHECK(std::isfinite(average));
        CHECK(average == doctest::Approx(large));
    }
}

TEST_CASE("Rolling SMA agrees with independently recomputed window means") {
    std::mt19937 generator(42);
    std::uniform_real_distribution<double> distribution(1.0, 1000.0);
    std::vector<double> prices(257);
    for (double& price : prices) {
        price = distribution(generator);
    }
    for (const std::size_t window : {1u, 2u, 20u, 100u, 257u}) {
        const auto actual = quant::simple_moving_average(prices, window);
        REQUIRE(actual.size() == prices.size() - window + 1);
        for (std::size_t i = 0; i < actual.size(); ++i) {
            long double sum = 0.0L;
            for (std::size_t j = 0; j < window; ++j) {
                sum += prices[i + j];
            }
            CHECK(actual[i] == doctest::Approx(static_cast<double>(sum / window))
                                  .epsilon(1e-11));
        }
    }
}

TEST_CASE("Calculations leave the input unchanged") {
    const std::vector<double> prices{10.0, 12.0, 11.0};
    const auto original = prices;
    quant::simple_returns(prices);
    quant::simple_moving_average(prices, 2);
    CHECK(prices == original);
}

TEST_CASE("Small observations survive when a large price leaves the window") {
    const auto result = quant::simple_moving_average({1e16, 1.0, 1.0}, 2);
    REQUIRE(result.size() == 2);
    CHECK(result.back() == 1.0);

    const auto wider = quant::simple_moving_average({1e308, 1e100, 1.0, 1.0, 1.0}, 3);
    REQUIRE(wider.size() == 3);
    CHECK(wider.back() == 1.0);
}

TEST_CASE("Subnormal prices are not divided before summation") {
    const double tiny = std::numeric_limits<double>::denorm_min();
    for (const std::size_t window : {2u, 3u, 7u}) {
        const auto result = quant::simple_moving_average(std::vector<double>(15, tiny), window);
        REQUIRE(result.size() == 15 - window + 1);
        for (const double average : result) {
            CHECK(average == tiny);
        }
    }
}

TEST_CASE("Large averages stay finite across stack transfers and window sizes") {
    const double largest = std::numeric_limits<double>::max();
    for (const std::size_t window : {2u, 3u, 7u, 31u}) {
        const auto result = quant::simple_moving_average(std::vector<double>(67, largest), window);
        for (const double average : result) {
            CHECK(std::isfinite(average));
            CHECK(average == doctest::Approx(largest));
        }
    }
}

TEST_CASE("A mixed extreme series remains correct after old prices expire") {
    const double tiny = std::numeric_limits<double>::denorm_min();
    const double large = std::numeric_limits<double>::max();
    const auto result = quant::simple_moving_average({large, tiny, tiny, large, tiny, tiny}, 2);
    REQUIRE(result.size() == 5);
    CHECK(result[0] == doctest::Approx(large / 2.0));
    CHECK(result[1] == tiny);
    CHECK(result[2] == doctest::Approx(large / 2.0));
    CHECK(result[3] == doctest::Approx(large / 2.0));
    CHECK(result[4] == tiny);
}
