#include "quant/analytics.hpp"
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <limits>
#include <stdexcept>
#include <vector>

using Catch::Approx;

TEST_CASE("Returns preserve chronological order and use previous price", "[returns]") {
    const auto result = quant::simple_returns({100.0, 110.0, 99.0});
    REQUIRE(result.size() == 2);
    CHECK(result[0] == Approx(0.1));
    CHECK(result[1] == Approx(-0.1));
}
TEST_CASE("Returns handle empty and single observation", "[returns]") {
    CHECK(quant::simple_returns({}).empty());
    CHECK(quant::simple_returns({100.0}).empty());
}
TEST_CASE("Unchanged prices have zero return", "[returns]") {
    CHECK(quant::simple_returns({42.0, 42.0, 42.0}) == std::vector<double>{0.0, 0.0});
}
TEST_CASE("All prices including a singleton are validated", "[returns]") {
    for (const double bad : {0.0, -1.0, std::numeric_limits<double>::infinity(),
                              std::numeric_limits<double>::quiet_NaN()}) {
        CHECK_THROWS_AS(quant::simple_returns({100.0, bad}), std::invalid_argument);
        CHECK_THROWS_AS(quant::simple_returns({bad, 100.0}), std::invalid_argument);
        CHECK_THROWS_AS(quant::simple_returns({bad}), std::invalid_argument);
    }
}
TEST_CASE("Unrepresentable return reports overflow", "[returns]") {
    CHECK_THROWS_AS(quant::simple_returns({std::numeric_limits<double>::min(),
                                         std::numeric_limits<double>::max()}),
                    std::overflow_error);
}
TEST_CASE("Very small positive prices remain valid", "[returns]") {
    const double tiny = std::numeric_limits<double>::min();
    const auto result = quant::simple_returns({tiny, 2.0 * tiny});
    REQUIRE(result.size() == 1);
    CHECK(result[0] == Approx(1.0));
}
TEST_CASE("Moving average emits only complete windows", "[average]") {
    const auto result = quant::moving_average({1, 2, 3, 4, 5}, 3);
    REQUIRE(result.size() == 3);
    CHECK(result[0] == Approx(2.0));
    CHECK(result[1] == Approx(3.0));
    CHECK(result[2] == Approx(4.0));
}
TEST_CASE("Window one preserves observations", "[average]") {
    CHECK(quant::moving_average({1.5, -2.5, 3.5}, 1) ==
          std::vector<double>{1.5, -2.5, 3.5});
}
TEST_CASE("Full width returns a single mean", "[average]") {
    const auto result = quant::moving_average({2, 4, 6, 8}, 4);
    REQUIRE(result.size() == 1);
    CHECK(result[0] == Approx(5.0));
}
TEST_CASE("Signed observations are accepted", "[average]") {
    const auto result = quant::moving_average({-2, 2, -2, 2}, 2);
    REQUIRE(result.size() == 3);
    for (const double value : result) CHECK(value == Approx(0.0).margin(1e-12));
}
TEST_CASE("Invalid windows including empty input are rejected", "[average]") {
    CHECK_THROWS_AS(quant::moving_average({1, 2, 3}, 0), std::invalid_argument);
    CHECK_THROWS_AS(quant::moving_average({1, 2, 3}, 4), std::invalid_argument);
    CHECK_THROWS_AS(quant::moving_average({}, 1), std::invalid_argument);
}
TEST_CASE("Nonfinite observations anywhere are rejected", "[average]") {
    for (const double bad : {std::numeric_limits<double>::infinity(),
                             -std::numeric_limits<double>::infinity(),
                              std::numeric_limits<double>::quiet_NaN()}) {
        CHECK_THROWS_AS(quant::moving_average({bad, 1, 2}, 2), std::invalid_argument);
        CHECK_THROWS_AS(quant::moving_average({1, 2, bad}, 2), std::invalid_argument);
    }
}
TEST_CASE("Large finite averages avoid unnecessary overflow", "[average]") {
    const double large = std::numeric_limits<double>::max();
    const auto result = quant::moving_average({large, large}, 2);
    REQUIRE(result.size() == 1);
    CHECK(result[0] == Approx(large));
}

TEST_CASE("Small values survive when a large value leaves the window", "[average]") {
    const auto result = quant::moving_average({1e20, 1.0, 1.0}, 2);
    REQUIRE(result.size() == 2);
    CHECK(result[1] == Approx(1.0));
}
TEST_CASE("Rounding at the upper finite boundary does not cause false overflow", "[average]") {
    const double large = std::numeric_limits<double>::max();
    const auto result = quant::moving_average(std::vector<double>(7, large), 7);
    REQUIRE(result.size() == 1);
    CHECK(result[0] == Approx(large));
}
TEST_CASE("Representable subnormal means are retained", "[average]") {
    const double tiny = std::numeric_limits<double>::denorm_min();
    const auto result = quant::moving_average({tiny, tiny}, 2);
    REQUIRE(result.size() == 1);
    CHECK(result[0] == tiny);
}
