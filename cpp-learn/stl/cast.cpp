/*
 * https://embeddedartistry.com/blog/2017/03/15/c-casting-or-oh-no-they-broke-malloc/
 */

#include <cassert>
#include <cstdint>
#include <iostream>

class Parent {
   public:
    virtual void call() = 0;

    virtual ~Parent() = default;
};

class Child : public Parent {
   public:
    void call() override { std::cout << "call\n"; }

    void call2() { std::cout << "call2 \n"; };
};

class Child2 : public Parent {
   public:
    void call() override { std::cout << "call\n"; }

    void call3() { std::cout << "call3 \n"; };
};

class Other {};

void call(Parent *p)
{
    p->call();

    if (auto c = dynamic_cast<Child *>(p)) {
        c->call2();
    }
}

void call(Parent &p)
{
    p.call();

    try {
        auto c = dynamic_cast<Child &>(p);
        c.call2();
    }
    catch (const std::bad_cast &) {
        std::cout << "cannot call\n";
    }

    try {
        auto c = dynamic_cast<Child2 &>(p);
        c.call3();
    }
    catch (const std::bad_cast &) {
        std::cout << "cannot call\n";
    }
}

struct S {
    int x;
};

int test(int *x) { return *x; }

int main()
{
    /**
     * static_cast
     *       no runtime checks
     *       cannot cast away const or volatile
     *
     * reinterpret_cast
     *      compiler directive which tells the compiler to treat the current type as a new type
     *      cannot cast away const or volatile
     *
     * const_cast
     *      adds or removes const from a variable
     *      can use to add or remove volatile from a variable
     *      it's not safe to remove const and start modifying the underlying data
     *
     * dynamic_cast
     *      run-time-type-information (RTTI)
     *      handle polymorphism
     *      can convert pointers and references to any polymorphic type at run-time
     *
     * C-style casts
     *      Avoid C-style casts. Be explicit in your intentions
     */

    // static_cast
    int *y = static_cast<int *>(malloc(2 * sizeof(int)));
    *y = 1;
    assert(*y == 1);
    free(y);

    int a = 1;
    long b = static_cast<long>(a);
    assert(b == a);

    // reinterpret_cast
    int *pi = new int(1);
    // uintptr_t ptr = static_cast<uintptr_t>(p); // Static_cast from 'int *' to 'uintptr_t' (aka 'unsigned long') is
    // not allowed
    uintptr_t ptr = reinterpret_cast<uintptr_t>(pi);
    assert(ptr != 0);
    delete pi;

    S s1{1};
    auto s_ref = reinterpret_cast<int &>(s1);
    assert(s_ref == 1);

    S *s2 = new S{2};
    auto s_ptr = reinterpret_cast<int *>(s2);
    assert(s_ptr != nullptr);
    assert(*s_ptr == 2);
    delete s2;

    /*
    // fails on Clang/Linux
    try {
        auto sse = reinterpret_cast<std::string &>(s);
    } catch (const std::exception &e) {
        std::cout << e.what() << "\n";
    }
    //*/

    auto sp = new S{2};

    auto spp = reinterpret_cast<int *>(sp);
    assert(spp != nullptr);
    assert(*spp == 2);

    auto spc = reinterpret_cast<std::string *>(sp);  // possible, but not good
    assert(spc != nullptr);
    assert(spc->empty());  // invalid read

    delete sp;

    // const_cast
    volatile int vint = 1;
    // test(&vint); // 1st argument ('volatile int *') would lose volatile qualifier
    auto result = test(const_cast<int *>(&vint));
    assert(result == 1);

    // dynamic_cast
    auto c = new Child;
    c->call();

    auto p = dynamic_cast<Parent *>(c);
    assert(p != nullptr);
    p->call();

    auto pp = dynamic_cast<Other *>(c);
    assert(pp == nullptr);

    call(c);

    auto c2ptr = new Child2;
    call(c2ptr);
    delete c2ptr;

    Child c2;
    call(c2);

    Child2 c3;
    call(c3);

    delete c;

    // C-style casts
    // A C-style cast in C++ tries the following casts in order, using the first C++ cast that works:
    //      const_cast
    //      static_cast
    //      static_cast, then const_cast (change type + remove const)
    //      reinterpret_cast
    //      reinterpret_cast, then const_cast (change type + remove const)
}
