#include <array>
#include <cassert>
#include <functional>
#include <utility>

struct Object {
    int id{};
    int value{};
};

template <typename T, typename Function>
struct Callable : T {
    explicit Callable(Function func) : func_{std::move(func)} {}

    template <typename... Args>
    void operator()(Args&&... args)
    {
        func_(static_cast<T&>(*this), std::forward<Args>(args)...);
    }

   private:
    std::function<Function> func_;
};

int main()
{
    using MyObject = Callable<Object, void(Object&, int)>;

    std::array<MyObject, 2> objects = {
        MyObject{[](Object& object, int i) {
            object.id = i;
            object.value = i;
        }},
        MyObject{[](Object& object, int i) {
            object.id = i;
            object.value = i * 10;
        }},
    };

    int inc = 0;
    for (auto& object : objects) {
        ++inc;
        object(inc);
    }

    assert(objects[0].id == 1);
    assert(objects[0].value == 1);

    assert(objects[1].id == 2);
    assert(objects[1].value == 20);
}
