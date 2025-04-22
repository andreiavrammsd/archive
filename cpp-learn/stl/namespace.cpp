/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <string>
#include <vector>
#include <iostream>

// Convenience vs. Safety
namespace X {
    int i, j, k;
}

int k;

void f1() {
    int i = 0;
    using namespace X; // make names from X accessible
    i++; // local i
    j++; // X::j
    // k++; // error : X’s k or the global k?
    ::k++; // the global k
    X::k++; // X's l
}

void f2() {
    int i = 0;
    // using X::i; // error: i declared twice in f2()
    using X::j;
    using X::k; // hides global k

    i++;
    j++; // X::j
    k++; // X::k
}

// Alias
namespace American_Telephone_and_Telegraph {
    int x{};
}

namespace ATT = American_Telephone_and_Telegraph;

// Composition
namespace His_string {
    class String {
    public:
        std::string string;
    };

    String operator+(const String &, const String &);

    String operator+(const String &, const char *);
}

namespace Her_vector {
    template<class T>
    class Vector : public std::vector<T> {
    };
}

namespace My_lib {
    using namespace His_string;
    using namespace Her_vector;

    void my_fct(String &);
}

void My_lib::my_fct(String &s) {
    std::cout << s.string << std::endl;
}

void f() {
    My_lib::String s{"Byron"}; // finds My_lib::His_string::String
}

using namespace My_lib;

void g(Vector<String> &vs) {
    my_fct(vs[0]);
}

// Composition and Selection
namespace His_lib {
    class String {
    };

    template<class T>
    class Vector {

    };
}

namespace Her_lib {
    template<class T>
    class Vector {
    };

    class String {
    };
}

namespace My_lib {
    using namespace His_lib; // everything from His_lib
    using namespace Her_lib; // everything from Her_lib

    using His_lib::String; // resolve potential clash in favor of His_lib
    using Her_lib::Vector; // resolve potential clash in favor of Her_lib

    template<class T>
    class List { // additional stuff
    };
}

namespace Lib2 {
    using namespace His_lib; // everything from His_lib
    using namespace Her_lib; // everything from Her_lib

    using His_lib::String; // resolve potential clash in favor of His_lib
    using Her_lib::Vector; // resolve potential clash in favor of Her_lib

    using Her_string = Her_lib::String; // rename
    template<class T>
    using His_vec = His_lib::Vector<T>; // rename

    template<class T>
    class List { // additional stuff
    };
}

// Versioning
namespace Popular {
    inline namespace V3_2 { // V3_2 provides the default meaning of Popular
        double f(double);

        int f(int) { return 0; }

        template<class T>
        class C {
        };
    }

    namespace V3_0 {
        double f(double) { return 0; }
    }

    namespace V2_4_2 {
        double f(double) { return 0; }

        template<class T>
        class C {
        };
    }
}

namespace A {
    namespace B {
        namespace C {
            int i;
        }
    }
}
// vs.
namespace A::B::C {
    int j;
}

int main() {
    // Convenience vs. Safety
    f1();
    f2();

    assert(X::i == 0);
    assert(X::j == 2);
    assert(X::k == 2);
    assert(k == 1);
    assert(::k == 1);

    // Alias
    American_Telephone_and_Telegraph::x++;
    ATT::x++;
    assert(American_Telephone_and_Telegraph::x == ATT::x);
    assert(ATT::x == 2);

    // Composition
    f();
    Her_vector::Vector<His_string::String> vec;
    vec.push_back(His_string::String{"Byron"});
    g(vec);

    // Versioning
    using namespace Popular;

    f(1); // Popular::V3_2::f(int)
    V3_0::f(1); // Popular::V3_0::f(double)
    V2_4_2::f(1); // Popular::V2_4_2::f(double)

    // Nested namespaces with namespace resolution operator (C++17)
    A::B::C::i = 1;
    A::B::C::j = 1;
    assert(A::B::C::i == A::B::C::j);
}
