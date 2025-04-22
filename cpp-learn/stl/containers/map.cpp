/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <map>
#include <string>
#include <cassert>
#include <iostream>

int get_number(const std::map<std::string, int> &phone_book, const std::string &name) {
    if (name.empty()) {
        return 0;
    }

    auto entry = phone_book.find(name);
    return (entry != phone_book.end()) ? entry->second : 0;
}

bool delete_number(std::map<std::string, int> &phone_book, const std::string &name) {
    if (name.empty()) {
        return false;
    }

    auto i = phone_book.erase(name);
    return i > 0;
}

int main() {
    std::map<std::string, int> phone_book = {
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

    assert(phone_book.at("David Hume") == 123456);
    assert(phone_book["David Hume"] == 123456);

    try {
        phone_book.at("John Doe");
        assert(false);
    } catch (const std::out_of_range &e) {
        std::cout << e.what() << '\n';
    }
    assert(phone_book["John Doe"] == 0);

    // multimap
    std::multimap<int, int> multimap;
    multimap.emplace(0, 1);
    multimap.emplace(0, 1);
    multimap.emplace(0, 2);
    assert(multimap.size() == 3);

    /**
     * splicing (C++17)
     */
    std::map<int, std::string> src{
            {1, "one"},
            {2, "two"},
            {3, "buckle my shoe"},
    };
    std::map<int, std::string> dst{
            {3, "three"},
    };

    // Cheap remove and insert of { 1, "one" } from `src` to `dst`.
    dst.insert(src.extract(src.find(1)));

    // Cheap remove and insert of { 2, "two" } from `src` to `dst`.
    dst.insert(src.extract(2));

    assert(dst == (std::map<int, std::string>{{1, "one"},
                                              {2, "two"},
                                              {3, "three"}}));

    // Change key
    for (const auto &[k, v] : dst) {
        std::cout << k << ": " << v << '\n';
    }

    auto e = dst.extract(1);
    e.key() = 4;
    dst.insert(std::move(e));

    for (const auto &[k, v] : dst) {
        std::cout << k << ": " << v << '\n';
    }

    /**
     * try_emplace (C++17)
     */
    assert(dst.size() == 3);

    dst.try_emplace(5, "five");
    assert(dst.size() == 4);

    dst.try_emplace(5, "five");
    assert(dst.size() == 4);

    /**
     * insert_or_assign (C++17)
     */
    assert(dst[5] == "five");
    dst.insert_or_assign(5, "six");
    assert(dst[5] == "six");

    dst.insert_or_assign(dst.begin(), 5, "seven");
    assert(dst[5] == "seven");

    dst.try_emplace(5, "five");
    assert(dst[5] == "seven");
}
