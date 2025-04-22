/*
 https://www.fluentcpp.com/2020/05/15/runtime-polymorphism-without-virtual-functions/
 */

#include <vector>
#include <iostream>
#include <algorithm>

struct Input {
    double value{};
};

struct Output {
    double value{};
};

struct BigCalculator {
    static bool handles(const Input &input) {
        return input.value > 10;
    }

    static Output compute(const Input &input) {
        return Output{input.value * 5};
    }

    static void log(const Input &input, const Output &output) {
        std::cout << "BigCalculator took an input of " << input.value
                  << " and produced an output of " << output.value << '\n';
    }
};

struct SmallCalculator {
    static bool handles(const Input &input) {
        return input.value <= 10;
    }

    static Output compute(const Input &input) {
        return Output{input.value + 2};
    }

    static void log(const Input &input, const Output &output) {
        std::cout << "SmallCalculator took an input of " << input.value
                  << " and produced an output of " << output.value << '\n';
    }
};

struct Calculator {
    bool (*handles)(const Input &input);

    Output (*compute)(const Input &input);

    void (*log)(const Input &input, const Output &output);

    template<typename CalculatorImplementation>
    static Calculator createFrom() {
        return Calculator{
                &CalculatorImplementation::handles,
                &CalculatorImplementation::compute,
                &CalculatorImplementation::log,
        };
    }
};

std::vector<Calculator> getCalculators() {
    return {
            Calculator::createFrom<BigCalculator>(),
            Calculator::createFrom<SmallCalculator>(),
    };
}

void calculate(const Input &input, const std::vector<Calculator> &calculators) {
    auto const calculator = std::find_if(begin(calculators), end(calculators),
                                         [&input](auto &&calc) { return calc.handles(input); });

    if (calculator != end(calculators)) {
        auto const output = calculator->compute(input);
        calculator->log(input, output);
    }
}

int main() {
    auto const calculators = getCalculators();

    auto const bigInput = Input{50};
    calculate(bigInput, calculators);

    auto const smallInput = Input{10};
    calculate(smallInput, calculators);
}
