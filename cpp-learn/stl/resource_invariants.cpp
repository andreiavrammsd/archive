/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <vector>

template<class T>
class Handle {
    T *p;
public:
    explicit Handle(T *pp) : p{pp} {}

    T &operator*() { return *p; }

    ~Handle() { delete p; }

    Handle(const Handle &) = delete;
};

template<class T>
class HandleCopy {
    T *p;
public:
    explicit HandleCopy(T *pp) : p{pp} {}

    T &operator*() { return *p; }

    ~HandleCopy() { delete p; }

    HandleCopy(const HandleCopy &h) : p{new T(*h.p)} {} // clone

    HandleCopy &operator=(const HandleCopy &h) {
        if (this == &h) {
            return *this;
        }

        p = new T(*h.p);

        return *this;
    }
};

// Partially Specified Invariants
class Tic_tac_toe {
public:
    Tic_tac_toe() : pos(9) {} // always 9 positions

    Tic_tac_toe(const Tic_tac_toe &) = default;

    Tic_tac_toe &operator=(const Tic_tac_toe &) = default;

    ~Tic_tac_toe() = default;

    enum State {
        empty, nought, cross
    };

    size_t size() const {
        return pos.size();
    }

private:
    std::vector<State> pos;
};

// vs

class Tic_tac_toe_2 {
public:

    enum State {
        empty, nought, cross
    };

    size_t size() const {
        return pos.size();
    }

private:
    std::vector<State> pos{std::vector<State>(9)}; // always 9 positions
};

int main() {
    // The absence of a default constructor saves us from the possibility of a delete with a random memory address.
    // Handle<int> h; // error : no default constructor

    Handle<int> h1{new int(7)};
    assert(*h1 == 7);

    // Handle<int> h2{h1}; // error: no copy constructor
    // Had Handle had a default copy constructor, both h1 and h2 would have had a copy of the pointer
    // and both would have deleted it.

    // Copy construction
    HandleCopy<int> hc1{new int(7)};
    assert(*hc1 == 7);

    HandleCopy<int> hc2{hc1};
    assert(*hc2 == 7);

    (*hc1)++;
    assert(*hc1 == 8);
    assert(*hc2 == 7);

    // Copy assignment
    HandleCopy<int> hc3 = hc1;
    assert(*hc3 == 8);

    (*hc1)++;
    assert(*hc1 == 9);
    assert(*hc3 == 8);
}
