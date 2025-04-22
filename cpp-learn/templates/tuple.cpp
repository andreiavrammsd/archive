/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/utility/make_from_tuple
 */

#include <tuple>
#include <type_traits>
#include <ostream>
#include <iostream>
#include <sstream>
#include <cassert>

using std::enable_if;
using std::ostream;
using std::tuple;
using std::get;

// make_tuple implementation
template <typename... Types>
std::tuple<Types...> make_tuple(Types &&...t)
{
    return std::tuple<Types...>(t...);
}

template <typename T0, typename... T>
ostream &operator<<(ostream &os, const tuple<T0, T...> &t);

// print tuple to ostream
template <size_t N>  // print element N and following elements
struct print_tuple {
    template <typename... T>
    typename enable_if<(N < sizeof...(T))>::type static print(ostream &os, const tuple<T...> &t)
    {                                 // nonempty tuple
        os << ", " << get<N>(t);      // print an element
        print_tuple<N + 1>()(os, t);  // print the rest of the elements
    }

    template <typename... T>
    typename enable_if<N >= sizeof...(T)>::type static print(ostream &, const tuple<T...> &)
    {  // empty tuple
    }

    template <typename... T>
    print_tuple &operator()(ostream &, const tuple<T...> &)
    {
        return *this;
    }
};

std::ostream &operator<<(ostream &os, const tuple<> &)
{  // the empty tuple
    return os << "{}";
}

template <typename T0, typename... T>
ostream &operator<<(ostream &os, const tuple<T0, T...> &t)
{                                  // a nonempty tuple
    os << '{' << std::get<0>(t);   // print first element
    print_tuple<1>::print(os, t);  // print the rest of the elements
    return os << '}';
}

int main()
{
    std::stringstream out;

    out << make_tuple(1, "abc");
    assert(out.str() == "{1, abc}");
    out.str("");

    out << std::make_tuple();
    assert(out.str() == "{}");
    out.str("");

    out << std::make_tuple(std::make_tuple(std::make_tuple()));
    assert(out.str() == "{{{}}}");
    out.str("");

    out << make_tuple("One meatball!");
    assert(out.str() == "{One meatball!}");
    out.str("");

    out << std::make_tuple(2.0, std::make_tuple(2.1), nullptr);
    assert(out.str() == "{2, {2.1}}");
    out.str("");

    // make_from_tuple (C++17)
    struct Foo {
        int first;
        float second;
        int third;

        Foo(int first, float second, int third) : first{first}, second{second}, third{third} {}

        std::string operator()() const
        {
            std::ostringstream os;
            os << first << ", " << second << ", " << third;
            return os.str();
        }
    };
    auto tuple = std::make_tuple(42, 3.14f, 0);
    auto foo = std::make_from_tuple<Foo>(std::move(tuple));
    assert(foo() == "42, 3.14, 0");

    // deduction guides (C++17)
    int a[2], b[3], c[4];
    std::tuple t1{a, b, c};
    assert(std::get<1>(t1) == b);
}
