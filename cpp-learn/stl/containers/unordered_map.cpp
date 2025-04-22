/*
 The C++ Programming Language, 4th Edition
 */

#include <unordered_map>
#include <string>
#include <cassert>
#include <iostream>

int get_number(const std::unordered_map<std::string, int> &phone_book, const std::string &name) {
    if (name.empty()) {
        return 0;
    }

    auto entry = phone_book.find(name);
    return (entry != phone_book.end()) ? entry->second : 0;
}

bool delete_number(std::unordered_map<std::string, int> &phone_book, const std::string &name) {
    if (name.empty()) {
        return false;
    }

    auto i = phone_book.erase(name);
    return i > 0;
}

// [...] we may prefer to define the meaning of hash and equality once for all unordered containers
// of Record by specializing the standard-library hash and equal_to templates used by unordered_map
struct Record {
    std::string name;
    int val{};
};

namespace std {
    template<>
    struct hash<Record> {
        size_t operator()(const Record &r) const {
            return std::hash<std::string>{}(r.name) ^ std::hash<int>{}(r.val);
        }
    };

    template<>
    struct equal_to<Record> {
        bool operator()(const Record &r, const Record &r2) const {
            return r.name == r2.name && r.val == r2.val;
        }
    };
}

int main() {
    std::unordered_map<std::string, int> phone_book = {
            {"David Hume",                      123456},
            {"Karl Popper",                     234567},
            {"Bertrand Arthur William Russell", 345678}
    };

    assert(phone_book.size() == 3);

    assert(get_number(phone_book, "David Hume") == 123456);
    assert(get_number(phone_book, "") == 0);
    assert(get_number(phone_book, "John Doe") == 0);

    assert(!delete_number(phone_book, ""));
    assert(!delete_number(phone_book, "John Doe"));
    assert(delete_number(phone_book, "Bertrand Arthur William Russell"));

    assert(phone_book.size() == 2);

    phone_book.emplace("A", 1);
    for (const auto &e : phone_book) {
        std::cout << e.first << ": " << e.second << '\n';
    }
    std::cout << '\n';

    phone_book.clear();
    assert(phone_book.empty());

    // multimap
    std::unordered_multimap<std::string, int> multimap;
    multimap.emplace("A", 1);
    multimap.emplace("A", 2);
    multimap.emplace("B", 1);
    multimap.emplace("B", 2);
    multimap.emplace_hint(multimap.begin(), "B", 3);
    for (const auto &e : multimap) {
        std::cout << e.first << ": " << e.second << '\n';
    }
    std::cout << '\n';

    std::cout << multimap.load_factor() << "\n\n";

    // custom hash function
    std::unordered_map<Record, int> records;
    records.emplace(Record{"A", 1}, 1);
    records.emplace(Record{"B", 1}, 2);

    for (const auto &r : records) {
        std::cout << '{' << r.first.name << ", " << r.first.val << "}: " << r.second << '\n';
    }
    std::cout << '\n';

    records.rehash(100);
    records.reserve(100);

    for (const auto &r : records) {
        std::cout << '{' << r.first.name << ", " << r.first.val << "}: " << r.second << '\n';
    }
    std::cout << '\n';

    std::cout << records.bucket_count() << std::endl;
    std::cout << records.bucket_size(1) << std::endl;
}
