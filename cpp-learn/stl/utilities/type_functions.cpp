/*
 The C++ Programming Language, 4th Edition
 */

#include <type_traits>
#include <cstring>
#include <memory>
#include <iostream>
#include <cassert>
#include <array>

/**
 * In <type_traits>, the standard library provides type functions (§28.2) to determine properties of
 * types (type traits; §35.4.1) and to generate new types from existing ones (type generators; §35.4.2).
 * These type functions are primarily used at compile time to support simple, and not so simple,
 * metaprogramming.
 */

class X {
   public:
    int i = 0;

    virtual void m() {}
};

template <typename T>
void f(T &)
{
    static_assert(std::is_floating_point<T>::value, "FP type expected");
}

template <typename T>
constexpr bool Is_floating_point()
{
    return std::is_floating_point<T>::value;
}

template <typename T>
void f2(T &)
{
    static_assert(Is_floating_point<T>(), "FP type expected");
}

template <typename Scalar>
class complex {
    Scalar re{}, im{};

   public:
    complex() = default;

    static_assert(std::is_arithmetic<Scalar>(), "Sorry, I only support complex of arithmetic types");

    virtual ~complex() = default;
};

class string : public std::string {
};

template <typename T>
class Cont {
    size_t sz{};
    T *elem{};

   public:
    explicit Cont(size_t sz) : sz{sz}, elem(new T[sz]) {}

    Cont(const Cont &a)  // copy constructor
        : sz(a.sz), elem(new T[a.sz])
    {
        static_assert(std::is_copy_constructible<T>::value, "Cont::Cont(): no copy");

        if (std::is_trivially_copyable<T>::value) {
            std::cout << "first here\n";

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdynamic-class-memaccess"
#elif __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#endif

            memcpy(elem, a.elem, sz * sizeof(T));  // memcopy optimization

#ifdef __clang__
#pragma clang diagnostic pop
#elif __GNUC__
#pragma GCC diagnostic pop
#endif
        }
        else {
            std::cout << "second here\n";
            std::uninitialized_copy(a.elem, a.elem + a.sz, elem);  // use copy constructors
        }
    }

    T &operator[](size_t i) { return elem[i]; }
};

struct Shape {
    virtual void draw() {}
};

template <typename T>
void draw(T t)
{
    static_assert(std::is_same<Shape *, T>() || std::is_base_of<Shape, std::remove_pointer<T>>(), "");
    t->draw();
}

template <typename K, typename V>
class My_map {
   public:
    std::pair<typename std::add_const<K>, V> default_node;
};

template <typename T>
void ref(T &&v)
{
    typename std::remove_reference<T>::type x = v;  // copy of v
    T y = v;                                        // maybe copy of v; maybe a reference to x
    ++x;                                            // increment local variable
    ++y;

    // x really is a copy of v, but if T is a reference type, y is a reference to v
}

template <typename T, typename U>
using Common_type = typename std::common_type<T, U>::type;

struct S {
    double operator()(char, int &);

    float operator()(int) { return 1.0; }
};

