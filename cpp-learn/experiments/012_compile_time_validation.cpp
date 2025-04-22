#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>

namespace lib {
using A = std::array<int, 9>;

inline A fetch()
{
    A a;
    std::iota(a.begin(), a.end(), 1);
    return a;
}
}  // namespace lib

namespace app {
struct S {
    int i = 0;
    S() = default;
    explicit S(int v) noexcept : i{v} {}
};
inline bool operator==(int i, S s) noexcept { return i == s.i; }

using B = std::array<S, 9>;

template <typename>
struct array_size;

template <typename T, std::size_t S>
struct array_size<std::array<T, S>> {
    static constexpr std::size_t size = S;
};

inline B convert(const lib::A& a)
{
    B b;

    static_assert(array_size<lib::A>::size == array_size<B>::size, "size mismatch");

    std::transform(a.cbegin(), a.cend(), b.begin(), [](int i) noexcept { return S{i}; });

    return b;
}
}  // namespace app

int main()
{
    const auto a = lib::fetch();
    const auto b = app::convert(a);

    assert(std::equal(a.cbegin(), a.cend(), b.cbegin()));
}
