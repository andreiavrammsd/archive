/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <iostream>
#include <regex>

bool match(const std::string &);

void search(const std::string &);

void replace(const std::string &);

int main()
{
    // Matching Options

    // match_not_bol		The character ^ is not considered to mean "beginning of line"
    // match_not_eol		The character $ is not considered to mean "end of line"
    // match_not_bow		\b does not match the subsequence [first, first)
    // match_not_eow		\b does not match the subsequence [last, last)
    // match_any		If more than one match is possible, then any match is acceptable
    // match_not_null		Do not match an empty sequence
    // match_continuous	Match only a subsequence that begins at first
    // match_prev_avail	--first is a valid iterator position

    /**
     * regex_match()
     *
     * To look for a pattern matching a whole sequence with a known length,
     * such as a line of text, use regex_match()
     */

    // regex_match(b,e ,m,pat,flags)
    // regex_match(b,e ,m,pat)
    // regex_match(b,e ,pat,flags)
    // regex_match(b,e ,pat)
    // regex_match(x,m,pat,flags)
    // regex_match(x,m,pat)
    // regex_match(x,pat,flags)
    // regex_match(x,pat)

    // A table is a series of rows, each with four tab-separated fields, except for
    // the first (title row) which may have only three fields.
    //
    // The numbers are supposed to add up horizontally and vertically.
    //
    // The program reads the title line and then does the sums for each line until it
    // reaches the final line labeled "Total"

    std::string table =
        "Class\tBoys\tGirls\tTotal\n"
        "1a\t12\t15\t27\n"
        "1b\t16\t14\t30\n"
        "Total\t28\t29\t57\n";

    assert(match(table));

    /**
     * regex_search()
     *
     * To look for a pattern in a part of a sequence,
     * such as a file, use regex_search()
     */

    // regex_search(b,e ,m,pat,flags)
    // regex_search(b,e ,m,pat)
    // regex_search(b,e ,pat,flags)
    // regex_search(b,e ,pat)
    // regex_search(x,m,pat,flags)
    // regex_search(x,m,pat)
    // regex_search(x,pat,flags)
    // regex_search(x,pat)

    std::string lines =
        "stroustrup\n"
        "strupstrup\n"
        "abc\n"
        "Strovstrup\n";

    search(lines);

    /**
     * regex_search()
     *
     * To make simple substitutions of a pattern in a part of a sequence,
     * such as a file, use regex_replace() :
     */

    // out=regex_replace(out,b,e ,pat,fmt,flags)
    // out=regex_replace(out,b,e ,pat,fmt)
    // s=regex_replace(x,pat,fmt,flags)
    // s=regex_replace(x,pat,fmt)

    std::string input{"x 1 y2 22 zaq 34567"};
    replace(input);
}

bool match(const std::string &table)
{
    std::stringstream in;
    in.str(table);

    std::string line;  // input buffer
    int lineno = 0;

    std::regex header{R"(^[\w ]+(\t[\w]+)*$)"};            // tab separated words
    std::regex row{R"(^([\w ]+)(\t\d+)(\t\d+)(\t\d+)$)"};  // label followed by three tab-separated numbers

    if (std::getline(in, line)) {  // check and discard the header line
        std::smatch matches;
        if (!std::regex_match(line, matches, header)) {
            std::cerr << "no header\n";
        }
    }

    int boys = 0;  // running totals
    int girls = 0;

    while (getline(in, line)) {
        ++lineno;

        std::smatch matches;  // submatches go here

        if (!std::regex_match(line, matches, row)) {
            std::cerr << "bad line: " << lineno << '\n';
            continue;
        }

        int curr_boy = stoi(matches[2]);
        int curr_girl = stoi(matches[3]);
        int curr_total = stoi(matches[4]);
        if (curr_boy + curr_girl != curr_total) {
            std::cerr << "bad row sum on line: " << lineno << '\n';
        }

        if (matches[1] == "Total") {  // last line
            if (curr_boy != boys) {
                std::cerr << "boys do not add up\n";
            }

            if (curr_girl != girls) {
                std::cerr << "girls do not add up\n";
            }

            return true;
        }

        boys += curr_boy;
        girls += curr_girl;
    }

    std::cerr << "didn't find total line\n";

    return false;
}

void search(const std::string &lines)
{
    std::stringstream in;
    in.str(lines);

    std::regex pat{"[Ss]tro?u?v?p?stra?o?u?p?b?"};

    std::smatch m;
    for (std::string s; in >> s;) {
        if (std::regex_search(s, m, pat)) {
            if (m[0] != "stroustrup" && m[0] != "Stroustrup") {
                std::cout << "Found: " << m[0] << '\n';
            }
        }
    }
}

void replace(const std::string &input)
{
    std::regex pat{R"((\w+)\s(\d+))"};  // word space number
    std::string format{"($1,$2)\n"};

    std::cout << std::regex_replace(input, pat, format) << "\n";

    // Note the annoying "spurious" spaces at the beginning of the lines. By default, regex_match()
    // copies unmatched characters to its output, so the two spaces that were not matched by pat are
    // printed.
    //
    // To eliminate those spaces, we can use the format_no_copy option (§37.2.2):

    std::cout << std::regex_replace(input, pat, format, std::regex_constants::format_no_copy);
}
