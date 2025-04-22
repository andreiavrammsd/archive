/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <vector>
#include <tuple>
#include <cassert>
#include <typeinfo>

struct Record {
    int id;
    std::string name;
};

std::tuple<std::vector<Record>, std::string> searchRecordsByName(const std::vector<Record> &, const std::string &);

void f(std::tuple<std::string, int, std::string, int> &&t) {
    assert(std::get<1>(t) == 1);
}

int main() {
    auto t1 = std::make_tuple("A", 1);
    auto t2 = std::make_tuple("B", 2);

    // Swap
    t1.swap(t2);
    assert(std::get<0>(t1) == std::string{"B"});
    assert(std::get<1>(t2) == 1);

    // Move
    auto t3 = std::move(t1);
    assert(std::get<0>(t3) == std::string{"B"});

    // Concatenate tuples
    auto t4 = std::tuple_cat(t2, t3);
    assert(std::tuple_size<decltype(t4)>::value == 4);
    assert(std::get<3>(t4) == 2);

    // Forward
    f(std::forward_as_tuple("A", 1, "B", 2));

    // Tie
    std::string a1;
    int a2{};
    std::tie(a1, std::ignore) = t2;
    assert(a1 == "A");
    assert(a2 == 0);

    // Return
    std::vector<Record> records = {
            {1, "A"},
            {2, "B"},
            {3, "A"},
            {4, "A"},
            {5, "D"},
            {6, "A"},
            {7, "C"},
    };

    auto result = searchRecordsByName(records, "A");

    assert(std::get<0>(result).size() == 4);
    assert(std::get<1>(result) == "1346");

    assert(std::tuple_size<decltype(result)>::value == 2);

    typename std::tuple_element<0, decltype(result)>::type vec;
    assert(typeid(vec) == typeid(std::vector<Record>));
}

std::tuple<std::vector<Record>, std::string>
searchRecordsByName(const std::vector<Record> &records, const std::string &name) {
    std::vector<Record> results{};
    std::string resultsHash;

    for (const auto &r:records) {
        if (r.name == name) {
            results.push_back(r);
            resultsHash += std::to_string(r.id);
        }
    }

    return std::make_tuple(results, resultsHash);
}
