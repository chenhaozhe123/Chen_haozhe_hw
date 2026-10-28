#include "quant/analytics.hpp"

#include <iomanip>
#include <iostream>
#include <vector>

namespace {

void print_values(const char* label, const std::vector<double>& values) {
    std::cout << label;
    for (const double value : values) {
        std::cout << ' ' << value;
    }
    std::cout << '\n';
}

}  // namespace

int main() {
    const std::vector<double> prices{100.0, 102.0, 101.0, 105.0, 107.0};
    std::cout << std::fixed << std::setprecision(6);
    print_values("Prices:", prices);
    print_values("Simple returns:", quant::simple_returns(prices));
    print_values("SMA (window=3):", quant::simple_moving_average(prices, 3));
}
