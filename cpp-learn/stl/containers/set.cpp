/*
 The C++ Programming Language, 4th Edition
 */

#include <set>
#include <unordered_set>
#include <cassert>
#include <iostream>
#include <functional>

struct Record {
    std::string name;
    int val{};
};

struct Nocase_hash {
    int d = 1;

    size_t operator()(const Record &r) const
    {
        size_t h = 0;

        for (auto x : r.name) {
            h <<= d;
            h ^= std::toupper(x);
        }

        return h;
    }
};

struct Nocase_equal {
    bool operator()(const Record &r, const Record &r2) const
    {
        if (r.name.size() != r2.name.size()) return false;

        for (size_t i = 0; i < r.name.size(); ++i) {
            if (std::toupper(r.name[i]) != std::toupper(r2.name[i])) return false;
        }

        return true;
    }
};

// [...] we may prefer to define the meaning of hash and equality once for all unordered containers
// of Record by specializing the standard-library hash and equal_to templates used by unordered_map
namespace std {
template <>
struct hash<Record> {
    size_t operator()(const Record &r) const { return std::hash<std::string>{}(r.name) ^ std::hash<int>{}(r.val); }
};

template <>
struct equal_to<Record> {
    bool operator()(const Record &r, const Record &r2) const { return r.name == r2.name && r.val == r2.val; }
};
}  // namespace std

int main()
{
    std::set<int> set;
    set.emplace(1);
    set.emplace(1);
    set.emplace(2);

    assert(set.size() == 2);
    for (const auto &i : set) {
        std::cout << i << '\n';
    }
    std::cout << '\n';

    std::set<int, std::less_equal<>> set2;
    set2.emplace(1);
    set2.emplace(1);
    set2.emplace(2);

    assert(set2.size() == 3);
    for (const auto &i : set2) {
        std::cout << i << '\n';
    }
    std::cout << '\n';

    // multiset
    std::multiset<int> mset;
    mset.emplace(1);
    mset.emplace(1);
    mset.emplace(2);

    assert(mset.size() == 3);
    for (const auto &i : mset) {
        std::cout << i << '\n';
    }
    std::cout << '\n';

    // unordered set
    std::unordered_set<Record, Nocase_hash, Nocase_equal> unordered_set{{
                                                                            {"andy", 7},
                                                                            {"al", 9},
                                                                            {"bill", -3},
                                                                            {"barbara", 12},
                                                                        },
                                                                        4,
                                                                        Nocase_hash{2},
                                                                        Nocase_equal{}};
    for (const auto &r : unordered_set) std::cout << "{" << r.name << ',' << r.val << "}\n";
    std::cout << '\n';

    auto hf = [](const Record &r) { return std::hash<std::string>()(r.name) ^ std::hash<int>()(r.val); };
    auto eq = [](const Record &r, const Record &r2) { return r.name == r2.name && r.val == r2.val; };

    std::unordered_set<Record,        // value type
                       decltype(hf),  // hash type
                       decltype(eq)   // hash type
                       >
        unordered_set2{{
                           {"andy", 7},
                           {"al", 9},
                           {"bill", -3},
                           {"barbara", 12},
                       },
                       4,
                       hf,
                       eq};
    for (const auto &r : unordered_set2) std::cout << "{" << r.name << ',' << r.val << "}\n";
    std::cout << '\n';

    std::unordered_set<Record> m1;
    m1.emplace(Record{"A", 1});
    m1.emplace(Record{"B", 2});
    m1.emplace(Record{"B", 2});
    for (const auto &r : m1) std::cout << "{" << r.name << ',' << r.val << "}\n";
    std::cout << '\n';

    // unordered multiset
    std::unordered_multiset<int> unordered_multiset;

    // splicing (C++17)
    std::set<int> numbers{1, 2, 3};
    assert(numbers.size() == 3);

    auto node = numbers.extract(2);
    assert(numbers.size() == 2);

    std::set<int> other_numbers;
    other_numbers.insert(node.value());
    assert(other_numbers.size() == 1);

    numbers.merge(other_numbers);
    assert(numbers.size() == 3);
}
