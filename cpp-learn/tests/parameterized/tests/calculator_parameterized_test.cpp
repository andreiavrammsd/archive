#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <string>

#include "calculator.hpp"
#include "json.hpp"

namespace {

struct Test {
    int a;
    int b;
    int sum;
};

std::vector<Test> GetTests()
{
    return {
        {1, 2, 3},
        {4, 5, 9},
    };
}

void from_json(const nlohmann::json& j, Test& test)
{
    test.a = j.at("a");
    test.b = j.at("b");
    test.sum = j.at("sum");
}

std::vector<Test> GetTests(const std::string& path)
{
    std::ifstream input(path);
    nlohmann::json j;
    input >> j;
    return j;
}

std::string PrintToString(const Test& test)
{
    std::stringstream ss;
    ss << "> " << test.a << " + " << test.b << " = " << test.sum;
    return ss.str();
}

}  // namespace

class CalculatorParameterizedTest : public testing::TestWithParam<Test> {
};

TEST_P(CalculatorParameterizedTest, Sum)
{
    const auto actual = calculator::sum(GetParam().a, GetParam().b);
    const auto expected = GetParam().sum;
    EXPECT_EQ(actual, expected);
}

// List of parameters
// clang-format off
INSTANTIATE_TEST_SUITE_P(InlineValues, CalculatorParameterizedTest, testing::Values(
    Test{1, 2, 3},
    Test{4, 5, 9}
));
// clang-format on

// Container of parameters
INSTANTIATE_TEST_SUITE_P(Container, CalculatorParameterizedTest, testing::ValuesIn(GetTests()));

// Parameters from json file
INSTANTIATE_TEST_SUITE_P(Json, CalculatorParameterizedTest, testing::ValuesIn(GetTests("input.json")));
