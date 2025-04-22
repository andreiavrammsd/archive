#include <gtest/gtest.h>

#include "calculator.hpp"

TEST(CalculatorSimpleTest, Sum)
{
    const auto actual = calculator::sum(1, 2);
    const auto expected = 3;
    EXPECT_EQ(actual, expected);
}
