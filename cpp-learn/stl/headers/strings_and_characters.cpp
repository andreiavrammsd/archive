/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <cctype>   // Character classification
#include <cstdlib>  // C-style allocation functions
#include <cstring>  // C-style string functions
#include <cuchar>   // C-style multibyte characters
#include <cwchar>   //  C-style wide-character string functions
#include <cwctype>  // Wide-character classification
#include <regex>    // Regular expression matching
#include <string>   // String of T
#include <array>

// C++17
#include <cassert>
#include <charconv>     // Primitive numeric conversions (to_chars and from_chars)
#include <string_view>  // A non-owning reference to a string

int main()
{
    // String of T
    static_assert(std::is_same<std::basic_string<char>::value_type, std::string::value_type>::value);

    std::string str = "A";
    str.append("B");
    str += 'C';
    assert(str == "ABC");

    // Character classification
    assert(isspace('\t'));
    assert(isalpha('c'));

    // Wide-character classification
    assert(iswspace('\t'));
    assert(iswalpha('c'));

    // C-style string functions
    const char *cstr = "abc";
    char cstr2[4];
    assert(strlen(cstr) == 3);
    strcpy(cstr2, cstr);
    assert(strcmp(cstr, cstr2) == 0);

    // C-style wide-character string functions
    wchar_t wstr[] = L"ABC";
    assert(wcsnlen(wstr, 16) == 3);

    // C-style allocation functions
    int *array = static_cast<int *>(malloc(sizeof(int) * 2));
    array[0] = 1;
    array[1] = 2;
    assert(array[0] + array[1] == 3);
    free(array);

    // C-style multibyte characters
    // https://en.cppreference.com/w/cpp/string/multibyte/mbrtoc16
    const std::string mbstr{reinterpret_cast<const char*>(u8"This is a UTF-8 string")};
    char16_t c16;
    const char *mbptr = &mbstr[0], *mbend = &str[0] + mbstr.size();
    std::mbstate_t state{};

    std::size_t rc = mbrtoc16(&c16, mbptr, mbend - mbptr + 1, &state);
    assert(rc == 1);
    assert(c16 == 122);
    assert(c16 == 'z');

    // Regular expression matching
    std::string words = "tox, the quick brown box";

    std::regex words_count_pattern{R"(\w+)"};
    auto beg = std::sregex_iterator(words.begin(), words.end(), words_count_pattern);
    auto end = std::sregex_iterator{};
    auto count = std::distance(beg, end);
    assert(count == 5);

    std::regex replace_pattern{R"(.ox)"};
    words = std::regex_replace(words, replace_pattern, "fox");
    assert(words == "fox, the quick brown fox");

    // A non-owning reference to a string.
    // Useful for providing an abstraction on top of strings (e.g. for parsing).
    std::string string{"   me"};
    std::string_view view{string};

    assert(view.size() == string.size());
    view.remove_prefix(std::min(view.find_first_not_of(" "), view.size()));
    assert(string == "   me");
    assert(view == "me");

    string = view;
    assert(string == "me");

    assert(view.compare("mez") == -1);

    using namespace std::literals::string_view_literals;
    assert(string == "me"sv);

    // Primitive numeric conversions (to_chars and from_chars)
    std::array<char, 10> chars{"42"};
    int result;
    auto [p, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), result);
    assert(ec == std::errc{});
    assert(result == 42);

    std::array<char, 10> num_string{};
    auto [p2, ec2] = std::to_chars(num_string.data(), num_string.data() + num_string.size(), 42);
    assert(ec2 == std::errc{});
    assert(std::string_view(num_string.data(), p2 - num_string.data()) == "42"sv);
}
