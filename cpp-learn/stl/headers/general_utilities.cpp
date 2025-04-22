/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <chrono>            // Time utilities
#include <ctime>             // C-style date and time
#include <functional>        // Functions objects
#include <iterator>          // Iterators and iterator support
#include <memory>            // Resource management pointers
#include <ratio>             // Compile-time rational arithmetic
#include <scoped_allocator>  // Scoped allocators
#include <tuple>             // Tuples
#include <type_traits>       // Type traits
#include <typeindex>         // Use a type_info as key or a hash code
#include <utility>           // Operators and pairs

// C++17
#include <any>  // Objects that hold instances of any CopyConstructible type
#include <cassert>
#include <iostream>
#include <map>
#include <optional>  // Optional value
#include <variant>   // Type-safe union

template <typename T>
struct Simple_alloc {  // use new[] and delete[] to allocate and deallocate bytes
    using value_type = T;

    Simple_alloc() = default;

    T *allocate(size_t n) { return reinterpret_cast<T *>(new char[n * sizeof(T)]); }

    void deallocate(T *p, size_t) { delete[] reinterpret_cast<char *>(p); }
};

std::optional<std::string> create(bool b)
{
    if (b) {
        return "Godzilla";
    }

    return {};
}

struct X {
    int val;
};

// func_ is enabled if all Ts... have the same type as T
template <typename T, typename... Ts>
std::enable_if_t<std::conjunction_v<std::is_same<T, Ts>...>> func_conjunction(T, Ts...)
{
    std::cout << "all types in pack are T\n";
}

// otherwise
template <typename T, typename... Ts>
std::enable_if_t<!std::conjunction_v<std::is_same<T, Ts>...>> func_conjunction(T, Ts...)
{
    std::cout << "not all types in pack are T\n";
}

int main()
{
    // Operators and pairs
    std::pair<int, bool> pair;
    assert(pair.first == 0);
    assert(!pair.second);

    std::string mutableString = "Hello World!";
    const std::string &constView = std::as_const(mutableString);  // since C++17
    assert(constView == mutableString);

    // Tuples
    std::tuple<int, bool, float> tuple;
    assert(std::get<0>(tuple) == 0);

    // Type traits
    typename std::enable_if<true, int>::type enable;
    assert(typeid(enable) == typeid(int));

    typename std::conditional<true, int, double>::type conditional;
    assert(typeid(conditional) == typeid(int));

    func_conjunction(1, 2, 3);  // since C++17
    func_conjunction(1, 2, "hello!");
    // disjunction // since C++17
    // negation // since C++17

    struct Agg {
        int i;
    };
    static_assert(std::is_aggregate<Agg>::value);  // since C++17
    static_assert(!std::is_aggregate<int>::value);

    struct Unique {
        int i;
    };
    static_assert(std::has_unique_object_representations<Unique>::value);  // since C++17
    struct NonUnique {
    };
    static_assert(!std::has_unique_object_representations<NonUnique>::value);

    // Use a type_info as key or a hash code
    std::map<std::type_index, std::string> tmap;
    tmap[typeid(int)] = "int";

    // Functions objects
    std::function<int(int, int)> func;
    func = [](int, int) -> int { return 1; };

    std::vector<int> nums{1, 2, 3};
    auto is_even = [](int x) { return x % 2 == 0; };
    auto is_odd = std::not_fn(is_even);  // since C++17
    auto odd_count = std::count_if(nums.cbegin(), nums.cend(), is_odd);
    assert(odd_count == 2);

    // Resource management pointers
    std::unique_ptr<int> uptr;
    uptr = std::make_unique<int>(1);

    std::shared_ptr<int> sptr;
    sptr = std::make_shared<int>(2);

    // Scoped allocators
    {
        std::vector<int, Simple_alloc<int>> vec(2);
        assert(typeid(vec.get_allocator()) == typeid(Simple_alloc<int>{}));
        vec[0] = 1;
    }

    // Compile-time rational arithmetic
    static_assert(std::ratio<1, 2>::num == 1);
    static_assert(std::ratio<1, 2>::den == 2);
    static_assert(std::ratio_add<std::ratio<4, 4>, std::ratio<8, 8>>::num == 2);
    static_assert(std::ratio_add<std::ratio<1, 10>, std::ratio<1, 100>>::den == 100);

    // Time utilities
    using namespace std::chrono_literals;  // since C++14
    auto h = 24h;
    assert(h == 1440min);

    // C-style date and time
    clock_t t = clock();
    assert(t / CLOCKS_PER_SEC == 0);

    time_t seconds;

    seconds = time(nullptr);
    std::cout << seconds << '\n';

    time(&seconds);
    std::cout << seconds << '\n';

    struct tm tm = *localtime(&seconds);
    printf("%d-%0d-%0d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    std::cout << '\n';

    // Iterators and iterator support
    std::string str = "the quick brown fox";

    std::string::const_iterator p = std::find(str.cbegin(), str.cend(), 'o');
    assert(p == str.cbegin() + 12);

    std::string::const_reverse_iterator rp = std::find(str.crbegin(), str.crend(), 'o');
    assert(rp == str.crbegin() + 1);

    std::vector<int> numbers{1, 2, 3, 4};
    std::copy(numbers.begin(), numbers.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';

    // Objects that hold instances of any CopyConstructible type
    std::any x{5};
    assert(x.has_value());
    assert(std::any_cast<int>(x) == 5);
    assert(std::any_cast<int &>(x) == 5);
    try {
        std::any_cast<int *>(x);
        assert(false);
    }
    catch (const std::bad_any_cast &) {
    }

    // Optional value
    assert(create(false).value_or("empty") == "empty");
    assert(create(true).value_or("empty") == "Godzilla");
    assert(create(true).value() == "Godzilla");
    assert(create(true).value().size() == 8);
    assert(!create(true).value().empty());

    static_assert(std::optional<X>{X{1}}.value().val == 1);

    // Type-safe union
    std::variant<int, double, X *> v{12};
    assert(std::get<int>(v) == 12);
    assert(std::get<0>(v) == 12);

    v = 12.0;
    assert(std::get<double>(v) == 12.0);
    assert(std::get<1>(v) == 12.0);

    v = new X{2};
    assert(std::get<X *>(v)->val == 2);
    assert(std::get<2>(v)->val == 2);

    struct A {
        int i;
    };
    struct B {
        int i;
    };
    struct Visitor {
        void operator()(const A &a, const A &a2) { std::cout << '{' << a.i << ',' << a2.i << '}' << '\n'; };

        void operator()(const A &a, const B &b) { std::cout << '{' << a.i << ',' << b.i << '}' << '\n'; };

        void operator()(const B &b, const B &b2) { std::cout << '{' << b.i << ',' << b2.i << '}' << '\n'; };

        void operator()(const B &b, const A &a) { std::cout << '{' << b.i << ',' << a.i << '}' << '\n'; };
    };

    std::variant<A, B> var1{A{1}}, var2{A{2}};
    std::visit(Visitor{}, var1, var2);

    var1 = {B{3}};
    var2 = {B{4}};
    std::visit(Visitor{}, var2, var1);
}
