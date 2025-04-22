#include <any>
#include <cassert>
#include <functional>
#include <iostream>
#include <numeric>
#include <ostream>
#include <variant>
#include <vector>

enum class Type {
    kApple = 1,
    kBanana = 2,
    kPear = 3,
};

enum class Color {
    kGreen = 1,
    kRed = 2,
    kYellow = 3,
};

using Weight = int;

struct Fruit {
    Type type;
    Color color;
    Weight weight;
};

struct Filter {
    std::vector<Type> types;
    std::vector<Color> colors;
    Weight weight_more_than;
};

using Fruits = std::vector<Fruit>;
using Types = std::vector<Type>;
using Colors = std::vector<Color>;
using FilterValue = std::any;  // using FilterValue = std::variant<Fruits, Weight>;
using FruitsFilter = std::function<FilterValue(FilterValue)>;
using Criteria = std::vector<std::function<FilterValue(FilterValue)>>;

std::ostream& operator<<(std::ostream& out, Type type)
{
    out << static_cast<int>(type);
    return out;
}

const auto filter_pred = [](const FilterValue& input, auto&& find) -> FilterValue {
    auto fruits = std::any_cast<Fruits>(input);

    const auto it = std::remove_if(fruits.begin(), fruits.end(), find);
    fruits.erase(it, fruits.cend());

    return fruits;
};

FruitsFilter select_by_types(const Types& types)
{
    return [types](const FilterValue& input) -> FilterValue {
        const auto match = [types](const auto& fruit) {
            return std::find(types.cbegin(), types.cend(), fruit.type) == types.cend();
        };
        return filter_pred(input, match);
    };
}

FruitsFilter select_by_colors(const Colors& colors)
{
    return [colors](const FilterValue& input) -> FilterValue {
        const auto match = [colors](const auto& fruit) {
            return std::find(colors.cbegin(), colors.cend(), fruit.color) == colors.cend();
        };
        return filter_pred(input, match);
    };
}

FruitsFilter select_if_weighs_more_than(Weight weight)
{
    return [weight](const FilterValue& input) -> FilterValue {
        const auto match = [weight](auto fruit) { return fruit.weight > weight; };
        return filter_pred(input, std::not_fn(match));
    };
}

const auto get_total_weight = [](const FilterValue& input) -> FilterValue {
    const auto fruits = std::any_cast<Fruits>(input);

    const auto op = [](const auto& total, const auto& fruit) { return total + fruit.weight; };
    return std::accumulate(fruits.cbegin(), fruits.cend(), 0, op);
};

static const auto list = [](std::ostream& out) -> FruitsFilter {
    return [&out](const FilterValue& input) -> FilterValue {
        const auto fruits = std::any_cast<Fruits>(input);

        for (auto&& fruit : fruits) {
            out << fruit.type << ":" << fruit.weight << "\n";
        }

        out << "\n";
        return fruits;
    };
};

int main()
{
    Fruits fruits = {
        // clang-format off
        {Type::kApple, Color::kGreen, 30},
        {Type::kApple, Color::kGreen, 130},
        {Type::kApple, Color::kYellow, 130},
        {Type::kApple, Color::kGreen, 330},
        {Type::kApple, Color::kGreen, 630},
        {Type::kApple, Color::kRed, 630},
        {Type::kBanana, Color::kGreen, 130},
        {Type::kApple, Color::kYellow, 131},
        {Type::kPear, Color::kGreen, 30},
        {Type::kPear, Color::kGreen, 180},
        // clang-format on
    };

    const auto filter = Filter{
        {Type::kApple, Type::kPear},
        {Color::kGreen, Color::kRed},
        130,
    };

    Criteria criteria = {
        select_by_types(filter.types),
        select_by_colors(filter.colors),
        select_if_weighs_more_than(filter.weight_more_than),
        list(std::cout),
        get_total_weight,
    };

    const auto get_weight = [](const Fruits& fruits, const Criteria& criteria) -> Weight {
        FilterValue out = fruits;
        for (const auto& criterion : criteria) {
            out = criterion(out);
        }
        return std::any_cast<Weight>(out);
    };

    assert(get_weight(fruits, criteria) == 1770);
}
