/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <utility>
#include <cassert>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <typeindex>
#include <string>

class X {
   public:
    int x{};

    virtual void m() {}
};

void a(X &&x);

void b(X &&x);

void a(X &&x)
{
    ++x.x;
    assert(x.x == 1);

    b(std::forward<X>(x));
    assert(x.x == 2);
}

void b(X &&x) { ++x.x; }

template <typename T>
void swap_impl(T &a, T &b) noexcept(std::is_nothrow_move_constructible<T>() && std::is_nothrow_move_assignable<T>())
{
    T tmp{move(a)};
    a = move(b);
    b = move(tmp);
}

struct Val {
    double d;

    bool operator==(Val v) const { return d == v.d; }

    bool operator<(Val v) const { return d < v.d; }
};

void my_algo(std::vector<Val> &vv)
{
    using namespace std::rel_ops;
    for (size_t i = 0; i < vv.size(); ++i)
        if (Val{0} > vv[i]) vv[i].d = std::abs(vv[i].d);  // OK: > from rel_ops
}

int main()
{
    /**
     * move() and forward()
     *
     * Use move() when the intent is to "steal the representation" of an object with a move operation,
     * and use forward() for forwarding. Thus, forward(x) is safe, whereas move(x) marks x for destruction
     * so that move(x) should be used with care. The only safe use of an x after a move(x) is destruction or
     * as a target for an assignment.
     */

    // x2=forward(x)
    // x2=move(x)
    // x2=move_if_noexcept(x)

    X x;
    a(std::move(x));  // using x after this point is undefined behavior

    X y;
    a(std::forward<X>(y));

    /**
     * swap()
     */

    int a = 1, b = 2;
    assert(a == 1);
    assert(b == 2);

    std::swap(a, b);
    assert(a == 2);
    assert(b == 1);

    std::vector<int> va = {1, 2};
    std::vector<int> vb = {3, 4};
    assert((va == std::vector<int>{1, 2}));
    assert((vb == std::vector<int>{3, 4}));

    swap_impl(va, vb);
    assert((va == std::vector<int>{3, 4}));
    assert((vb == std::vector<int>{1, 2}));

    static_assert(std::is_swappable<std::vector<int>>::value);             // since C++17
    static_assert(!std::is_swappable_with<std::vector<int>, int>::value);  // since C++17

    /**
     * Relational Operators
     *
     * Given a user-defined operator== and operator< for objects of type T, implements
     * the usual semantics of other comparison operators.
     *
     * https://en.cppreference.com/w/cpp/utility/rel_ops/operator_cmp
     */

    Val v1{1};
    Val v2{2};

    assert(!(v1 == v2));
    assert(v1 < v2);

    using namespace std::rel_ops;

    assert(v1 != v2);
    assert(!(v1 > v2));
    assert(v1 <= v2);
    assert(!(v1 >= v2));

    std::vector<Val> vval{{-1}, {2}};
    my_algo(vval);
    assert((vval == std::vector<Val>{{1}, {2}}));

    /**
     * Comparing and Hashing type_info
     */

    std::unordered_map<std::type_index, std::string> types;
    types[std::type_index{typeid(int)}] = "int";
    types[std::type_index{typeid(double)}] = "double";

    assert(types[std::type_index{typeid(1)}] == "int");
    assert(types[std::type_index{typeid(1.0)}] == "double");
}
