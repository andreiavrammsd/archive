/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <vector>
#include <iostream>
#include <initializer_list>

class Vector {
private:
    double *elem;
    int sz;

public:
    explicit Vector(int s);

    explicit Vector(std::initializer_list<double> l) {
        sz = l.size();
        elem = new double[sz];
        int i = 0;
        for (auto const &el : l) {
            elem[i] = el;
        }
    }

    virtual ~Vector();

    Vector(const Vector &a); // copy constructor

    Vector &operator=(const Vector &a); // copy assignment

    Vector(Vector &&a); // move constructor

    Vector &operator=(Vector &&a) noexcept; // move assignment

    void push_back(double el);

    double &operator[](int i);

    const double &operator[](int i) const;

    int size() const;
};

Vector::Vector(int s) : elem{new double[s]}, sz{0} {}

Vector::~Vector() {
    delete[] elem;
}

Vector::Vector(const Vector &a) // copy constructor
        : elem{new double[a.sz]}, // allocate space for elements
          sz{a.sz} {
    for (int i = 0; i != sz; ++i) {
        elem[i] = a.elem[i]; // copy elements
    }
}

Vector &Vector::operator=(const Vector &a) { // copy assignment
    if (this == &a) {
        return *this;
    }

    auto *p = new double[a.sz];
    for (int i = 0; i != a.sz; ++i) {
        p[i] = a.elem[i];
    }
    delete[] elem; // delete old elements
    elem = p;
    sz = a.sz;
    return *this;
}

Vector::Vector(Vector &&a) // move constructor
        : elem(a.elem), // "grab the elements" form a
          sz{a.sz} {
    a.elem = nullptr; // now a has no elements
    a.sz = 0;
}

Vector &Vector::operator=(Vector &&a) noexcept { // move assignment
    if (this != &a) {
        delete[] elem;

        elem = a.elem;
        sz = a.sz;

        a.elem = nullptr;
        a.sz = 0;
    }
    return *this;
}

void Vector::push_back(double el) {
    elem[sz] = el;
    sz++;
}

double &Vector::operator[](int i) {
    return elem[i];
}

const double &Vector::operator[](int i) const {
    return elem[i];
}

int Vector::size() const {
    return sz;
}

int main() {
    // Move constructor
    std::vector<Vector> vector;
    vector.push_back(Vector({2, 5, 6}));

    Vector a(2);
    a.push_back(1);
    a.push_back(2);

    assert(a.size() == 2);
    assert(a[0] == 1);
    assert(a[1] == 2);

    // Move assignment
    Vector b(1);
    b.push_back(33);

    b = std::move(a);

    assert(b.size() == 2);
    assert(b[0] == 1);
    assert(b[1] == 2);
}
