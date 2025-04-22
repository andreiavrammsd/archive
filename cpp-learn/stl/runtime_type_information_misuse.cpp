/*
 The C++ Programming Language, 4th Edition
 */

// misuse of run-time type information:

#include <vector>
#include <string>
#include <cassert>

class Object {  // polymorphic
    virtual void m() = 0;
};

class Container : public Object {
   public:
    void put(Object *){};

    Object *get() { return this; };
};

class Ship : public Object {
};

Ship *f(Ship *ps, Container *c)
{
    c->put(ps);

    Object *p = c->get();
    if (Ship *q = dynamic_cast<Ship *>(p)) {
        return q;
    }
    else {
        return nullptr;
    }
}

// Here, class Object is an unnecessary implementation artifact. It is overly general because it does
// not correspond to an abstraction in the application domain and forces the application programmer
// to use an implementation-level abstraction ( Object ). Problems of this kind are often better solved
// by using container templates that hold only a single kind of pointer:

class BetterShip {
   public:
    std::string m() { return "ship"; }
};

BetterShip *f(BetterShip *ps, std::vector<BetterShip *> &c)
{
    c.push_back(ps);

    auto v = c.back();
    c.pop_back();
    return v;
}
// This style of code is less error-prone (better statically type checked) and less verbose than a pure-
// Object-based alternative. Combined with the use of virtual functions, this technique handles most
// cases. In a template, a template argument T takes the place of Object and enables static type checking (§27.2).

int main()
{
    auto ship = new BetterShip{};
    std::vector<BetterShip *> ships;

    auto s = f(ship, ships);
    assert(s->m() == "ship");

    delete ship;
}
