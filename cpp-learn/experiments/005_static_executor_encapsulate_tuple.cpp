#include <iostream>
#include <tuple>
#include <type_traits>

namespace input {

struct A {
    int a;
};

struct B {
    int b;
};

}  // namespace input

namespace operations {

void operate(input::A& a)
{
    a.a *= 10;
    std::cout << a.a << " - a\n";
}

void operate(input::B& b) { std::cout << b.b << " - b\n"; }

}  // namespace operations

namespace iterator {
template <typename T, std::size_t S = std::tuple_size<T>::value, std::size_t I = S - 1>
class Iterator {
   public:
    template <typename C>
    void operator()(T& objects, C&& callback)
    {
        callback(std::get<S - I - 1>(objects));
        Iterator<T, S, I - 1>{}(objects, std::forward<C>(callback));
    }
};

template <typename T, std::size_t S>
class Iterator<T, S, 0> {
   public:
    template <typename C>
    void operator()(T& objects, C&& callback)
    {
        callback(std::get<S - 1>(objects));
    }
};

template <typename... Ts>
class StaticIterator {
    static_assert(sizeof...(Ts) > 0, "at least one type is required");

   public:
    template <typename... Os>
    StaticIterator(Os&&... os) : objects{os...}
    {
        static_assert(std::is_same<std::tuple<Os...>, std::tuple<Ts...>>::value,
                      "argument list must match the types given to the iterator");
    }

    template <typename C>
    void operator()(C&& callback)
    {
        iterator(objects, std::forward<C>(callback));
    }

   private:
    using Objects = std::tuple<Ts...>;
    Objects objects;
    Iterator<Objects> iterator;
};

}  // namespace iterator

int main()
{
    auto a11 = input::A{0};
    auto a12 = input::A{1};
    auto b11 = input::B{};
    b11.b = 2;
    auto a13 = input::A{3};

    std::cout << "-> " << a12.a << "\n";

    iterator::StaticIterator<input::A&, input::A&, input::B&, input::A&> iterate{a11, a12, b11, a13};
    iterate([](auto& object) { operations::operate(object); });

    std::cout << "-> " << a12.a << "\n";
}

// https://github.com/andreiavrammsd/cpp-static-task-executor/blob/master/tests/executor_benchmark.cpp

// boost::variant < 17; std::variant 17+

// class ParameterBase {
// public:
//    virtual ~ParameterBase() = default;
//
//    template<class T>
//    const T& get() const; //to be implemented after Parameter
//    template<class T, class U>
//    void setValue(const U& rhs); //to be implemented after Parameter
//};
//
// template<typename T>
// class Parameter : public ParameterBase {
// public:
//    explicit Parameter(const T& rhs) : value(rhs) {}
//
//    const T& get() const { return value; }
//
//    void setValue(const T& rhs) { value = rhs; }
//
// private:
//    T value;
//};

////Here's the trick: dynamic_cast rather than virtual
// template<class T>
// const T& ParameterBase::get() const { return dynamic_cast<const Parameter<T>& >(*this).get(); }
//
// template<class T, class U>
// void ParameterBase::setValue(const U& rhs) { return dynamic_cast<Parameter<T>& >(*this).setValue(rhs); }

// main
// https://stackoverflow.com/questions/13980157/c-class-with-template-member-variable
//    std::array<ParameterBase *, 5> v;
//    Parameter<input::A> p1(a11);
//    v[0] =& p1;
//    Parameter<input::A> p2(b11);
//    v[1] =& p2;
//
//    for (auto& i : v) {
//        operations::operate(i->get<input::A>());
//    }