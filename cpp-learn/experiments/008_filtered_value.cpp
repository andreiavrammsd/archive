#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <tuple>

namespace msd {

template <typename Wrappers, std::size_t Size = std::tuple_size<Wrappers>::value, std::size_t Index = Size - 1>
class wrappers_iterator {
   public:
    template <typename Value>
    Value operator()(Wrappers& wrappers, Value&& new_value) const
    {
        auto value = std::get<Size - Index - 1>(wrappers)(std::forward<Value>(new_value));
        return wrappers_iterator<Wrappers, Size, Index - 1>{}(wrappers, std::move(value));
    }
};

template <typename Wrappers, std::size_t Size>
class wrappers_iterator<Wrappers, Size, 0> {
   public:
    template <typename Value>
    Value operator()(Wrappers& wrappers, Value&& new_value) const
    {
        return std::get<Size - 1>(wrappers)(std::forward<Value>(new_value));
    }
};

template <typename Value, typename... Wrappers>
class wrapped_value {
    static_assert(sizeof...(Wrappers) > 0, "at least one wrapper required");

   public:
    wrapped_value() noexcept(std::is_nothrow_default_constructible<Value>::value) = default;

    template <class T>
    wrapped_value(T&& value) noexcept(std::is_nothrow_constructible<Value>::value&& noexcept(
        this->iterate_(this->wrappers_, std::forward<Value>(value))))
        : value_(iterate_(wrappers_, std::forward<Value>(value)))
    {
    }

    operator const Value&() const noexcept { return value_; }

    // new
    operator Value&() noexcept { return value_; }
    //        operator Value const *() noexcept { return value_; }
    //    operator Value* const() noexcept { return &value_; }
    //    operator const Value*() noexcept { return &value_; }
    //    operator const Value* const() noexcept { return &value_; }
    //    operator Value*() noexcept { return &value_; }

    wrapped_value& operator+=(const Value& value) noexcept(noexcept(this->iterate_(this->wrappers_,
                                                                                   this->value_ + value, this->value_)))
    {
        iterate_(wrappers_, value_ + value, value_);
        return *this;
    }

    wrapped_value& operator++() noexcept(noexcept(this->iterate_(this->wrappers_, this->value_ + 1)))
    {
        value_ = iterate_(wrappers_, value_ + 1);
        return *this;
    }

    wrapped_value& operator--() noexcept(noexcept(this->iterate_(this->wrappers_, this->value_ - 1)))
    {
        value_ = iterate_(wrappers_, value_ - 1);
        return *this;
    }

    wrapped_value& operator-=(const Value& value) noexcept(noexcept(this->iterate_(this->wrappers_,
                                                                                   this->value_ - value)))
    {
        value_ = iterate_(wrappers_, value_ - value);
        return *this;
    }

    bool operator!() const noexcept(noexcept(!this->value_)) { return !value_; }

    friend std::ostream& operator<<(std::ostream& os, const wrapped_value& v) noexcept(noexcept(os << v.value_))
    {
        os << v.value_;
        return os;
    }

   private:
    Value value_;

    using WrappersType = std::tuple<Wrappers...>;
    static WrappersType wrappers_;
    static wrappers_iterator<WrappersType> iterate_;
};

}  // namespace msd

template <typename Value, typename... Wrappers>
std::tuple<Wrappers...> msd::wrapped_value<Value, Wrappers...>::wrappers_ = {};

template <typename Value, typename... Wrappers>
msd::wrappers_iterator<std::tuple<Wrappers...>> msd::wrapped_value<Value, Wrappers...>::iterate_ = {};

// Wrappers
template <typename Value, int Limit>
struct LowerBoundLimiter {
    constexpr Value operator()(const Value& value) const noexcept { return std::max(value, Limit); }
};

template <typename Value, int Limit>
struct UpperBoundLimiter {
    constexpr Value operator()(const Value& value) const noexcept { return std::min(value, Limit); }
};

