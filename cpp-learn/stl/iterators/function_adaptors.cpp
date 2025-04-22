/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

/**
 * A function adaptor takes a function as argument and returns a function object that can be used to
 * invoke the original function.
 */

double cube(double d);

void incr(int &i);

class Shape {
   public:
    explicit Shape(int i);

    void draw();

   private:
    int i;
};

int myround(double x);

enum class Round_style { truncate, round };

struct Round {  // function object carrying a state
    Round_style s;

    Round(Round_style ss);

    int operator()(double x) const;
};

template <typename Callable>
class Proxy {
    Callable c;

   public:
    explicit Proxy(Callable c) : c{c} {}

    template <class... Args>
    decltype(auto) operator()(Args &&...args)
    {
        // ...
        return std::invoke(c, std::forward<Args>(args)...);
    }
};

int main()
{
    /**
     * bind
     *
     * Given a function and a set of arguments, bind() produces a function object that can be called with
     * "the remaining" arguments, if any, of the function.
     */

    auto cube2 = std::bind(cube, 2);
    assert(cube2() == 8);

    int i = 1;
    incr(i);
    assert(i == 2);

    using namespace std::placeholders;
    auto inc = std::bind(incr, _1);
    inc(i);
    assert(i == 3);

    // ref/cref: reference_wrapper<T>
    auto i2 = std::ref(i);
    ++i2;
    assert(i == 4);
    assert(i2 == 4);

    /**
     * mem_fm
     */
    std::unique_ptr<Shape>(new Shape(1))->draw();

    auto draw = std::mem_fn(&Shape::draw);
    draw(std::unique_ptr<Shape>(new Shape(2)));

    std::vector<std::shared_ptr<Shape>> vs{
        std::shared_ptr<Shape>(new Shape(3)),
        std::shared_ptr<Shape>(new Shape(4)),
        std::shared_ptr<Shape>(new Shape(5)),
    };
    for_each(vs.begin(), vs.end(), std::mem_fn(&Shape::draw));

    /**
     * function
     */
    std::function<double(double)> func;
    // vs
    // using func_type = double (*)(double);
    // func_type func_;

    func = [](double x) { return x * 2; };
    assert(func(2) == 4);

    func = cube;
    assert(func(2) == 8);

    std::function<double(double)> func2;

    func2.swap(func);
    assert(func2(2) == 8);
    assert(func == nullptr);

    // Function object
    std::function<int(double)> f;  // f can hold anything that can be called with a double and return an int

    f = myround;
    assert(f(7.6) == 8);  // call through f to the function myround

    f = Round(Round_style::truncate);
    assert(f(7.6) == 7);  // call the function object

    f = Round(Round_style::round);
    assert(f(7.6) == 8);

    Round_style style = Round_style::round;
    f = [style](double x) { return (style == Round_style::round) ? x + 0.5 : x; };
    assert(f(7.6) == 8);  // call the lambda

    std::vector<double> v{7.6};
    f = Round(Round_style::round);
    std::transform(v.begin(), v.end(), v.begin(), f);  // pass to algorithm
    assert(v[0] == 8);

    /**
     * invoke (C++17)
     *
     * Invoke a Callable object with parameters.
     */
    static_assert(std::is_invocable<int()>::value);
    static_assert(std::is_invocable_r<int, int()>::value);

    auto callable = [](int i) { return i * 2; };
    auto res = std::invoke(callable, 1);
    assert(res == 2);

    auto add = [](int x, int y) { return x + y; };

    Proxy<decltype(add)> proxy1{add};
    assert(proxy1(1, 2) == 3);

    Proxy proxy2{add};
    assert(proxy2(1, 2) == 3);

    /**
     * apply (C++17)
     *
     * Invoke a Callable object with a tuple of arguments.
     */
    assert(std::apply(add, std::make_tuple(1, 2)) == 3);
}

double cube(double d) { return d * d * d; }

void incr(int &i) { ++i; }

Shape::Shape(int i) : i{i} {}

void Shape::draw() { std::cout << "draw: " << i << "\n"; }

int myround(double x) { return static_cast<double>(std::floor(x + 0.5)); }

Round::Round(Round_style ss) : s(ss) {}

int Round::operator()(double x) const { return (s == Round_style::round) ? (x + 0.5) : x; }