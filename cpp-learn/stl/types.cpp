/*
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <iostream>
#include <cassert>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <numeric>

template<typename T>
constexpr bool is_char() {
    return typeid(T) == typeid(char) || typeid(T) == typeid(unsigned char);
}

template<typename T>
void info(T i, int l) {
    std::cout << typeid(i).name() << '(' << l << ')'
              << ": " << sizeof(T) << ", "
              << std::numeric_limits<T>::digits
              << " (";

    if (is_char<T>()) {
        std::cout << long(std::numeric_limits<T>::min()) << ",";
        std::cout << long(std::numeric_limits<T>::max());
    } else {
        std::cout << std::numeric_limits<T>::min() << ", ";
        std::cout << std::numeric_limits<T>::max();
    }

    std::cout << ")\n";
}

void function(int) {}

int main() {
    /**
     * Primitives
     */

    // Integer
    short int s{}; // short
    info(s, __LINE__);

    unsigned short int usi{}; // unsigned short
    info(usi, __LINE__);

    unsigned ui{};
    info(ui, __LINE__);

    int i{};
    info(i, __LINE__);

    long int li{}; // unsigned long
    info(li, __LINE__);

    unsigned long int uli{}; // unsigned long
    info(uli, __LINE__);

    long long int lli{1'000'000'000}; // long long (with single quot as digit separator - since C++14)
    info(lli, __LINE__);

    unsigned long long int ulli{}; // unsigned long long
    info(ulli, __LINE__);

    // Character
    char c{};
    info(c, __LINE__);

    unsigned char uc{};
    info(uc, __LINE__);

    // Boolean
    bool b{}; // true/false
    info(b, __LINE__);

    // Floating Point
    float f{};
    info(f, __LINE__);

    // Double Floating Point
    double d{};
    info(d, __LINE__);

    // Void
    void *v{};
    info(v, __LINE__);

    // Wide Character
    // wchar_t wc{};
    // info(wc, __LINE__);

    /**
     * Derived
     */

    // Function
    info(function, __LINE__);

    // Array
    int array[4];
    static_assert(sizeof(array) == sizeof(int) * 4);

    // Pointer
    int *ip{};
    info(ip, __LINE__);

    // Reference
    auto& ref = i;
    info(ref, __LINE__);

    /**
     * User-defined
     */

    // Class
    class Class {
        int x{};
    public:
        int X() { return x; }
    };

    // Structure
    struct Struct {
        int x{};
    };

    // Union
    union Union {
        int x, y{};
    };
    union Union u;
    u.x = 1;

    // Enumeration
    enum Enum {
        v1, v2
    };

    enum class EnumClass {
        v1 = 1, v2,
    };

    // Typedef
    typedef int alias;
    alias i2 = 2;
    assert(typeid(i2) == typeid(int));

    // vs
    using alias = int;
    alias i3 = 3;
    assert(typeid(i3) == typeid(int));

    // Byte (C++17)
    std::byte byte_a{0};

    std::byte byte_b{0xff};
    int intval = std::to_integer<int>(byte_b);
    assert(intval == 255);

    std::byte byte_c = byte_a & byte_b;
    assert(std::to_integer<int>(byte_c) == 0);

    std::string bytes_str{"abcdef"};
    std::vector<std::byte> bytes;
    std::transform(bytes_str.begin(), bytes_str.end(), std::back_inserter(bytes), [](char c) {
        return static_cast<std::byte>(c);
    });

    std::for_each(bytes.begin(), bytes.end(), [](std::byte b) {
        std::cout << static_cast<int>(b);
    });
}
