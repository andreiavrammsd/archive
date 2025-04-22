/*
 The C++ Programming Language, 4th Edition
 */

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>

template <typename T, size_t S>
void odd(const std::array<T, S> &v);

int main()
{
    std::array<double, 5> array{};

    std::generate(array.begin(), array.end(), [] {
        static double i = 1;
        return std::pow(i++, 2);
    });

    odd(array);

    static_assert(array.size() == 5, "not 5");

    assert(array[0] == 1);

    assert(array.front() == 1);

    assert(*(array.begin() + 1) == 4);

    static_assert(!array.empty(), "empty");

    assert(*(array.rend() - 1) == 1);

    assert(std::get<2>(array) == 9);

    assert(std::tuple_size<decltype(array)>() == 5);

    typename std::tuple_element<0, decltype(array)>::type i = 13;
    assert(typeid(i) == typeid(double));

    array.fill(1);
    assert(std::accumulate(array.begin(), array.end(), 0.0) == 5);
}

template <typename T, size_t S>
void odd(const std::array<T, S> &v)
{
    std::for_each(v.begin(), v.end(), [](T i) {
        if (std::fmod(i, 2) != 0) {
            std::cout << i << std::endl;
        }
    });
}
