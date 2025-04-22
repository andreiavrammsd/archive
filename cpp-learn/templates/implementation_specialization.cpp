/*
 The C++ Programming Language, 4th Edition
 */

// Specialization can be used to provide alternative implementations of a class template for a specific
// set of template parameters. In that case, a specialization can even provide a representation that
// differs from that of the general template.

#include <cassert>

template<typename T, int N>
class Matrix {
public:
    inline constexpr int size() const {
        return N;
    }
}; // N-dimensional Matrix of Ts

template<typename T>
class Matrix<T, 0> { // specialization for N==0
    T val{};
public:
    inline constexpr int size() const {
        return 0;
    }
};

template<typename T>
class Matrix<T, 1> { // specialization for N==1
    T *elem{};
    int sz = 1; // number of elements
public:
    inline constexpr int size() const {
        return 1;
    }
};

template<typename T>
class Matrix<T, 2> {
    // specialization for N==2
    T *elem{};
    int dim1 = 2; // number of rows
    int dim2 = 2; // number of columns
public:
    inline constexpr int size() const {
        return 2;
    }
};

int main() {
    Matrix<int, 5> m5;
    assert(m5.size() == 5);

    Matrix<int, 0> m0;
    assert(m0.size() == 0);

    Matrix<int, 1> m1;
    assert(m1.size() == 1);

    Matrix<int, 2> m2;
    assert(m2.size() == 2);
}
