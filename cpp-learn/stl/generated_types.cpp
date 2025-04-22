/*
 The C++ Programming Language, 4th Edition
 */

#include <set>
#include <iostream>
#include <cassert>
#include <csignal>

class Shape {
   public:
    virtual void draw() {}
};

struct Point {
    int x{}, y{};
};

class Circle : public Shape {
    Point center;
    unsigned radius{};

   public:
    void draw() override { std::cout << center.x << ',' << radius << '\n'; }
};

class Triangle : public Shape {
};

void f(std::set<Shape *> &s)
{
    s.insert(new Triangle{});
    s.insert(new Shape);
}

void g(std::set<Circle *> &)
{
    // f(s); // error, type mismatch: s is a set<Circle*>, not a set<Shape*>
}

void maul(Shape *p, int n)
{  // Danger!
    for (int i = 0; i != n; ++i) {
        p[i].draw();  // looks innocent; it is not
    }
}

void user()
{
    Circle image[10];  // an image is composed of 10 Circles
    maul(image, 10);   // "maul" 10 Circles
}

int main()
{
    // [...] image's type is converted (decays) from Circle[] to Circle*.
    // Next, Circle* is converted to Shape*.

    // [...] assume that a Shape is an abstract class with size 4 and that Circle
    // adds a center and a radius. Then sizeof(Circle)>sizeof(Shape).

    // When maul() tries to invoke a virtual function on p[1], there is no virtual function
    // pointer where it is expected and the call hopefully fails instantly.

    // Prefer containers over built-in arrays.
    // Consider interfaces such as void f(T* p, int count) highly suspect; when T can be a base class
    // and count is an element count, trouble awaits.
    // Consider . (dot) suspect when applied to something that is supposed to be run-time polymorphic
    // unless it is obviously applied to a reference.

    assert(sizeof(Circle) > sizeof(Shape));

    signal(SIGSEGV, [](int sig) {
        std::cerr << "caught signal: " << sig << std::endl;
        exit(0);
    });

    user();
    assert(false);
}
