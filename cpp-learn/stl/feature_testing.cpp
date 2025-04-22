/*
 https://devdocs.io/cpp/feature_test
 */

#include <iostream>

#ifdef __has_include    // Check if __has_include is present
#   if __has_include(<optional>)    // Check for a standard library

#       include <optional>

#       define BRANCH 1
#   elif __has_include(<experimental/optional>)     // Check for an experimental version
#       include<experimental/optional>
#       define BRANCH 2
#   elif __has_include(<boost/optional.hpp>)      // Try with an external librar
#       include<boost/optional.hpp>
#       define BRANCH 3
#   else        // Not found at all
#        error "Missing <optional>"
#   endif
#endif

int main() {
    constexpr std::optional<bool> b;
    static_assert(!b.has_value());

    std::cout << BRANCH << '\n';
}
