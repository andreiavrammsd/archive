/*
 https://en.wikipedia.org/wiki/Template_metaprogramming
 */

#include <cstdlib>
#include <cassert>

template<int length>
class Vector {
public:
    Vector() = default;

    Vector<length> &operator+=(const Vector<length> &);

    int &operator[](size_t);

private:
    int value[length];
};

template<int length>
Vector<length> &Vector<length>::operator+=(const Vector<length> &rhs) {
    for (int i = 0; i < length; ++i)
        value[i] += rhs.value[i];
    return *this;
}

/**
 * When the compiler instantiates the function template defined above, the following code may be produced:

    template <>
    Vector<2>& Vector<2>::operator+=(const Vector<2>& rhs)
    {
        value[0] += rhs.value[0];
        value[1] += rhs.value[1];
        return *this;
    }
 */

template<int length>
int &Vector<length>::operator[](size_t i) {
    return value[i];
}

int main() {
    Vector<2> a{};
    a[0] = 1;
    a[1] = 2;

    Vector<2> b{};
    b[0] = 3;
    b[1] = 4;

    a += b;
    assert(a[0] == 4);
    assert(a[1] == 6);
}
