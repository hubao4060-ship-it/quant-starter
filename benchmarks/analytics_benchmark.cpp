#include "quant/analytics.hpp"
#include <benchmark/benchmark.h>
#include <cstddef>
#include <vector>

namespace {

std::vector<double> make_prices(std::size_t size) {
    std::vector<double> prices(size);
    for (std::size_t i = 0; i < size; ++i) {
        prices[i] = 100.0 + static_cast<double>(i % 1000) * 0.01;
    }
    return prices;
}

void BM_SimpleReturns(benchmark::State& state) {
    const auto prices = make_prices(static_cast<std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto result = quant::simple_returns(prices);
        benchmark::DoNotOptimize(result.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

void BM_MovingAverage(benchmark::State& state) {
    const auto prices = make_prices(static_cast<std::size_t>(state.range(0)));
    for (auto _ : state) {
        auto result = quant::moving_average(prices, 20);
        benchmark::DoNotOptimize(result.data());
        benchmark::ClobberMemory();
    }
    state.SetItemsProcessed(state.iterations() * state.range(0));
}

BENCHMARK(BM_SimpleReturns)->Arg(1000)->Arg(100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_MovingAverage)->Arg(1000)->Arg(100000)->Unit(benchmark::kMicrosecond);

}  // namespace

BENCHMARK_MAIN();
