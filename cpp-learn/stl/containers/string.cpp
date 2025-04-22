/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <cassert>
#include <iostream>
#include <cstring>
#include <algorithm>

size_t f(const char *str) {
    return strlen(str);
}

int main() {
    std::string str = "the quick brown fox";

    assert(str.size() == 19);
    assert(str[1] == 'h');
    assert(str.at(1) == 'h');

    try {
        (void)str.at(100);
    } catch (const std::out_of_range &e) {
        std::cout << e.what() << '\n';
    }

    str.clear();
    assert(str.empty());

    str = "string";
    assert(str == "string");

    assert(f(str.c_str()) == str.size());

    str.replace(str.begin(), str.begin() + 2, "ST!");
    assert(str == "ST!ring");

    assert(str.front() == 'S');

    str.pop_back();
    assert(str == "ST!rin");

    std::cout << str << std::endl;

    for (const auto &c : str) {
        std::cout << c;
    }
    std::cout << std::endl;

    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    assert(str == "ST!RIN");

    std::transform(str.begin(), str.end(), str.begin(), [](const char c) {
        return c + 3;
    });
    assert(str == "VW$ULQ");

    str += "W";
    assert(str.find('W') == 1);
    assert(str.find_first_of('W', 2) == 6);
    assert(str.find("UL") == 3);

    // Basic string
    std::basic_string<char> bs = "VW$ULQW";
    assert(str == bs);

    // Wide string: basic_string<wchar_t> (u16string: basic_string<char16_t>, u32string: basic_string<char32_t>)
    std::wstring ws{'a'};
    ws.push_back('b');

    std::wstring ws2{'a'};
    ws2.push_back('b');
    ws2.push_back('c');

    assert(ws.compare(ws2) == -1);
}