int main()
{
    /**
     * Type traits
     */

    // Primary Type Predicates

    static_assert(std::is_void<void>::value, "");

    static_assert(std::is_integral<int>::value, "");
    static_assert(!std::is_integral<X>::value, "");

    static_assert(std::is_floating_point<double>::value, "");

    static_assert(std::is_array<int[]>::value, "");

    static_assert(std::is_pointer<X *>::value, "");

    // is_lvalue_reference<X>
    // is_rvalue_reference<X>
    // is_member_object_pointer<X>
    // is_member_function_pointer<X>
    // is_enum<X>
    // is_union<X>
    // is_class<X>
    // is_function<X>

    assert(std::is_class<complex<int>>());

    // int i = 1;
    // f(i); // error: FP type expected
    // f2(i); // error: FP type expected

    double d = 1;
    f(d);   // OK
    f2(d);  // OK

    // Composite Type Predicates

    // is_reference<X>
    // is_arithmetic<X>
    // is_fundamental<X>
    // is_object<X>
    // is_scalar<X>
    // is_compound<X>
    // is_member_pointer<X>

    complex<int> c{};
    // complex<std::string> s{};

    // Type property predicates

    // is_const<X>
    // is_volatile<X>
    // is_trivial<X>
    // is_trivially_copyable<X>
    // is_standard_layout<X>
    // is_pod<X>
    // is_literal_type<X>
    // is_empty<X>
    // is_polymorphic<X>
    // is_abstract<X>
    // is_signed<X>
    // is_unsigned<X>
    // is_constructible<X,args>
    // is_default_constructible<X>
    // is_copy_constructible<X>
    // is_move_constructible<X>
    // is_assignable<X,Y>
    // is_copy_assignable<X>
    // is_move_assignable<X>
    // is_destructible<X>
    // is_trivially_constructible<X,args>
    // is_trivially_default_constructible<X>
    // is_trivially_copy_constructible<X>
    // is_trivially_move_constructible<X>
    // is_trivially_assignable<X,Y>
    // is_trivially_copy_assignable<X>
    // is_trivially_move_assignable<X>
    // is_trivially_destructible<X>
    // is_nothrow_constructible<X,args>
    // is_nothrow_default_constructible<X>
    // is_nothrow_copy_constructible<X>
    // is_nothrow_move_constructible<X>
    // is_nothrow_assignable<X,Y>
    // is_nothrow_copy_assignable<X>
    // is_nothrow_move_assignable<X>
    // is_nothrow_destructible<X>
    // has_virtual_destructor<X>

    assert(std::is_pod<int>());

    assert(!std::is_literal_type<std::string>());

    Cont<int> ci1(2);
    ci1[0] = 1;
    ci1[1] = 2;
    Cont<int> ci1copy(ci1);

    Cont<X> ci2(1);
    ci2[0] = X{};
    Cont<X> ci2copy(ci2);

    assert(std::has_virtual_destructor<complex<int>>());

    // Type Property Queries

    // n=alignment_of<X>
    // n=rank<X>
    // n=extent<X,N>
    // n=extent<X>

    // Type Relations

    // is_same<X,Y>
    // is_base_of<X,Y>
    // is_convertible<X,Y>

    draw(new Shape);
    // draw(new X); // error

    /**
     * Type Generators
     */

    // const and volatile Modification

    // remove_const<X>
    // remove_volatile<X>
    // remove_cv<X>
    // add_const<X>
    // add_volatile<X>
    // struct add_cv<X>

    My_map<int, int> map;
    // map.default_node.first = 1; // error
    map.default_node.second = 1;  // OK

    // Reference Modification
    // remove_reference<X>
    // add_lvalue_reference<X>
    // add_rvalue_reference<X>
    // decay<X>

    int val = 7;
    ref(val);
    assert(val == 8);

    // Sign Modification

    // make_signed<X>
    // make_unsigned<X>

    // Array Modification

    // remove_extent<X>
    // remove_all_extents<X>

    int a[10][20];

    std::remove_extent<decltype(a)>::type a20;  // an array[20]
    assert(typeid(a20) == typeid(int[20]));

    std::remove_all_extents<decltype(a)>::type intval;
    assert(typeid(intval) == typeid(int));

    // Pointer Modification

    // remove_pointer<X>
    // add_pointer<X>

    // Alignments

    // aligned_storage<n,a>
    // aligned_storage<n>
    // aligned_union < n, X...>

    // Other Transformations

    // enable_if<b,X>
    // enable_if<b>
    // conditional<b,T,F>
    // common_type<X>
    // underlying_type<X>
    // result_of<FX>

    Common_type<int, double> x1;  // x1 is a double
    assert(typeid(x1) == typeid(double));
    // Common_type<int,string> x2; // error: no common type
    // Common_type<int,short,long,long long> x3; // x3 is a long long
    // Common_type<Shape*,Circle*> x4; // x4 is a Shape*
    // Common_type<void*,double*,Shape*> x5; // x5 is a void*

    int ff(int);  // function
    std::result_of<decltype (&ff)(int)>::type c1 = 1;
    assert(typeid(c1) == typeid(int));

    auto fx = [](char ch) { return tolower(ch); };  // lambda
    std::result_of<decltype(fx)(char)>::type c2 = 'a';
    assert(typeid(c2) == typeid(int));

    // declval(): Returns an rvalue for T: typename add_r value_reference<T>::type ;
    // The declval() type function is unusual in the standard library because it is actually a function
    // (without users needing to wrap it). It returns a value that must never be used. The intent is to use
    // declval<X> as a type where the type of a variable of type X is needed.
}
