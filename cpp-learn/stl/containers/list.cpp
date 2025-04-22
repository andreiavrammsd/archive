/*
 The C++ Programming Language, 4th Edition
 */

#include <list>
#include <string>
#include <cassert>

struct Entry {
    std::string name;
    int number;

    bool operator<(const Entry &e) {
        return e.name < name;
    }

    bool operator==(const Entry &e) {
        return e.name == name and e.number == number;
    }
};

int get_number(const std::list<Entry> &phone_book, const std::string &name) {
    if (name.empty()) {
        return 0;
    }

    for (const auto &entry : phone_book) {
        if (entry.name == name) {
            return entry.number;
        }
    }

    return 0;
}

bool delete_number(std::list<Entry> &phone_book, const std::string &name) {
    if (name.empty()) {
        return false;
    }

    for (auto entry = phone_book.begin(); entry != phone_book.end(); ++entry) {
        if (entry->name == name) {
            phone_book.erase(entry);
            return true;
        }
    }

    return false;
}

int main() {
    std::list<Entry> phone_book = {
            {"David Hume",                      123456},
            {"Karl Popper",                     234567},
            {"Bertrand Arthur William Russell", 345678}
    };

    assert(phone_book.size() == 3);

    // Find
    assert(get_number(phone_book, "David Hume") == 123456);
    assert(get_number(phone_book, "") == 0);
    assert(get_number(phone_book, "John Doe") == 0);

    // Erase
    assert(!delete_number(phone_book, ""));
    assert(!delete_number(phone_book, "John Doe"));
    assert(delete_number(phone_book, "Bertrand Arthur William Russell"));

    assert(phone_book.size() == 2);

    // Merge
    auto phone_book2 = phone_book;
    phone_book.merge(phone_book2);
    assert(phone_book.size() == 4);
    assert(phone_book2.empty());

    // Pop
    assert(phone_book.front().name == "David Hume");
    phone_book.pop_front();
    assert(phone_book.front().name == "Karl Popper");

    // Remove
    phone_book.remove({"Karl Popper", 234567});
    assert(phone_book.front().name == "David Hume");

    // Remove if
    struct remove_by_number {
        int number;

        bool operator()(const Entry &entry) {
            return entry.number == number;
        }
    };
    phone_book.remove_if(remove_by_number{123456});
    assert(phone_book.empty());
}
