/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <tuple>
#include <cassert>

struct Record {
    int id;
    std::string name;
};

struct Univ {
    Univ(std::string n, int r) : name{std::move(n)}, rank{r} {}

    std::string name;
    int rank;
    std::string city = "unknown";

    bool operator==(const Univ &u) const { return name == u.name && rank == u.rank && city == u.city; }
};

int main()
{
    std::pair<std::string, int> p{"Cambridge", 1209};
    assert(p.first == "Cambridge");
    p.second += 800;
    assert(p.second == 2009);

    // The piecewise_construct is the name of an object of type piecewise_construct_t used to distinguish
    // between constructing a pair with members of tuple types and constructing a pair using tuples as
    // argument lists for its first and second.
    using Tup = std::tuple<std::string, int>;
    Tup t1{"Columbia", 11};
    Tup t2{"Cambridge", 2};

    std::pair<Tup, Tup> p1{t1, t2};
    std::pair<Univ, Univ> p2{std::piecewise_construct, t1, t2};

    assert(p1.second == t2);
    assert(p2.second == Univ("Cambridge", 2));

    auto p3 = p1;
    assert(p3 == p1);
    assert(!(p3 < p1));

    using namespace std::string_literals;
    auto pair = std::make_pair("Harvard"s, 1736);
    assert(std::get<0>(pair) == "Harvard");

    std::vector<Record> records = {
        {1, "A"}, {2, "B"}, {3, "A"}, {4, "A"}, {5, "D"}, {6, "A"}, {7, "C"},
    };

    auto rec_eq = [](const Record &r1, const Record &r2) { return r1.name < r2.name; };

    auto er = std::equal_range(records.begin(), records.end(), Record{1, "A"}, rec_eq);

    for (auto it = er.first; it != er.second; ++it) {
        std::cout << (*it).id << ": " << (*it).name << std::endl;
    }
}
