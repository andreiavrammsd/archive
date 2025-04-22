/*
 The C++ Programming Language, 4th Edition

 Constructors, destructors, and copy and move operations for a type are not logically separate.
 We must define them as a matched set or suffer logical or performance problems. If a class X has a
 destructor that performs a nontrivial task, such as free-store deallocation or lock release, the class is
 likely to need the full complement of functions

 A programmer who declares a destructor for a class must also decide if objects of that class can be
 copied or moved (§17.6).

 Had I aimed at optimal performance, I would have used three separate constructors rather than a
 single constructor with default arguments.

 https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
 */

#include <iostream>
#include <cassert>

using SomeType = int;

class X {
   public:
    explicit X(SomeType);  // "ordinary constructor": create an object

    X();  // default constructor

    X(const X &);  // copy constructor

    X(X &&) noexcept;  // move constructor

    X &operator=(const X &);  // copy assignment: clean up target and copy

    X &operator=(X &&) noexcept;  // move assignment: clean up target and move

    virtual ~X();  // destructor: clean up

    SomeType const *Somevalueptr() const;

   private:
    SomeType *somevalue{};
};

X::X(SomeType val)
{
    somevalue = new SomeType{};
    *somevalue = val;
}

X::X() = default;

X::X(const X &x) : somevalue(new SomeType(*x.somevalue)) {}  // copy constructor

X::X(X &&x) noexcept : somevalue{nullptr}
{  // move constructor
    somevalue = x.somevalue;
    x.somevalue = nullptr;
}

X &X::operator=(const X &x)
{  // copy assignment: clean up target and copy
    if (this == &x) {
        return *this;
    }

    somevalue = new SomeType{};
    *somevalue = *x.somevalue;

    return *this;
}

X &X::operator=(X &&x) noexcept
{  // move assignment: clean up target and move
    if (this == &x) {
        return *this;
    }

    delete somevalue;

    somevalue = x.somevalue;
    x.somevalue = nullptr;

    return *this;
}

X::~X() { delete somevalue; }

SomeType const *X::Somevalueptr() const { return somevalue; }

SomeType moveval(X x) { return *x.Somevalueptr(); }

int main()
{
    // ordinary constructor
    X a(1);

    // default constructor
    X b;
    assert(b.Somevalueptr() == nullptr);

    // copy constructor
    X c(a);
    assert(*a.Somevalueptr() == 1);
    assert(*c.Somevalueptr() == 1);

    // move constructor
    assert(c.Somevalueptr() != nullptr);
    assert(moveval(std::move(c)) == 1);
    assert(c.Somevalueptr() == nullptr);

    // copy assignment
    b = a;
    assert(*a.Somevalueptr() == 1);
    assert(*b.Somevalueptr() == 1);
    assert(a.Somevalueptr() != b.Somevalueptr());

    // move assignment
    X d = std::move(b);
    assert(*d.Somevalueptr() == 1);
    assert(b.Somevalueptr() == nullptr);
}
