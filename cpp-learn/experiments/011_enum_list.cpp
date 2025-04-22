#include <cassert>
#include <cstdint>
#include <type_traits>

enum class Values : std::uint32_t {
    a = 1,
    b = 2,
    c = 3,
    d = 4,
};

enum class BitValues : std::uint32_t {
    a = 1 << 1,
    b = 1 << 2,
    c = 1 << 3,
    d = 1 << 4,
};

struct ShiftConverter {
    template <typename Enum, typename Type>
    static constexpr Type convert(const Enum v) noexcept
    {
        return 1U << static_cast<Type>(v);
    }
};

struct ValueConverter {
    template <typename Enum, typename Type>
    static constexpr Type convert(const Enum v) noexcept
    {
        return static_cast<Type>(v);
    }
};

template <typename Enum, typename Converter = ShiftConverter>
class enum_list {
   public:
    static_assert(std::is_enum_v<Enum>, "an enum is required");

    using Type = typename std::underlying_type_t<Enum>;
    static_assert(std::is_unsigned_v<Type>, "the underlying type of the enum must be unsigned");

    enum_list() = default;
    constexpr explicit enum_list(Type values) : values_{values} {};

    template <typename... Ts>
    void set(Enum v, Ts... vs) noexcept
    {
        values_ = (get_underlying_value(v) | ... | get_underlying_value(vs));
    }

    template <typename... Ts>
    void add(Enum v, Ts... vs) noexcept
    {
        values_ |= (get_underlying_value(v) | ... | get_underlying_value(vs));
    }

    template <typename... Ts>
    void remove(Enum v, Ts... vs) noexcept
    {
        values_ &= (~get_underlying_value(v) & ... & ~get_underlying_value(vs));
    }

    void load(Type values) noexcept { values_ = values; }

    void reset() noexcept { values_ = 0U; }

    constexpr Type get() const noexcept { return values_; }

    constexpr bool none() const noexcept { return values_ == 0U; }

    template <typename... Ts>
    constexpr bool all(Enum v, Ts... vs) const noexcept
    {
        return ((values_ & get_underlying_value(v)) && ... && all(vs));
    }

    template <typename... Ts>
    constexpr bool some(Enum v, Ts... vs) const noexcept
    {
        return ((values_ & get_underlying_value(v)) || ... || some(vs));
    }

   private:
    Type values_{};
    static constexpr Type get_underlying_value(Enum v) noexcept { return Converter::template convert<Enum, Type>(v); }
};

int main()
{
    static_assert(sizeof(enum_list<Values>) == sizeof(std::underlying_type_t<Values>));
    static_assert(std::is_trivially_copyable_v<enum_list<Values>>);

    enum_list<Values> list{6U};

    assert(list.get() == 6U);
    assert(list.all(Values::a, Values::b));
    assert(!list.all(Values::a, Values::b, Values::c));
    assert(list.some(Values::a, Values::b));
    assert(!list.some(Values::c));

    list.set(Values::d, Values::c);
    assert(list.get() == 24U);

    list.add(Values::b, Values::a, Values::a, Values::a, Values::a, Values::a);
    assert(list.get() == 30U);

    assert(!list.none());
    list.reset();
    assert(list.none());

    list.add(Values::a, Values::b, Values::c, Values::d);
    assert(list.get() == 30U);

    list.remove(Values::a, Values::b);
    assert(list.get() == 24U);

    list.load(30U);
    assert(list.get() == 30U);
    assert(list.all(Values::a, Values::b, Values::c, Values::d));

    // Compile-time
    constexpr enum_list<Values> const_list{2U};
    static_assert(const_list.get() == 2U);
    static_assert(!const_list.none());
    static_assert(const_list.all(Values::a));

    constexpr enum_list<Values> const_empty_list{};
    static_assert(!const_empty_list.some(Values::a));

    // Custom converter
    constexpr enum_list<BitValues, ValueConverter> value_list{2U};
    static_assert(value_list.some(BitValues::a));
}
