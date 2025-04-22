/*
 The C++ Programming Language, 4th Edition
 */

#include <cctype>
#include <cassert>
#include <string>
#include <iostream>

int main() {
    /**
     * Classification Functions
     *
     * In <cctype>, the standard library provides functions to classify the characters
     * from the basic execution character set
     */

    // Character Classification

    assert(isspace('\t')); // Is c whitespace (space ' ', horizontal tab '\t', newline '\n',
    // vertical tab '\v', form feed '\f', carriage return '\r' )?
    assert(isalpha('c')); // Is c a letter ( 'a'..'z', 'A'..'Z' )? note: not underscore '_'
    assert(isdigit('1')); // Is c a decimal digit ( '0'..'9' )?
    assert(isxdigit('F')); // Is c hexadecimal digit (decimal digit or 'a'..'f' or 'A'..'F' )?
    assert(isupper('A')); // Is c an uppercase letter?
    assert(islower('a')); // Is c a lowercase letter?
    assert(isalnum('a')); // isalpha(c) or isdigit(c)
    assert(iscntrl(127)); // Is c a control character (ASCII 0..31 and 127)?
    assert(ispunct(',')); // Is c not a letter, digit, whitespace, or invisible control character?
    assert(isprint(',')); // Is c printable (ASCII " . ' ~ )?
    assert(isgraph('3')); // isalpha(c) or isdigit(c) or ispunct(c) ? note: not space

    // Upper- and Lowercase

    assert(toupper('c') == 'C');
    assert(tolower('C') == 'c');

    /**
     * Character Traits
     *
     * The standard provides four specializations of char_traits (§iso.21.2.3):
     *
     * template<> struct char_traits<char>;
     * template<> struct char_traits<char16_t>;
     * template<> struct char_traits<char32_t>;
     * template<> struct char_traits<wchar_t>;
     */

    const auto toc = std::char_traits<char>::to_char_type(65);
    static_assert(toc == 'A');
    std::cout << toc << '\n';

    const auto toi = std::char_traits<char>::to_int_type('A');
    static_assert(toi == 65);
    std::cout << toi << '\n';

    // Comparing with eq() is often not simply an ==. For example, a case-insensitive char_traits would
    // define its eq() so that eq('b','B') would return true.
    static_assert(std::char_traits<char>::eq(65, 'A'));

    static_assert(std::char_traits<char>::lt('A', 'B'));

    static_assert(std::char_traits<char>::compare("Anna", "Anma", 1) == 0);
    static_assert(std::char_traits<char>::compare("Anna", "Anma", 2) == 0);
    static_assert(std::char_traits<char>::compare("Anna", "Anma", 3) > 0);

    // assign
    // move
    // copy

    static_assert(std::char_traits<char>::length("abc") == 3);

    // find

    static_assert(std::char_traits<char>::eof());

    static_assert(std::char_traits<char>::not_eof(1));
}
