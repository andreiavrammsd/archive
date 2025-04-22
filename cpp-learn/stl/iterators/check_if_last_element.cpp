#include <sstream>
#include <vector>
#include <array>
#include <forward_list>
#include <cassert>

template<typename Iterator>
std::stringstream concat(const Iterator &v, char separator = ',') {
    std::stringstream s;

    for (auto i = std::cbegin(v); i != std::cend(v); ++i) {
        s << *i;

        // Check if next item is the last one
        if (std::next(i) != std::end(v)) {
            s << separator;
        }
    }

    return s;
}

int main() {
    assert(concat(std::vector<int>{1, 2, 3}).str() == "1,2,3");
    assert(concat(std::array<int, 3>{1, 2, 3}).str() == "1,2,3");
    assert(concat(std::forward_list<int>{1, 2, 3}).str() == "1,2,3");
}
