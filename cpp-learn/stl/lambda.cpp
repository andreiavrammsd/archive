/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP20.md#template-syntax-for-lambdas
 */

#include <algorithm>
#include <cassert>
#include <ostream>
#include <sstream>
#include <vector>

void print_modulo(const std::vector<int> &v, std::ostream &os, int m)
{  // output v[i] to os if v[i]%m==0
    std::for_each(std::begin(v), std::end(v), [&os, m](int x) {
        if (x % m == 0) {
            os << x << ' ';
        }
    });
}

int init() { return 0; }

struct MyObj {
    int value{123};

    auto getValueCopy()
    {
        return [*this] { return value; };
    }

    auto getValueRef()
    {
        return [this] { return value; };
    }
};

int main()
{
    const size_t size = 20;
    const int divisor = 3;
    std::vector<int> numbers(size);
    std::string expected = "3 6 9 12 15 18 ";

    auto generate = [] {
        static int i = 0;
        return ++i;
    };
    std::generate(numbers.begin(), numbers.end(), generate);

    std::stringstream result;
    print_modulo(numbers, result, divisor);
    assert(result.str() == expected);

    // initialized capture: i = 0 (C++14)
    auto generatemutable = [i = init()]() mutable { return ++i; };
    std::generate(std::begin(numbers), std::end(numbers), generatemutable);

    result.str("");
    print_modulo(numbers, result, divisor);
    assert(result.str() == expected);

    // Polymorphic lambdas (C++14)
    auto lmbd = [](auto x) { return x + x; };
    assert(lmbd(1) == 2);
    assert(lmbd(std::string{"a"}) == "aa");

    // constexpr lambda (C++17)
    auto identity = [](int n) constexpr { return n; };
    static_assert(identity(123) == 123);

    // Lambda capture this by value (C++17)
    MyObj mo;
    auto valueCopy = mo.getValueCopy();
    auto valueRef = mo.getValueRef();
    mo.value = 321;
    assert(valueCopy() == 123);
    assert(valueRef() == 321);

    // Template syntax for lambdas (C++20)
    auto twice = []<typename T>(T v) { return v * 2; };
    assert(twice(2) == 4);
    assert(twice(2.0) == 4.0);
}
