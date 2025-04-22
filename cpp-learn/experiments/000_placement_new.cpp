#include <iostream>
#include <memory>

class Shape {
   public:
    virtual void draw() const = 0;
    virtual ~Shape() = default;
};

class Circle : public Shape {
   public:
    void draw() const override { std::cout << "circle\n"; }
    ~Circle() override { std::cout << "dtor\n"; }
};

class Square : public Shape {
   public:
    void draw() const override { std::cout << "square\n"; }
};

static char buffer[128];

class ShapeFactory {
   public:
    static Shape* create(int type)
    {
        switch (type) {
            case 1:
                return new (buffer) Circle();
            case 2:
                return new (buffer) Square();
            default:
                throw std::invalid_argument{"unknown shape"};
        }
    }
};

template <std::size_t Size>
class StaticMemoryAllocator {
    static char buffer_[Size];
    static std::size_t offset_;

   public:
    template <typename T>
    static auto allocate()
    {
        const auto new_offset = offset_ + sizeof(T);
        const auto place = buffer_ + new_offset - sizeof(T);
        offset_ = new_offset;
        return new (place) T{};
    }
};

template <std::size_t Size>
char StaticMemoryAllocator<Size>::buffer_[] = {};

template <std::size_t Size>
std::size_t StaticMemoryAllocator<Size>::offset_ = 0;

class StaticShapeFactory {
    using allocator = StaticMemoryAllocator<8>;

   public:
    static Shape* create(int type)
    {
        switch (type) {
            case 1:
                return allocator::allocate<Circle>();
            case 2:
                return allocator::allocate<Square>();
            default:
                throw std::invalid_argument{"unknown shape"};
        }
    }
};

int main()
{
    //    const auto circle = ShapeFactory::create(1);
    //    circle->draw();
    //
    //    const auto square = ShapeFactory::create(2);
    //    square->draw();

    const auto circle = StaticShapeFactory::create(1);

    const auto square = StaticShapeFactory::create(2);
    circle->draw();
    square->draw();
    circle->~Shape();
}
