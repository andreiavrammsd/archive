/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <cassert>

class Shape {
};

class Circle : public Shape {
};

class Square {
};

template <typename T>
void draw_all(std::vector<T *> &v)
{
    static_assert(std::is_base_of<Shape, T>(), "non-Shape type for draw_all()");

    for (auto p : v) {
        assert(p != nullptr);
    }
}

int main()
{
    auto circles = std::vector<Circle *>{
        new Circle,
        new Circle,
        new Circle,
    };
    draw_all(circles);

    auto squares = std::vector<Square *>{
        new Square,
        new Square,
        new Square,
    };
    // draw_all(squares);
}
