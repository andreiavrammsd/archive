/*
 The C++ Programming Language, 4th Edition
 */

#include <type_traits>
#include <string>
#include <cassert>
#include <iostream>

// conditional
template <bool C, typename T, typename F>  // general template
struct conditional {
    using type = T;
};

template <typename T, typename F>
struct conditional<false, T, F> {
    using type = F;
};

struct X {
    std::string operator()() { return "polymorphic"; }
};

struct Y {
    std::string operator()() { return "nonpolymorphic"; }
};

class Parent {
    virtual void f();
};

class Child : Parent {
};

class Parent2 {
};

class Child2 : Parent2 {
};

template <typename T>
typename conditional<(std::is_polymorphic<T>::value), X, Y>::type z;

template <bool B, typename T, typename F>
using Conditional = typename std::conditional<B, T, F>::type;

struct Square {
    constexpr int operator()(int i) { return i * i; }
};

struct Cube {
    constexpr int operator()(int i) { return i * i * i; }
};

template <typename T>
struct My_cond {
    const size_t sz{};

    constexpr bool operator()() { return sizeof(T) > sz; }
};

//// selection
// class Nil {
// };
//
// template<int I, typename T1 = Nil, typename T2 = Nil, typename T3 = Nil, typename T4 = Nil>
// struct selection;
//
// template<int I, typename T1 = Nil, typename T2 = Nil, typename T3 = Nil, typename T4 = Nil>
// using Select = typename selection<I, T1, T2, T3, T4>::type;
//
//// selection: Specializations for 0-3
// template<typename T1, typename T2, typename T3, typename T4>
// struct selection<0, T1, T2, T3, T4> {
//     using type = T1;
// }; // specialize for N==0
//
// template<typename T1, typename T2, typename T3, typename T4>
// struct selection<1, T1, T2, T3, T4> {
//     using type = T2;
// }; // specialize for N==1
//
// template<typename T1, typename T2, typename T3, typename T4>
// struct selection<2, T1, T2, T3, T4> {
//     using type = T3;
// }; // specialize for N==2
//
// template<typename T1, typename T2, typename T3, typename T4>
// struct selection<3, T1, T2, T3, T4> {
//     using type = T4;
// }; // specialize for N==3
//
//// Select<5, int, double, char> x;
//
// template<int N, typename T1, typename T2, typename T3, typename T4>
// Select<N, T1, T2, T3, T4> get(std::tuple<T1, T2, T3, T4> &t);
//
////std::tuple<int> t;
////auto x = get<2>(std::tuple<int>(2));

template <unsigned N, typename... Cases>  // general case; never instantiated
struct selection;

template <unsigned N, typename T, typename... Cases>
struct selection<N, T, Cases...> : selection<N - 1, Cases...> {
};

template <typename T, typename... Cases>  // final case: N==0
struct selection<0, T, Cases...> {
    using type = T;
};

template <unsigned N, typename... Cases>
using Select = typename selection<N, Cases...>::type;

int main()
{
    // conditional
    assert(z<int>() == "nonpolymorphic");
    assert(z<Child>() == "polymorphic");
    assert(z<Child2>() == "nonpolymorphic");

    Conditional<std::is_polymorphic<int>::value, X, Y> zz1;
    assert(zz1() == "nonpolymorphic");

    Conditional<std::is_polymorphic<Child>::value, X, Y> zz2;
    assert(zz2() == "polymorphic");

    Conditional<My_cond<int>{2}(), Square, Cube> c1;
    static_assert(c1(2) == 4, "");

    Conditional<My_cond<int>{16}(), Square, Cube> c2;
    static_assert(c2(2) == 8, "");

    // selection (Selecting among Several Types)
    Select<0, long double, bool, int, char, long double> t1;
    assert(sizeof(t1) == sizeof(long double));

    Select<1, long double, bool, int, char, long double> t2;
    assert(sizeof(t2) == sizeof(bool));
}
