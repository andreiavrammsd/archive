/*
 The C++ Programming Language, 4th Edition
 */

#include <regex>
#include <cassert>

/**
 * Regular Expression Notation
 */

int main() {
    /**
     * Regular Expression Special Characters
     *
        .	Any single character (a "wildcard")
        [	Begin character class
        ]	End character class
        {	Begin count
        }	End count
        (	Begin grouping
        )	End grouping
        \	Next character has a special meaning
        *	Zero or more
        +	One or more
        ?	Optional (zero or one)
        |	Alternative (or)
        ^	Start of line; negation
        $	End of line
     */

    // [...] a line starting with zero or more As followed by one or more Bs
    // followed by an optional C [...]: ^A*B+C?$
    //
    // Examples that match:
    // AAAAAAAAAAAABBBBBBBBBC
    // BC
    // B
    //
    // Examples that do not match:
    // AAAAA // no B
    //  AAAABC // initial space
    // AABBCC // too many Cs

    /**
     * Repetition
     *
        {n}     Exactly n times
        {n,}    n or more times
        {n,m}   At least n and at most m times
        *       Zero or more, that is, {0,}
        +       One or more, that is, {1,}
        ?       Optional (zero or one), that is {0,1}
     */

    // A{3}B{2,4}C*
    //
    // Examples that match:
    // AAABBC
    // AAABBB
    //
    // Example that do not match:
    // AABBC // too few As
    // AAABC // too few Bs
    // AAABBBBBCCC
    // too many Bs

    /**
     * Character Classes
     *
        alnum	Any alphanumeric character
        alpha	Any alphabetic character
        blank	Any whitespace character that is not a line separator
        cntrl	Any control character
        d	    Any decimal digit
        digit	Any decimal digit
        graph	Any graphical character
        lower	Any lowercase character
        print	Any printable character
        punct	Any punctuation character
        s	    Any whitespace character
        space	Any whitespace character
        upper	Any uppercase character
        w	    Any word character (alphanumeric characters plus the underscore)
        xdigit	Any hexadecimal digit character
     */

    /**
     * Character Class Abbreviations
     *
        \d  A decimal digit                                     [[:digit:]]
        \s  A space (space, tab, etc.)                          [[:space:]]
        \w  A letter (a-z) or digit (0-9) or underscore (_)     [_[:alnum:]]
        \D  Not \d                                              [^[:digit:]]
        \S  Not \s                                              [^[:space:]]
        \W  Not \w                                              [^_[:alnum:]]
     */

    /**
     * Nonstandard (but Common) Character Class Abbreviations
     *
        \l  A lowercase character       [[:lower:]]
        \u  An uppercase character      [[:upper:]]
        \L  Not \l                      [^[:lower:]]
        \U  Not \u                      [^[:upper:]]
     */

    // For full portability, use the character class names rather than these abbreviations.

    // [:alpha:][:alnum:]* // wrong: characters from the set ":alph" followed by...
    // [[:alpha:]][[:alnum:]]* // wrong: doesn’t accept underscore ('_' is not alp
    // ([[:alpha:]]|_)[[:alnum:]]* // wrong: underscore is not part of alnum either
    // ([[:alpha:]]|_)([[:alnum:]]|_)* // OK, but clumsy
    // [[:alpha:]_][[:alnum:]_]* // OK: include the underscore in the character classes
    // [_[:alpha:]][_[:alnum:]]* // also OK
    // [_[:alpha:]]\w* // \w is equivalent to [_[:alnum:]]

    /**
     * Special Characters
     *
        \n          Newline
        \t          Tab
        \\          One backslash
        \x hh       Unicode characters expressed using two hexadecimal digits
        \u hhhh     Unicode characters expressed using four hexadecimal digits
        \b          The first or last character of a word (a "boundary character")
        \B          Not a \b
        \i          The i th sub_match in this pattern
     */

    // Using raw string literals alleviates many problems with special characters.
    std::string s = "abc0";
    std::regex pat{R"([_[:alpha:]]\w*)"};
    assert(std::regex_match(s, pat));

    // Examples:
    // Ax*              // A, Ax, Axxxx
    // Ax+              // Ax, Axxx                         Not A
    // \d-?\d           // 1-2, 12                          Not 1--2
    // \w{2}-\d{4,5}    // Ab-1234, XX-54321, 22-5432       Digits are in \w
    // (\d*:)?(\d+)     // 12:3, 1:23, 123, :123            Not 123:
    // (bs|BS)          // bs, BS                           Not bS
    // [aeiouy]         // a, o, u                          An English vow el, not x
    // [^aeiouy]        // x, k                             Not an English vow el, not e
    // [a^eiouy]        // a, ^, o, u                       An English vow el or ^

    // A group (a subpattern) potentially to be represented by a sub_match is delimited by parentheses.
    // If you need parentheses that should not define a subpattern, use (? rather than plain ( . For example:
    //
    // (\s|:|,)*(\d*) // spaces, colons, and/or commas followed by a number
    //
    // Assuming that we were not interested in the characters before the number (presumably separators),
    // we could write:
    //
    // (?\s|:|,)*(\d*) // spaces, colons, and/or commas followed by a number
    //
    // This would save the regular expression engine from having to store the first characters:
    // the (? variant has only one subpattern.


    /**
     * Regular Expression Grouping Examples
     *
        \d*\s\w+            No groups (subpatterns)
        (\d*)\s(\w+)        Two groups
        (\d*)(\s(\w+))+     Two groups (groups do not nest)
        (\s*\w*)+           One group, but one or more subpatterns; only the last subpattern is saved as a sub_match
        <(.*?)>(.*?)</\1>   Three groups; the \1 means "same as group 1"
     */

    // That last pattern is useful for parsing XML. It finds tag/end-of-tag markers. Note that I used a
    // non-greedy match (a lazy match), .*? , for the subpattern between the tag and the end tag. Had I
    // used plain .* , this input would have caused a problem:
    //
    // Always look for the <b>bright</b> side of <b>life</b>.
    //
    // A greedy match for the first subpattern would match the first < with the last > . A greedy match on
    // the second subpattern would match the first <b> with the last </b> . Both would be correct behavior,
    // but unlikely what the programmer wanted.
}
