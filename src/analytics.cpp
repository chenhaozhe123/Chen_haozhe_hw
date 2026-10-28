#include "quant/analytics.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace {

void validate_prices(const std::vector<double>& prices) {
    for (const double price : prices) {
        if (!std::isfinite(price) || price <= 0.0) {
            throw std::invalid_argument("Prices must be finite and strictly positive");
        }
    }
}

// A positive sum represented as fraction * 2^exponent. Keeping the exponent
// separate prevents overflow even when the sum exceeds the range of double.
struct ScaledSum {
    long double fraction = 0.0L;
    int exponent = 0;
    double maximum = 0.0;
};

ScaledSum make_sum(const double value) {
    int exponent = 0;
    const long double fraction = std::frexp(static_cast<long double>(value), &exponent);
    return {fraction, exponent, value};
}

ScaledSum combine(ScaledSum left, ScaledSum right) {
    if (left.fraction == 0.0L) {
        return right;
    }
    if (right.fraction == 0.0L) {
        return left;
    }
    if (left.exponent < right.exponent) {
        std::swap(left, right);
    }
    const long double total = left.fraction +
                              std::ldexp(right.fraction, right.exponent - left.exponent);
    int shift = 0;
    const long double fraction = std::frexp(total, &shift);
    return {fraction, left.exponent + shift, std::max(left.maximum, right.maximum)};
}

// Two aggregate stacks implement a queue without subtracting a large outgoing
// price from a rounded sum. Every observation is pushed, transferred and popped
// at most once, giving amortized O(1) work per observation.
class MovingWindow {
public:
    explicit MovingWindow(const std::size_t capacity) {
        incoming_.reserve(capacity);
        outgoing_.reserve(capacity);
    }

    void push(const double value) {
        push_stack(incoming_, value);
    }

    void pop() {
        if (outgoing_.empty()) {
            while (!incoming_.empty()) {
                push_stack(outgoing_, incoming_.back().value);
                incoming_.pop_back();
            }
        }
        outgoing_.pop_back();
    }

    double mean(const std::size_t count) const {
        const auto sum = combine(aggregate(incoming_), aggregate(outgoing_));
        const long double mean = std::ldexp(
            sum.fraction / static_cast<long double>(count), sum.exponent);
        // Rounding near the upper limit must not produce infinity: a mean of
        // positive finite prices cannot exceed the largest observation.
        return static_cast<double>(std::min(mean, static_cast<long double>(sum.maximum)));
    }

private:
    struct Entry {
        double value;
        ScaledSum sum;
    };

    static ScaledSum aggregate(const std::vector<Entry>& stack) {
        return stack.empty() ? ScaledSum{} : stack.back().sum;
    }

    static void push_stack(std::vector<Entry>& stack, const double value) {
        const auto sum = combine(aggregate(stack), make_sum(value));
        stack.push_back({value, sum});
    }

    std::vector<Entry> incoming_;
    std::vector<Entry> outgoing_;
};

}  // namespace

namespace quant {

std::vector<double> simple_returns(const std::vector<double>& prices) {
    validate_prices(prices);
    std::vector<double> result;
    if (prices.size() < 2) {
        return result;
    }
    result.reserve(prices.size() - 1);
    for (std::size_t i = 1; i < prices.size(); ++i) {
        const double value = prices[i] / prices[i - 1] - 1.0;
        if (!std::isfinite(value)) {
            throw std::overflow_error("Return cannot be represented as a double");
        }
        result.push_back(value);
    }
    return result;
}

std::vector<double> simple_moving_average(const std::vector<double>& prices,
                                         const std::size_t window) {
    if (window == 0 || window > prices.size()) {
        throw std::invalid_argument("Window must be between 1 and the price count");
    }
    validate_prices(prices);
    if (window == 1) {
        return prices;
    }

    MovingWindow moving_window(window);
    std::vector<double> result;
    result.reserve(prices.size() - window + 1);
    for (std::size_t i = 0; i < prices.size(); ++i) {
        if (i >= window) {
            moving_window.pop();
        }
        moving_window.push(prices[i]);
        if (i >= window - 1) {
            result.push_back(moving_window.mean(window));
        }
    }
    return result;
}

}  // namespace quant
