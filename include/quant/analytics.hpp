#pragma once

#include <cstddef>
#include <vector>

/** @file analytics.hpp
 *  @brief Price-series calculations used by the example and benchmark.
 */

/** @brief Small quantitative-analysis library. */
namespace quant {

/**
 * @brief Calculate consecutive simple returns: price[i] / price[i-1] - 1.
 * @param prices Finite, strictly positive prices in chronological order.
 * @return One fewer value than prices; empty for zero or one price.
 * @throws std::invalid_argument If any price is non-positive or non-finite.
 * @throws std::overflow_error If a return cannot be represented as a double.
 * @note Complexity: O(n) time and O(n) output storage.
 */
std::vector<double> simple_returns(const std::vector<double>& prices);

/**
 * @brief Calculate a trailing simple moving average (SMA).
 * @param prices Finite, strictly positive prices in chronological order.
 * @param window Number of observations per average; must be in [1, prices.size()].
 * @return prices.size() - window + 1 complete-window averages. The first
 *         output corresponds to input index window - 1; no padding is added.
 * @throws std::invalid_argument If the window or any price is invalid.
 * @note Complexity: O(n) total time, O(window) auxiliary storage, O(n) output
 *       storage. Scaled sums and two aggregate stacks avoid sum overflow and
 *       cancellation when a large price leaves the window. Results are still
 *       subject to floating-point rounding.
 */
std::vector<double> simple_moving_average(const std::vector<double>& prices,
                                         std::size_t window);

}  // namespace quant
