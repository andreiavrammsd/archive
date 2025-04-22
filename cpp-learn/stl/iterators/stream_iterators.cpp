/*
 The C++ Programming Language, 4th Edition
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <string>
#include <vector>

bool unique_values_with_vector(const std::string &from, const std::string &to)
{
    std::ifstream is{from};                // input stream for file "from"
    is.exceptions(std::ifstream::badbit);  // throw exception if cannot read from file

    std::istream_iterator<std::string> ii{is};  // input iterator for stream
    std::istream_iterator<std::string> eos{};   // input sentinel

    std::ofstream os{to};                             // output stream for file "to"
    std::ostream_iterator<std::string> oo{os, "\n"};  // output iterator for stream

    std::vector<std::string> b{ii, eos};  // b is a vector initialized from input [ii:eos]
    std::sort(b.begin(), b.end());        // sort the buffer

    std::unique_copy(b.begin(), b.end(), oo);  // copy buffer to output, discard duplicated values_

    if (!is.eof()) {
        std::cout << "!is.eof()" << std::endl;
    }

    if (!os) {
        std::cout << "!os" << std::endl;
    }

    is.close();
    os.close();

    return !is.eof() || !os;  // return error state
}

bool unique_values_with_set(const std::string &from, const std::string &to)
{
    std::ifstream is{from};                // input stream for file "from"
    is.exceptions(std::ifstream::badbit);  // throw exception if cannot read from file

    std::ofstream os{to};  // output stream for file "to"

    // Set does not keep duplicate and keeps elements in order
    std::set<std::string> b{std::istream_iterator<std::string>{is},
                            std::istream_iterator<std::string>{}};                // read input
    std::copy(b.begin(), b.end(), std::ostream_iterator<std::string>{os, "\n"});  // copy to output

    is.close();
    os.close();

    return !is.eof() || !os;  // return error state
}

int main()
{
    std::string from, to_vector, to_set;
    std::cin >> from >> to_vector >>
        to_set;  // get source and targets file names (paths to input file and output files)
    // from must be a file with space or newline separated values_

    std::cout << from << std::endl;
    std::cout << to_vector << std::endl;
    std::cout << to_set << std::endl;

    auto result = unique_values_with_vector(from, to_vector);
    if (result) {
        return 1;
    }

    result = unique_values_with_set(from, to_set);
    if (result) {
        return 1;
    }
}
