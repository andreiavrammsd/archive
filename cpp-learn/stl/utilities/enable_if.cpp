/*
 https://eli.thegreenplace.net/2014/sfinae-and-enable_if/

 The C++ Programming Language, 4th Edition
 */

#include <type_traits>
#include <vector>
#include <cassert>
#include <iostream>

template <class T, typename std::enable_if_t<std::is_integral<T>::value> * = nullptr>
int do_stuff(T &)
{
    return 1;
}

template <bool B, typename T = void>
using enable_if_t = typename std::enable_if<B, T>::type;

template <class T, enable_if_t<std::is_class<T>::value> * = nullptr>
int do_stuff(T &)
{
    return 2;
}

template <class T, typename std::enable_if<std::is_function<T>::value, T>::type * = nullptr>
int do_stuff(T &)
{
    return 3;
}

template <typename T>
class vector {
    T *internal;
    size_t sz{};

   public:
    template <typename InputIterator, typename = std::_RequireInputIter<InputIterator>>
    vector(InputIterator first, InputIterator last)
    {
        sz = last - first;
        internal = new T[sz];
        std::copy(first, last, internal);
    }

    auto begin() { return internal; }

    auto end() { return internal + sz; }

    bool operator==(const std::vector<T> vec)
    {
        if (vec.size() != sz) {
            return false;
        }

        for (size_t i = 0; i < sz; i++) {
            if (internal[i] != vec.at(i)) {
                return false;
            }
        }

        return true;
    }

    vector(const vector<T> &) = delete;

    vector &operator=(const vector<T> &) = delete;

    virtual ~vector() { delete[] internal; }
};

struct S {
};

void f() {}

// Implementing Enable_if
template <bool B, typename T = void>
struct enable_if {
    using type = T;
};

template <typename T>
struct enable_if<false, T> {
};  // no ::type if B==false

template <bool B, typename T = void>
using Enable_if = typename enable_if<B, T>::type;

template <typename T, typename = Enable_if<std::is_enum<T>::value>>
int do_stuff(T t)
{
    return 4 + t;
}

template <typename T>
Enable_if<std::is_pointer<T>::value> fct(T a, T b)
{
    auto tmp = *b;
    *b = *a;
    *a = tmp;
}

int main()
{
    // Compile-time switch for templates
    int i{};
    assert(do_stuff(i) == 1);

    S s{};
    assert(do_stuff(s) == 2);

    assert(do_stuff(f) == 3);

    // Class constructor accepting only input iterator
    std::vector<int> v{1, 2, 3};
    vector<int> v2{v.begin(), v.end()};
    assert(v2 == v);

    // Implementing Enable_if
    enum en { val = 2 };
    assert(do_stuff(val) == 4 + val);

    auto a = new int(1);
    auto b = new int(2);
    assert(*a == 1);
    assert(*b == 2);
    fct(a, b);
    assert(*a == 2);
    assert(*b == 1);

    auto c = new int(1);
    auto d = new int(2);
    assert(*c == 1);
    assert(*d == 2);
    fct(&c, &d);
    assert(*c == 2);
    assert(*d == 1);

    // auto e = 1;
    // fct(e, e); // error: e is not pointer
}
