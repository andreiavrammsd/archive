#include <cassert>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

using func = std::function<int(int)>;

// template <class T, class V>
// concept IterableOfType = requires(T& it)
//{
//     it.begin();
//     it.end();
//     requires(std::is_same_v<typename std::remove_reference<decltype(*it.begin())>::type, V>);
//     //    requires(std::is_same_v<typename T::value_type, V>);
// };

// template <typename T, typename V>
// constexpr bool IterableOfType()
//{
//     return std::is_same<decltype(std::begin(std::declval<T&>())), decltype(std::end(std::declval<T&>()))>::value &&
//            std::is_same<typename std::remove_reference<decltype(*std::begin(std::declval<T&>()))>::type, V>::value;
// }

template <typename T, typename V, typename = decltype(std::begin(std::declval<T>())),
          typename = decltype(std::end(std::declval<T>()))>
constexpr bool IterableOfType()
{
    return std::is_same<typename T::value_type, V>::value;
}

template <typename, typename>
constexpr bool IterableOfType(unsigned)
{
    return false;
}

//, typename Iterator = typename T::iterator,
// typename = decltype(std::begin(std::declval<T>())), typename = decltype(std::end(std::declval<T>()))

// inline constexpr bool IterableOfType() { return false; }

template <typename T, typename = void>
struct is_iterable : std::false_type {
};

template <typename T>
struct is_iterable<T> : std::true_type {
};

template <typename T>
struct service {
    static_assert(is_iterable<T>(), "expected iterable with \"func\" value type");
    //    static_assert(IterableOfType<T, func>(), "expected iterable with \"func\" value type");
    //    static_assert(IterableOfType<T, func>, "expected iterable with \"func\" value type");
    T& funcs;

    int iterate(int input) const
    {
        int output = input;
        for (const auto& func : funcs) {
            output = func(output);
        }
        return output;
    }
};

struct custom_iterator {
    using container = std::array<func, 2>;
    //    using value_type = func;

    container funcs = {
        [](int n) { return n * 2; },
        [](int n) { return n - 2; },
    };

    //    container::iterator begin() noexcept { return funcs.begin(); };
    container::iterator end() noexcept { return funcs.end(); };
};

int main()
{
    std::array<func, 2> funcs = {
        [](int n) { return n * 2; },
        [](int n) { return n - 2; },
    };
    service<std::array<func, 2>> srv{funcs};

    assert(srv.iterate(1) == 0);

    std::array<func, 1> other_funcs = {
        [](int n) { return n + 2; },
    };
    service<std::array<func, 1>> other_srv{other_funcs};

    assert(other_srv.iterate(1) == 3);

    std::vector<func> vector_funcs = {
        [](int n) { return n * 2; },
        [](int n) { return n - 2; },
    };
    service<std::vector<func>> vector_srv{vector_funcs};

    assert(vector_srv.iterate(1) == 0);

    //    custom_iterator it{};
    //    service<custom_iterator> service_with_custom_iterator{it};
    //    assert(service_with_custom_iterator.iterate(1) == 0);

    //    std::vector<int> vector_ints = {};
    //    service<std::vector<int>> vector_ints_srv{vector_ints};
    //
    //    assert(vector_ints_srv.iterate(1) == 1);
}
//
// std::vector<int> vector_ints = {};
// service<std::vector<int>> vector_ints_srv{vector_ints};
//
//// error: expression cannot be used as a function
//// output = func(output);

// error: template constraint failure for ‘template<class T>  requires  IterableOfType<T, std::function<int()> > struct
// service’ service<std::vector<int>> vector_ints_srv{vector_ints};