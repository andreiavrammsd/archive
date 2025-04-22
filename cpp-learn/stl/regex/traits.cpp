/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <regex>
#include <cassert>

/**
 * A regex_traits<T> represents the correspondence between a character type, a string type, and a
 * locale as needed for a regex implementer:
 */

int main()
{
    // regex_traits tr {};
    // n=length(p)
    // c2=tr.translate(c)
    // c2=tr.translate_nocase(c)
    // s=tr.transform(b,e)
    // s=tr.transform_primary(b,e)
    // s=tr.lookup_collatename(b,e)
    // m=tr.lookup_classname(b,e,ign)
    // m=tr.lookup_classname(b,e)
    // tr.isctype(c,m)
    // i=tr.value(c,b)
    // loc2=tr.imbue(loc)
    // loc=tr.getloc()

    std::string input = "input";
    assert(std::regex_traits<char>::length(input.c_str()) == 5);

    // A transform is used to generate strings for fast comparisons in pattern-matching implementations.
    // A classification name is one of the character classifications listed in §37.1.1, such as alpha, s,
    // and xdigit .
}
