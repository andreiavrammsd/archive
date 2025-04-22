/*
 The C++ Programming Language, 4th Edition
 */

#include <memory>
#include <cassert>
#include <iostream>
#include <vector>

/**
 * Resource Management Pointers
 *
 * unique_ptr
 * shared_ptr
 * weak_ptr
 */

template<typename T>
void unique_ptr_cmp(const std::unique_ptr<T> &up, T cmp);

template<typename T>
void unique_ptr_cmp(std::unique_ptr<T> &&up, T cmp);

std::unique_ptr<int[]> make_sequence(int n);

class Base {
public:
    void m() {}
};

class Derived : public Base {
};

std::unique_ptr<int> f(std::unique_ptr<int> p);

void f2(const std::unique_ptr<int> &p);

int main() {
    /**
     * unique_ptr
     *
     * provides a semantics of strict ownership
     *
     * Note: unique_ptr does not offer a copy constructor or copy assignment. Had it done so,
     * the meaning of "ownership" would have been very hard to define and/or use.
     */
    // construct
    std::unique_ptr<std::string> up{};
    assert(up == nullptr);

    int *pi = new int(1);
    std::unique_ptr<int> upi(pi);
    assert(*upi == 1);

    // reset
    up.reset(new std::string("A"));
    assert(*up == "A");

    // make_unique
    up = std::make_unique<std::string>("B");
    assert(*up == "B");

    // swap
    auto up2 = std::make_unique<std::string>("C");
    up.swap(up2);
    assert(*up == "C");
    assert(*up2 == "B");

    // release
    auto up3 = up.release();
    assert(up == nullptr);
    assert(*up3 == "C");
    delete up3;

    // operations
    assert(up != up2);
    assert(up <= up2);

    // pass and return
    unique_ptr_cmp(up2, std::string{"B"});
    unique_ptr_cmp(move(up2), std::string{"B"});

    auto seq = make_sequence(5);
    for (int i = 0; i < 5; i++) {
        std::cout << seq[i] << ", ";
    }
    std::cout << '\n';

    // access
    std::unique_ptr<Base> sb{new Derived};
    sb->m();

    // To avoid slicing (§17.5.1.4), a Derived[] is not accepted as an argument
    // to a unique_ptr<Base[]> even if Base is a public base of Derived.

    std::unique_ptr<Base> b{new Derived}; // OK
    std::unique_ptr<Base[]> bs{new Base[2]{Base{}, Base{}}}; // OK
    // std::unique_ptr<Base[]> bsd{new Derived[2]{Derived{}, Derived{}}}; // error

    // The f2() body is slightly shorter than f() and f2() is simpler to call, but I find f() easier to think about.
    // The style illustrated by f() is explicit about ownership (and the use of unique_ptr is typically motivated
    // by ownership issues). See also the discussion of the use of non-const references in §7.7.1.
    // On balance, a notation f(x) that modifies x is more error-prone than a y=f(x) notation that does not.
    // It is a fair estimate that the call of f2() is one or two machine instructions faster than a call of f()
    // (because of the need to place a nullptr in the original unique_ptr), but that is unlikely to be
    // significant. On the other hand, access to the contained pointer involves an extra indirection in f2()
    // compared to f(). This, too, is unlikely to be significant in most programs, so the choice between the
    // styles of f() and f2() has to be made on reasoning about code quality.

    std::unique_ptr<int> p{new int{7}};
    // p = f(p); // error: no copy constructor
    p = f(move(p)); // transfer ownership there and back
    assert(*p == 8);
    f2(p); // pass a reference
    assert(*p == 9);

    // non-const vs const pointer (https://www.fluentcpp.com/2018/12/25/free-ebook-smart-pointers/)
    std::unique_ptr<int> ncp{new int{7}};
    assert(*ncp == 7);
    *ncp = 8;
    assert(*ncp == 8);

    ncp = std::make_unique<int>(9);
    assert(*ncp == 9);

    const std::unique_ptr<const int> cp{new int{7}};
    assert(*cp == 7);
    // *cp = 8; // error: const value
    // cp = std::make_unique<int>(9); // error: const pointer

    /**
     * shared_ptr
     *
     * represents shared ownership
     *
     * Prefer unique_ptr to shared_ptr.
     * Prefer ordinary scoped objects to objects on the heap owned by a unique_ptr.
     */
    // construct
    std::shared_ptr<std::string> sp{};
    assert(up == nullptr);

    int *si = new int(1);
    std::shared_ptr<int> spi(si);
    assert(*spi == 1);

    // reset
    sp.reset(new std::string("A"));
    assert(*sp == "A");

    // make_shared
    sp = std::make_shared<std::string>("B");
    assert(*sp == "B");

    // swap
    auto sp2 = std::make_shared<std::string>("C");
    sp.swap(sp2);
    assert(*sp == "C");
    assert(*sp2 == "B");

    // use_count
    assert(sp.use_count() == 1);
    {
        auto spc = sp;
        assert(sp.use_count() == 2);
        assert(spc.use_count() == 2);
    }
    assert(sp.use_count() == 1);

    // unique: sp.use_count() == 1 (does not check if nullptr)
    assert(sp.unique());

    // operations
    assert(sp != sp2);
    assert(sp <= sp);

    // access
    std::shared_ptr<Base> sba{new Derived};
    sba->m();

    // cast: static_pointer_cast, dynamic_pointer_cast, const_pointer_cast
    std::shared_ptr<Base> ca;
    std::shared_ptr<Derived> cb;

    ca = std::make_shared<Base>();
    cb = std::static_pointer_cast<Derived>(ca);
    cb->m();

    // aliasing constructor
    // https://www.nextptr.com/tutorial/ta1358374985/shared_ptr-basics-and-internals-with-examples
    struct Yolk {
    };
    struct White {
    };
    struct Egg {
        White w;
        Yolk y;
    };

    auto ep = std::make_shared<Egg>();
    auto yp = std::shared_ptr<Yolk>(ep, &ep->y);

    // custom deleter
    {
        std::shared_ptr<Yolk>(new Yolk(), [](Yolk *p) {
            std::cout << "custom deleter used\n";
            delete p;
        });

        std::shared_ptr<Yolk>(new Yolk(), [](Yolk *p) {
            std::cout << "custom deleter used\n";
            auto deleter = std::default_delete<Yolk>();
            deleter(p);
        });
    }

    /**
     * weak_ptr
     *
     * A weak_ptr refers to an object managed by a shared_ptr. To access the object, a weak_ptr can be
     * converted to a shared_ptr using the member function lock(). A weak_ptr allows access to an object,
     * owned by someone else, that
     * * You need access to (only) if it exists
     * * May get deleted (by someone else) at any time
     * * Must have its destructor called after its last use (usually to delete a non-memory resource)
     *
     * In particular, we use weak pointers to break loops in data structures managed using shared_ptrs.
     */
    // construct
    std::weak_ptr<std::string> wp{};
    assert(wp.lock() == nullptr);

    auto wpsi = std::make_shared<int>(1);
    std::weak_ptr<int> wpi(wpsi);
    assert(*wpi.lock() == 1);

    // reset
    auto pss = std::make_shared<std::string>("1");
    std::weak_ptr<std::string> wpss(pss);
    assert(wpss.lock() != nullptr);
    wpss.reset();
    assert(wpss.lock() == nullptr);

    // swap
    auto wps = std::make_shared<std::string>("XYZ");
    std::weak_ptr<std::string> wp2(wps);

    std::weak_ptr<std::string> wp3;

    wp3.swap(wp2);
    assert(*wp3.lock() == "XYZ");
    assert(wp3.lock()->length() == 3);
    assert(wp2.lock() == nullptr);

    // expired
    assert(!wpi.expired());

    // use_count
    assert(wpi.use_count() == 1);

    // example

    // Consider an implementation of the old "asteroid game." All asteroids are owned by "the game,"
    // but each asteroid must keep track of neighboring asteroids and handle collisions.
    //
    // A collision typically leads to the destruction of one or more asteroids.
    //
    // Each asteroid must keep a list of other asteroids in its neighborhood.
    // Note that being on such a neighbor list should not keep an asteroid "alive"
    // (so a shared_ptr would be inappropriate).
    //
    // On the other hand, an asteroid must not be destroyed while another asteroid is looking at it
    // (e.g., to calculate the effect of a collision). And obviously, an asteroid destructor must be
    // called to release resources (such as a connection to the graphics system).
    //
    // What we need is a list of asteroids that might still be intact and a way of "grabbing onto one"
    // for a while. A weak_ptr does just that:
    /**
        class Asteroid {
        public:
            explicit Asteroid(const std::weak_ptr<Asteroid> &a) : neighbor{a} {}

        private:
            const std::weak_ptr<Asteroid> &neighbor;
        };

        void owner() {
            std::vector<std::shared_ptr<Asteroid>> va(100);
            for (int i = 0; i < va.size(); ++i) {
                // ... calculate neighbors for new asteroid ...
                int neighbor = 5;
                va[i].reset(new Asteroid(std::weak_ptr<Asteroid>(va[neighbor])));
                // launch(i);
            }
        }

        void collision(std::weak_ptr<Asteroid> &p) {
            if (auto q = p.lock()) {

            } else {
                p.reset();
            }
        }
     */

    // https://www.fluentcpp.com/2018/12/25/free-ebook-smart-pointers/

    // memory leak because of circular reference
    struct House {
        std::shared_ptr<House> neighbour;
    };

    std::shared_ptr<House> house1 = std::make_shared<House>();
    std::shared_ptr<House> house2 = std::make_shared<House>();
    house1->neighbour = house2;
    house2->neighbour = house1;

    // no memory leak with weak_ptr
    struct House_weak {
        std::weak_ptr<House_weak> neighbour;
    };

    std::shared_ptr<House_weak> house1w = std::make_shared<House_weak>();
    std::shared_ptr<House_weak> house2w = std::make_shared<House_weak>();
    house1w->neighbour = house2w;
    house2w->neighbour = house1w;
}

template<typename T>
void unique_ptr_cmp(const std::unique_ptr<T> &up, T cmp) {
    assert(up != nullptr);
    assert(*up == cmp);
}

template<typename T>
void unique_ptr_cmp(std::unique_ptr<T> &&up, T cmp) {
    assert(up != nullptr);
    assert(*up == cmp);
}

std::unique_ptr<int[]> make_sequence(int n) {
    std::unique_ptr<int[]> p{new int[n]};

    for (int i = 0; i < n; ++i) {
        p[i] = i;
    }

    return p;
}

std::unique_ptr<int> f(std::unique_ptr<int> p) {
    ++*p;
    return p;
}

void f2(const std::unique_ptr<int> &p) {
    ++*p;
}