template <typename Value>
struct Notifier {
    Value operator()(const Value& value) const noexcept
    {
        std::cout << "Value set to: " << value << "\n";
        return value;
    }
};

template <typename Value, int Limit>
struct UpperLimitValidator {
    Value operator()(const Value& value) const
    {
        if (value > Limit) {
            throw std::range_error{"upper limit error"};
        }
        return value;
    }
};

// Struct with wrapped values
struct Input {
    msd::wrapped_value<int, LowerBoundLimiter<int, 1>, UpperBoundLimiter<int, 10>, Notifier<int>> a;
    msd::wrapped_value<int, UpperLimitValidator<int, 1000>, UpperBoundLimiter<int, 10>> b;
};

int inc_by_value(int a)
{
    auto res = a + 1;
    return res;
}
int inc_by_const_ref(const int& a) { return a + 1; }
void inc_by_ref(int& a) { ++a; }
void update(int& a) { a = 20; }

void inc3(int& a) { a = 1000; }
// void inc3(int* a) { (*a)++; }
//  void inc5(int* const a) { (*a)++; }
//  int inc6(const int* a) { return (*a) + 1; }
//  int inc7(const int* const a) { return (*a) + 1; }

/*
 * void inc2(int& a) { a++; }
int inc3(const int& a) { return a + 1; };
void inc4(int* a) { (*a)++; }
void inc5(int* const a) { (*a)++; };
int inc6(const int* a) { return (*a) + 1; };
int inc7(const int* const a) { return (*a) + 1; };
 */

// struct

// struct MyType {
//     int i;
// };

template <typename Value, int Limit>
struct MyUpperBoundLimiter {
    constexpr Value operator()(const Value& value) const noexcept { return Value{std::min(value.i, Limit)}; }
};

int main()
{
    //    msd::wrapped_value<MyType, MyUpperBoundLimiter<MyType, 10>> my_value = MyType{2};
    //    assert(my_value.operator!());

    msd::wrapped_value<int, UpperBoundLimiter<int, 10>> value = 2;
    assert(value == 2);
    assert(inc_by_value(value) == 3);
    assert(value == 2);
    assert(inc_by_const_ref(value) == 3);
    assert(value == 2);

    inc_by_ref(value);
    assert(value == 3);

    update(value);
    //    assert(value == 10);

    // working already

    //    inc3(value);
    //    assert(value == 1000);

    //    inc3(&value);
    //    assert(value == 1001);

    Input in{};

    static_assert(sizeof(in.a) == sizeof(int), "size of wrapped_value greater than type's");

    double a = 1000.0;
    in.a = a;
    assert(in.a == 10);

    in.a = 1;
    in.b = 2;
    assert(in.a == 1);
    assert(in.b == 2);

    std::stringstream str;
    str << in.a;
    assert(str.str() == "1");

    int x = in.a + 1;
    assert(x == 2);

    auto y = 2 + in.a;
    assert(y == 3);

    auto x1 = in.a - 1;
    assert(x1 == 0);

    int y1 = 2 - in.a;
    assert(y1 == 1);

    in.a = 100;
    assert(in.a == 10);
    assert(10 == in.a);
    assert(in.a == 10.0);

    assert(in.a == in.a);
    assert(!(in.a == in.b));
    assert(in.a != in.b);
    assert(in.a <= 10);
    assert(in.b < in.a);

    ++in.a;
    assert(in.a == 10);

    --in.a;
    assert(in.a == 9);

    ++in.a;
    assert(in.a == 10);

    in.a = in.a - in.b;
    assert(in.a == 8);

    in.a -= 5;
    assert(in.a == 3);

    assert(not in.a == false);

    in.b = 1000;
    assert(in.b == 10);

    try {
        in.b = 1001;
        assert(false);
    }
    catch (const std::range_error&) {
    }

    assert(inc_by_value(in.a) == 4);
}
