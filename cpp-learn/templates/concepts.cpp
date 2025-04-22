/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP20.md
 */

#include <initializer_list>
#include <ostream>
#include <vector>
#include <cassert>
#include <sstream>

template <typename T>
constexpr bool Regular()
{
    return true;
}

template <typename T>
constexpr bool Totally_ordered()
{
    return true;
}

template <typename T>
constexpr bool Ordered()
{
    return Regular<T>() && Totally_ordered<T>();
}

template <typename C>
class String {
    static_assert(Ordered<C>(), "String's character type is not ordered");

   public:
    String(std::initializer_list<C> chars) : chars{chars} {}

   private:
    std::vector<C> chars;

    template <typename T>
    friend std::ostream &operator<<(std::ostream &os, const String<T> &s);
};

template <typename C>
std::ostream &operator<<(std::ostream &os, const String<C> &s)
{
    for (const auto &c : s.chars) {
        os << c;
    }
    return os;
}

// concepts (C++20)
template <typename T>
concept integral = std::is_floating_point_v<T>;

template <integral T>
class Num {
   public:
    explicit Num(T num) : num{num} {}

    [[nodiscard]] T getNum() const { return num; }

   private:
    T num;
};

template <typename T>
T twice(T v) requires integral<T>
{
    return 2.0 * v;
}

template <typename T>
concept callable = requires(T f)
{
    f();
};

template <typename T>
int call(T f) requires callable<T>
{
    return f();
}

int main()
{
    String<char> str{{97, 'b'}};

    std::ostringstream ss;
    ss << str;
    assert(ss.str() == "ab");

    // concepts (C++20)
    Num num(1.0);
    assert(num.getNum() == 1.0);

    assert(twice(1.0) == 2.0);

    auto lambda = []() { return 1; };
    assert(call(lambda) == 1);
}
