# QuantStarter API

QuantStarter is a C++17 library and console example for chronological price series.
The public API lives in `quant/analytics.hpp` in namespace `quant`.

## Example

```cpp
#include "quant/analytics.hpp"
#include <vector>

const std::vector<double> prices{100.0, 102.0, 101.0, 105.0, 107.0};
const auto returns = quant::simple_returns(prices);
const auto sma = quant::simple_moving_average(prices, 3);
// returns: 0.02, -0.00980392..., 0.03960396..., 0.01904762...
// sma: 101.0, 102.666666..., 104.333333...
```

## Contracts and complexity

Both functions accept finite, strictly positive prices in chronological order.
Invalid prices raise `std::invalid_argument`.

`quant::simple_returns` produces one return for each adjacent pair, takes O(n)
time and uses O(n) output storage. Empty and singleton inputs return an empty
vector. A non-finite calculated return raises `std::overflow_error`.

`quant::simple_moving_average` requires a window between one and the input length;
otherwise it raises `std::invalid_argument`. Only complete windows are emitted,
with the first output aligned to input index `window - 1`. It takes O(n) total
time, O(window) auxiliary storage and O(n) output storage. Two aggregate stacks
maintain the window as a queue without subtracting outgoing prices. Each value
is pushed, transferred and popped at most once. Normalized fractions with separate
binary exponents avoid sum overflow and premature underflow for subnormal inputs.
Results remain subject to ordinary floating-point rounding.

## Engineering workflow

Use CMake to build, CTest/doctest for correctness and Google Benchmark for timing.
The GitHub Actions workflow repeats these checks and builds this documentation
on pushes and pull requests. Successful default-branch runs publish it to Pages.
See the repository README for exact commands and dependency information.
