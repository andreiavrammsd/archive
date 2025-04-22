/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <cassert>

template <typename T>
class Vector {  // general vector type
    T *v{};
    int sz{};

   public:
    Vector() = default;

    explicit Vector(int sz) : v{new T[sz]}, sz{sz} {}

    T &elem(int i) { return v[i]; }

    T &operator[](int i) { return v[i]; }
};

// The default behavior of most C++ implementations is to replicate the code for template
// functions. This is usually good for run-time performance, but unless care is taken,
// it leads to code bloat in critical cases such as the Vector example.

// Fortunately, there is an obvious solution. Containers of pointers can share a single
// implementation. This can be expressed through specialization. First, we define a version
// (a specialization) of Vector for pointers to void:

template <>
class Vector<void *> {  // complete specialization
    void **p{};
    int sz{};

   public:
    explicit Vector(int sz) : p{new void *[sz]}, sz{sz} {}

    void *&elem(int i) { return p[i]; }

    void *&operator[](int i) { return p[i]; }

    int size() const { return sz; }
};

// The Vector<void*> is a complete specialization. That is, there is no template parameter to
// specify or deduce when we use the specialization; Vector<void*> is used for Vectors declared
// like this: Vector<void*> vpv;

// To define a specialization that is used for every Vector of pointers and only for Vectors of
// pointers, we can write:

template <typename T>
class Vector<T *> : private Vector<void *> {  // partial specialization
   public:
    using Base = Vector<void *>;

    Vector() = default;

    explicit Vector(int sz) : Base(sz) {}

    T *&elem(int i) { return reinterpret_cast<T *&>(Base::elem(i)); }

    T *&operator[](int i) { return reinterpret_cast<T *&>(Base::operator[](i)); }
};

// The specialization pattern <T*> after the name says that this specialization is to be used for every
// pointer type; that is, this definition is to be used for every Vector with a template argument that can
// be expressed as T* . For example:
// Vector<Shape *> vps{2}; // <T*> is <Shape*> so T is Shape
// Vector<int**> vppi; // <T*> is <int**> so T is int*

struct Shape {
    int sz{};
};

int main()
{
    Vector<int> vi{2};
    vi[0] = 0;
    vi[1] = 1;
    assert(vi.elem(0) == 0);
    assert(vi.elem(1) == 1);

    Vector<std::string> vs{2};
    vs[0] = "a";
    vs[1] = "b";
    assert(vs.elem(0) == "a");
    assert(vs.elem(1) == "b");

    Vector<Shape *> vps{2};
    vps[0] = new Shape{};
    vps[1] = new Shape{2};
    assert(vps.elem(0)->sz == 0);
    assert(vps.elem(1)->sz == 2);

    Vector<int **> vppi{2};
    auto pi1 = new int(1);
    vppi[0] = &pi1;
    auto pi2 = new int(2);
    vppi[1] = &pi2;
    assert(vppi.elem(0) == &pi1);
    assert(**vppi.elem(1) == *pi2);
}
