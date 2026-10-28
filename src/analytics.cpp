#include "quant/analytics.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace {

double checked_result(long double value) {
    const auto limit = static_cast<long double>(std::numeric_limits<double>::max());
    if (!std::isfinite(value) || value > limit || value < -limit) {
        throw std::overflow_error("calculation is not representable as double");
    }
    return static_cast<double>(value);
}

}  // namespace

namespace quant {

std::vector<double> simple_returns(const std::vector<double>& prices) {
    for (const double price : prices) {
        if (!std::isfinite(price) || price <= 0.0) {
            throw std::invalid_argument("prices must be finite and strictly positive");
        }
    }
    std::vector<double> result;
    if (prices.size() < 2) {
        return result;
    }
    result.reserve(prices.size() - 1);
    for (std::size_t i = 1; i < prices.size(); ++i) {
        const long double change = static_cast<long double>(prices[i]) /
                                       static_cast<long double>(prices[i - 1]) -
                                   1.0L;
        result.push_back(checked_result(change));
    }
    return result;
}

std::vector<double> moving_average(const std::vector<double>& values,
                                   std::size_t window) {
    if (window == 0 || window > values.size()) {
        throw std::invalid_argument("window must be between one and the input length");
    }
    for (const double value : values) {
        if (!std::isfinite(value)) {
            throw std::invalid_argument("observations must be finite");
        }
    }
    const auto width = static_cast<long double>(window);
    const auto limit = static_cast<long double>(std::numeric_limits<double>::max());
    long double magnitude = 0.0L;
    for (const double value : values) {
        magnitude = std::max(magnitude, std::fabs(static_cast<long double>(value)));
    }
    // Sum unscaled values when safe, preserving even subnormal observations.
    // Near the finite limit use half-mean terms, leaving headroom on platforms
    // where long double has no wider exponent range than double (MSVC).
    const bool needs_scaling = magnitude > limit / (2.0L * width);
    const long double input_divisor = needs_scaling ? 2.0L * width : 1.0L;
    long double sum = 0.0L;
    long double correction = 0.0L;
    const auto add = [&](long double term) {
        const long double next = sum + term;
        // Neumaier compensation retains small terms when large terms leave.
        if (std::fabs(sum) >= std::fabs(term)) {
            correction += (sum - next) + term;
        } else {
            correction += (term - next) + sum;
        }
        sum = next;
    };
    const auto current_mean = [&]() {
        const long double corrected = sum + correction;
        if (!std::isfinite(corrected)) {
            throw std::overflow_error("moving average intermediate overflow");
        }
        if (needs_scaling) {
            // A finite-input mean lies inside [-DBL_MAX, DBL_MAX]. Clamp the
            // half-mean before doubling to remove boundary rounding overflow.
            return checked_result(std::clamp(corrected, -limit / 2.0L,
                                               limit / 2.0L) * 2.0L);
        }
        return checked_result(corrected / width);
    };
    for (std::size_t i = 0; i < window; ++i) {
        add(static_cast<long double>(values[i]) / input_divisor);
    }
    std::vector<double> result;
    result.reserve(values.size() - window + 1);
    result.push_back(current_mean());
    for (std::size_t i = window; i < values.size(); ++i) {
        add(-static_cast<long double>(values[i - window]) / input_divisor);
        add(static_cast<long double>(values[i]) / input_divisor);
        result.push_back(current_mean());
    }
    return result;
}

}  // namespace quant
