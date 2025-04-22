#include <type_traits>

template<typename T>
constexpr auto twice(T &&param) { // *
    using argument_type = typename std::decay<T>::type; // *
    static_assert(std::is_same<argument_type, int>::value);
    return param + param;
}

int main() {
    static_assert(twice(2) == 4);

    const int val = 2; // *
    static_assert(twice(val) == 4);
}
