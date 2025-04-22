/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <limits>            // Numeric limits
#include <climits>           // C-style numeric scalar-limit macros
#include <cfloat>            // C-style numeric floating-point limit macros
#include <cstdint>           // Standard integer type names
#include <new>               // Dynamic memory management
#include <typeinfo>          // Run-time type identification support
#include <exception>         // Exception-handling support
#include <initializer_list>  // initializer_list
#include <cstddef>           // C library language support
#include <cstdarg>           // Variable-length function argument lists
#include <csetjmp>           // C-style stack unwinding
#include <cstdlib>           // Program termination
#include <ctime>             // System clock
#include <csignal>           // C-style signal handling

#include <iostream>
#include <cassert>
#include <cstring>
#include <vector>

int main()
{
    // Numeric limits
    std::cout << static_cast<int>(std::numeric_limits<unsigned char>::max()) << '\n';

    // C-style numeric scalar-limit macros
    std::cout << UCHAR_MAX << '\n';

    // C-style numeric floating-point limit macros
    std::cout << FLT_MIN << '\n';
    std::cout << DECIMAL_DIG << '\n';

    // Standard integer type names
    uint32_t val_a = 1;
    uint32_t val_b = 2;
    auto val_sum = val_a + val_b;
    assert(val_sum == 3);
    assert(typeid(val_sum) == typeid(uint32_t));

    // Dynamic memory management
    class X {
    };
    X *x = new X();
    delete x;

#ifdef _GLIBCXX_HAVE_BUILTIN_LAUNDER
    struct L {
        int i;
    };
    auto l = new L{1};
    auto pl = std::launder(l);  // since C++17
    assert(&l != &pl);
    assert(pl->i == 1);
#endif

    int *aligned = static_cast<int *>(std::aligned_alloc(1024, 1024));  // since C++17
    *aligned = 1;
    assert(*aligned == 1);
    static_assert(sizeof(aligned) == sizeof(int *));
    std::free(aligned);

    // Run-time type identification support
    class Base {
       public:
        virtual void m(){};

        virtual ~Base() = default;
    };

    class Derived : public Base {
       public:
        void m() override {}
    };

    Base *b = new Derived();
    Derived *d = dynamic_cast<Derived *>(b);
    assert(d != nullptr);

    delete d;

    int i = 1;
    assert(typeid(i) == typeid(int));

    // Exception-handling support
    try {
        throw std::runtime_error{"message"};
    }
    catch (const std::exception &err) {
        assert(strcmp(err.what(), "message") == 0);
    }

    // initializer_list
    std::initializer_list<int> list{1, 2, 3};
    std::vector<int> vec{list};
    assert(vec.size() == 3);

    // C library language support

    // Variable-length function argument lists

    // C-style stack unwinding

    // Program termination

    // System clock

    // C-style signal handling
}
