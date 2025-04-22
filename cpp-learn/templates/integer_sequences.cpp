/*
 Since C++14
 https://en.cppreference.com/w/cpp/utility/integer_sequence
 */

#include <utility>

template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> seq) {
    static_assert(seq.size() == 10);
}

int main() {
    print_sequence(std::make_integer_sequence<int, 10>{});
}
