#include <array>
#include <iostream>
#include <tuple>

#if (__cplusplus >= 201402L || (defined(_MSVC_LANG) && _MSVC_LANG >= 201402L))
#define __CPP_GTE_14__
#endif

namespace input {
struct A {
    int a;
};

struct B : A {
    int b;
};
}  // namespace input

namespace operations {

#if defined(__CPP_GTE_14__)

template <typename T, typename T2>
using op = std::enable_if_t<std::is_same<T, T2>::value, void>;

template <typename T>
op<T, input::A> operate(T &a)
{
    a.a *= 10;
    std::cout << a.a << " - a\n";
}

template <typename T>
op<T, input::B> operate(T &b)
{
    std::cout << b.b << " - b\n";
}

#else

template <typename T, typename T2>
using op = typename std::enable_if<std::is_same<T, T2>::value, void>::type;

struct operations {
    template <typename T>
    op<T, input::A> operator()(T &a)
    {
        a.a *= 10;
        std::cout << a.a << " - a\n";
    }

    template <typename T>
    op<T, input::B> operator()(T &b)
    {
        std::cout << b.b << " - b\n";
    }
};

#endif

}  // namespace operations

namespace iterator {
template <typename T, std::size_t S = std::tuple_size<T>::value, std::size_t I = S - 1>
struct Iterator {
    template <typename C>
    void operator()(T &objects, C callback)
    {
        callback(std::get<S - I - 1>(objects));
        Iterator<T, S, I - 1>{}(objects, callback);
    }
};

template <typename T, std::size_t S>
struct Iterator<T, S, 0> {
    template <typename C>
    void operator()(T &objects, C callback)
    {
        callback(std::get<S - 1>(objects));
    }
};

template <typename... Ts>
struct StaticIterator {
    using Tasks = std::tuple<Ts...>;
    Iterator<Tasks> iterator;
    //        Tasks objects{};

    //        StaticIterator() = default;

    //        StaticIterator(Tasks &objects) : objects{objects} {}
};
}  // namespace iterator

int main()
{
    using Objects = std::tuple<input::A &, input::A &, input::B &, input::A &>;

    iterator::Iterator<Objects> iterate;

    auto a11 = input::A{0};
    auto a12 = input::A{1};
    std::cout << "-> " << a12.a << "\n";
    auto b11 = input::B{};
    b11.b = 2;
    auto a13 = input::A{3};
    Objects objects{a11, a12, b11, a13};

#if defined(__CPP_GTE_14__)
    iterate(objects, [](auto &object) { operations::operate(object); });
#else
    iterate(objects, operations::operations{});
#endif

    std::cout << "-> " << a12.a << "\n";

    iterator::StaticIterator<input::A &, input::A &, input::B &, input::A &> iterate_wrapper{};
    iterate_wrapper.iterator(objects, [](auto &object) { operations::operate(object); });
}

// class ParameterBase {
// public:
//     virtual ~ParameterBase() = default;
//
//     template<class T>
//     const T &get() const; //to be implemented after Parameter
//     template<class T, class U>
//     void setValue(const U &rhs); //to be implemented after Parameter
// };
//
// template<typename T>
// class Parameter : public ParameterBase {
// public:
//     explicit Parameter(const T &rhs) : value(rhs) {}
//
//     const T &get() const { return value; }
//
//     void setValue(const T &rhs) { value = rhs; }
//
// private:
//     T value;
// };

////Here's the trick: dynamic_cast rather than virtual
// template<class T>
// const T &ParameterBase::get() const { return dynamic_cast<const Parameter<T> &>(*this).get(); }
//
// template<class T, class U>
// void ParameterBase::setValue(const U &rhs) { return dynamic_cast<Parameter<T> &>(*this).setValue(rhs); }

// main
// https://stackoverflow.com/questions/13980157/c-class-with-template-member-variable
//    std::array<ParameterBase *, 5> v;
//    Parameter<input::A> p1(a11);
//    v[0] = &p1;
//    Parameter<input::A> p2(b11);
//    v[1] = &p2;
//
//    for (auto &i : v) {
//        operations::operate(i->get<input::A>());
//    }