#pragma once

#include <cstddef>
#include <vector>

/** @brief Pure analytics functions for educational price series. */
namespace quant {

/**
 * @brief Compute consecutive simple returns: prices[i] / prices[i-1] - 1.
 * @param prices Finite, strictly positive observations in chronological order.
 * @return A vector of size prices.size()-1, or empty for zero/one observation.
 * @throws std::invalid_argument If any price is nonfinite or not positive.
 * @throws std::overflow_error If a result cannot be represented as double.
 * @note Runs in O(n) time. Returns are fractions: 0.1 means 10 percent.
 */
std::vector<double> simple_returns(const std::vector<double>& prices);

/**
 * @brief Compute simple moving averages for complete windows only.
 * @param values Finite observations; signed values are allowed.
 * @param window Number of observations per window, from 1 to values.size().
 * @return Exactly values.size()-window+1 averages in chronological order.
 * @throws std::invalid_argument If the window is invalid or a value is nonfinite.
 * @throws std::overflow_error If an intermediate or output is not representable.
 * @note O(n) time with a rolling sum and O(n-window+1) output storage.
 * Floating-point results are approximate; this is not exact decimal arithmetic.
 */
std::vector<double> moving_average(const std::vector<double>& values,
                                   std::size_t window);

}  // namespace quant
