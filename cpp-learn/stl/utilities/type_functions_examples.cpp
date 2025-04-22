/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <array>
#include <cassert>
#include <typeinfo>

// conditional
constexpr int on_stack_max = sizeof(std::string);

template <typename T>
struct On_heap {
    On_heap() : p(new T) {}   // allocate
    ~On_heap() { delete p; }  // deallocate

    T &operator*() noexcept { return *p; }

    T *operator->() noexcept { return p; }

    On_heap(const On_heap &) = delete;  // prevent copying
    On_heap operator=(const On_heap &) = delete;

   private:
    T *p;  // pointer to object on the free store
};

template <typename T>
struct Scoped {
    T &operator*() noexcept { return x; }

    T *operator->() noexcept { return &x; }

   private:
    T x;  // the object
};

template <typename T>
struct Obj_holder {
    using type = typename std::conditional<(sizeof(T) <= on_stack_max),
                                           Scoped<T>,  // first alternative
                                           On_heap<T>  // second alternative
                                           >::type;
};

// multi-argument, multi-return-value type function
template <typename T, int N>
struct Array_type {
    using type = T;
    static const int dim = N;
};

// select
template <unsigned N, typename... Cases>
struct selection;

template <unsigned N, typename T, typename... Cases>
struct selection<N, T, Cases...> : selection<N - 1, Cases...> {
};

template <typename T, typename... Cases>
struct selection<0, T, Cases...> {
    using type = T;
};

template <unsigned N, typename... Cases>
using Select = typename selection<N, Cases...>::type;

template <int N>
struct Integer {
    using Error = void;
    using Type = Select<N, Error, signed char, short, Error, int, Error, Error, Error, long>;
};

int main()
{
    // conditional
    typename Obj_holder<double>::type v1;                   // the double goes on the stack
    typename Obj_holder<std::array<double, 200>>::type v2;  // the array goes on the free store

    *v1 = 7.7;
    assert(*v1 == 7.7);

    assert(v2->size() == 200);
    assert((*v2).size() == 200);

    // multi-argument, multi-return-value type function
    using Array = Array_type<int, 3>;

    Array::type x;

    assert(typeid(x) == typeid(int));
    constexpr int s = Array::dim;
    static_assert(s == 3, "dim not 3");

    // select
    typename Integer<4>::Type i4 = 8;
    static_assert(sizeof(i4) == 4, "not 4");

    typename Integer<1>::Type i1 = 1;
    static_assert(sizeof(i1) == 1, "not 1");
}
