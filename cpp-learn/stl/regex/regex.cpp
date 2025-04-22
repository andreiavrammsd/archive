/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <regex>

int main()
{
    // Member Constants

    // The meaning of regular expression patterns is controlled by syntax_option_type
    // constants defined identically in regex_constants and regex:

    /**
       icase           No case is used when matching
       nosubs          No subexpression matches are stored in the match results
       optimize        Prefer fast matching to fast regular expression object construction
       collate         Character ranges of the form [a-b] are locale sensitive
       ECMAScript      The regular expression grammar is the one used by ECMAScript in ECMA-262 (with minor
       modifications; §iso.28.13) basic           The regular expression grammar is the one used by basic regular
       expressions in POSIX extended        The regular expression grammar is the one used by extended regular
       expressions in POSIX awk             The regular expression grammar is the one used by POSIX awk grep The regular
       expression grammar is the one used by POSIX grep egrep           The regular expression grammar is the one used
       by POSIX grep -E
    */

    // Constructors

    std::basic_regex<char> r0{};

    using namespace std::regex_constants;
    std::basic_regex<char> r1{"([a-z]+)", icase | ECMAScript | extended};

    std::basic_regex<char> r2{"([a-z]+)"};

    // Operations

    r2 = r1;
    r0 = move(r2);
    r0.assign(r1);
    // r=r.assign(x,flags)
    // r=r.assign(x)
    // r=r.assign(p,n,flags)
    // r=r.assign(b,e ,flags)
    // r=r.assign(b,e)
    // n=r.mark_count()
    // x=r.flags()
    // loc2=r.imbue(loc)
    // loc=r.g etloc()
    // r.swap(r2)

    /**
     * Match Results
     */

    // Operations

    // Results from a regular expression match are gathered in a match_results object which contains one
    // or more sub_match objects:

    // sub_match sm {};
    // n=sm.length()
    // s=sm
    // s=sm.str()
    // x=sm.compare(x)
    // x==y
    // x!=y
    // x<y
    // x>y
    // x<=y
    // x>=y
    // sm.matched

    // Matches and Submatches

    // match_results m {};
    // match_results m {a};
    // match_results m {m2};
    // m2=m
    // m2=move(m)
    // m. ̃match_results()
    // m.ready()
    // n=m.size()
    // n=m.max_size()
    // m.empty()
    // r=m[i]
    // n=m.length(i)
    // n=m.length()
    // pos=m.position(i)
    // pos=m.position()
    // s=m.str(i)
    // s=m.str()
    // sm=m.prefix()
    // sm=m.suffix()
    // p=m.begin()
    // p=m.end()
    // p=m.cbegin()
    // p=m.cend()
    // a=m.get_allocator()
    // m.swap(m2)
    // m==m2
    // m!=m2

    /**
     * Formatting
     *
     * In regex_replace() , formatting is done using a format() function:
     */

    // Formatting

    // out=m.format(out,b,e ,flags)
    // out=m.format(out,b,e)
    // out=m.format(out,fmt,flags)
    // out=m.format(out,fmt)
    // s=m.format(fmt,flags)
    // s=m.format(fmt)

    // Format Substitution Notation

    // $&	The match
    // $'	The prefix
    // $'	The suffix
    // $i	The ith submatch, e.g., $1
    // $ii	The iith submatch, e.g., $12
    // $$	Not a match, the $ character

    // Formatting Options (regex_constants::match_flag_type)

    // format_default		Use ECMAScript (ECMA-262) rules (§iso.28.13)
    // format_sed		    Use POSIX sed notation
    // format_no_copy		Copy only matches
    // format_first_only	Only the first occurrence of the regular expression is replaced
}
