/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <iterator>
#include <regex>
#include <string>

/**
 * The regex_search() function allows us to find a single occurrence of a pattern in a data stream.
 *
 * What if we wanted to find and do something to all such occurrences? If the data is organized as a
 * sequence of easily recognized lines or records, we can iterate over those and use regex_match() for
 * each.
 *
 * If what we want to do with each occurrence of a pattern is a simple substitution, we can use regex_replace().
 *
 * If we want to iterate over a sequence of characters doing something for each occurrence of a pattern,
 * we use a regex_iterator.
 */

int main()
{
    /**
     * regex_iterator
     *
     * A regex_iterator is a bidirectional iterator that searches a sequence for the next match of a pattern
     * when incremented
     *
     * A regex_iterator provides a minimal set of iterator operations:
     */

    // regex_iterator p {};
    // regex_iterator p {b,e,pat,flags);
    // regex_iterator p {b,e,pat);
    // regex_iterator p {q};
    // p=q
    // p==q
    // p!=q
    // c=*p
    // x=p->m
    // ++p
    // q=p++

    std::string input = "aa as; asd ++e^asdf asdfg";

    std::regex pat{R"(\s+(\w+))"};

    for (std::sregex_iterator p(input.begin(), input.end(), pat); p != std::sregex_iterator{}; ++p) {
        std::cout << (*p)[1] << '\n';
    }
    // Note that we are missing the first word, aa, because it has no preceding whitespace.

    // If we simplify the pattern to R"((\ew+))", we get
    pat = R"((\w+))";

    std::cout << '\n';
    for (std::sregex_iterator p(input.begin(), input.end(), pat); p != std::sregex_iterator{}; ++p) {
        std::cout << (*p)[1] << '\n';
    }

    // You cannot write through a regex_iterator and regex_iterator{} is the only possible end-of-sequence.

    /**
     * regex_token_iterator
     *
     * A regex_token_iterator is an adaptor for regex_iterator that iterates over sub_matches of the
     * match_results found:
     */

    // regex_token_iterator p {};
    // regex_token_iterator p {b,e,pat,x,flags};
    // regex_token_iterator p {b,e,pat,x};
    // regex_token_iterator p {b,e,pat};
    // regex_token_iterator p {q};
    // p.~regex_token_iterator()
    // p=q
    // p==q
    // p!=q
    // c=*p
    // x=p->m
    // ++p
    // q=p++

    input = "aa::bb cc::dd ee::ff";
    pat = R"((\w)+([[:punct:]]+)(\w+)\s*)";

    std::cout << '\n';
    std::sregex_token_iterator end{};
    for (std::sregex_token_iterator p{input.begin(), input.end(), pat, {1, 3}}; p != end; ++p) {
        std::cout << *p << '\n';
    }

    // The -1 option basically inverts the strategy for reporting matches by representing each character
    // sequence that does not match as a sub_match. This is often referred to as token splitting (that is,
    // splitting a character stream into tokens) because when your pattern matches the token separators,
    // option -1 leaves you with the tokens.

    input = "1,2 , 3 ,4,5, 6 7";
    pat = R"(\s*,\s*)";  // use comma as token separator

    std::cout << '\n';
    std::copy(std::sregex_token_iterator{input.begin(), input.end(), pat, -1}, std::sregex_token_iterator{},
              std::ostream_iterator<std::string>{std::cout, "\n"});

    // This could equivalently be written using an explicit loop:
    std::cout << '\n';
    for (std::sregex_token_iterator p{input.begin(), input.end(), pat, -1}; p != std::sregex_token_iterator{}; ++p) {
        std::cout << *p << '\n';
    }
}
