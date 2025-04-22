/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <vector>

class Vector {
private:
    double *elem;
    int sz;

public:
    explicit Vector(int s);

    virtual ~Vector();

    Vector(const Vector &a); // copy constructor

    Vector &operator=(const Vector &a); // copy assignment

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
    Vector a(4);

    a.push_back(1);
    a.push_back(2);
    assert(a.size() == 2);
    assert(a[0] == 1);
    assert(a[1] == 2);

    // Copy constructor
    auto b = a;
    assert(b.size() == a.size());
    assert(b[0] == 1);
    assert(b[1] == 2);

    a.push_back(3);
    assert(b.size() == 2);

    // Copy constructor
    Vector c(a);
    assert(c.size() == 3);
    assert(c[0] == 1);
    assert(c[1] == 2);
    assert(c[2] == 3);

    Vector d(5);
    // Copy assignment
    d = a;

    assert(d.size() == a.size());
    assert(d[0] == 1);
    assert(d[1] == 2);
    assert(d[2] == 3);

    a.push_back(4);
    assert(d.size() == 3);
}
