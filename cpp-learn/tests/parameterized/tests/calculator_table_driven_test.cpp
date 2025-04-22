#include <gtest/gtest.h>

#include <vector>

#include "calculator.hpp"

TEST(CalculatorTableDrivenTest, Sum)
{
    struct Test {
        int a;
        int b;
        int sum;
    };

    const std::vector<Test> tests{{
        {1, 2, 3},
        {4, 5, 9},
    }};

    for (const auto& test : tests) {
        const auto actual = calculator::sum(test.a, test.b);
        const auto expected = test.sum;
        EXPECT_EQ(actual, expected);
    }
}
