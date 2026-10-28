#include "quant/analytics.hpp"

#include <exception>
#include <iomanip>
#include <iostream>
#include <vector>

namespace {
void print_series(const char* label, const std::vector<double>& values) {
    std::cout << label << ": [";
    for (std::size_t i = 0; i < values.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << values[i];
    }
    std::cout << "]\n";
}
}  // namespace

int main() {
    try {
        const std::vector<double> prices{100.0, 110.0, 99.0, 108.9, 119.79};
        std::cout << std::fixed << std::setprecision(8);
        print_series("Prices", prices);
        print_series("Simple returns (fraction)", quant::simple_returns(prices));
        print_series("Moving average (window=3)", quant::moving_average(prices, 3));
    } catch (const std::exception& error) {
        std::cerr << "Calculation failed: " << error.what() << '\n';
        return 1;
    }
}
