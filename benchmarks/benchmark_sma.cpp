#include <benchmark/benchmark.h>

#include "quant/analytics.hpp"

#include <cmath>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace {

std::vector<double> make_prices(const std::size_t size) {
    std::vector<double> prices(size);
    for (std::size_t i = 0; i < size; ++i) {
        prices[i] = 100.0 + static_cast<double>(i % 100) * 0.01;
    }
    return prices;
}

// Baseline deliberately recomputes each window: O(n * window).
// Match the production function's validation and allocation work.
std::vector<double> naive_sma(const std::vector<double>& prices, const std::size_t window) {
    if (window == 0 || window > prices.size()) {
        throw std::invalid_argument("Invalid window");
    }
    for (const double price : prices) {
        if (!std::isfinite(price) || price <= 0.0) {
            throw std::invalid_argument("Invalid price");
        }
    }
    std::vector<double> result;
    result.reserve(prices.size() - window + 1);
    const long double divisor = static_cast<long double>(window);
    for (std::size_t i = 0; i <= prices.size() - window; ++i) {
        long double average = 0.0L;
        for (std::size_t j = 0; j < window; ++j) {
            average += static_cast<long double>(prices[i + j]) / divisor;
        }
        result.push_back(static_cast<double>(average));
    }
    return result;
}

void BM_RollingSMA(benchmark::State& state) {
    const auto prices = make_prices(static_cast<std::size_t>(state.range(0)));
    const auto window = static_cast<std::size_t>(state.range(1));
    for (auto _ : state) {
        const auto result = quant::simple_moving_average(prices, window);
        benchmark::DoNotOptimize(result.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(prices.size()));
}

void BM_NaiveSMA(benchmark::State& state) {
    const auto prices = make_prices(static_cast<std::size_t>(state.range(0)));
    const auto window = static_cast<std::size_t>(state.range(1));
    for (auto _ : state) {
        const auto result = naive_sma(prices, window);
        benchmark::DoNotOptimize(result.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * static_cast<std::int64_t>(prices.size()));
}

BENCHMARK(BM_RollingSMA)->Args({1024, 20})->Args({16384, 20})->Args({65536, 100});
BENCHMARK(BM_NaiveSMA)->Args({1024, 20})->Args({16384, 20})->Args({65536, 100});

}  // namespace
