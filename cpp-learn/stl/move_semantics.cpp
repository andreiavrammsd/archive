/*
 * https://www.youtube.com/watch?v=UNJrgsQXvCA
 * https://docs.microsoft.com/en-us/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=vs-2019
 */

#include <cassert>
#include <iostream>
#include <utility>
#include <vector>
#include <cstdint>

class A {
   public:
    A() = default;

    explicit A(int i) : c{new int(i)} {}

    A(const A &a) : c{new int{*a.c}}
    {  // copy constructor
        std::cout << "copy ctor\n";
    }

    A &operator=(const A &a)
    {
        std::cout << "copy assign\n";

        if (this == &a) {
            return *this;
        }

        c = new int{};
        *c = *a.c;

        return *this;
    }

    // Define an empty constructor method that takes an rvalue reference to the class type as its parameter
    A(A &&a) noexcept : c{nullptr}
    {  // move constructor
        std::cout << "move ctor\n";

        // [...] assign the class data members from the source object to the object that is being constructed
        c = a.c;

        // Assign the data members of the source object to default values_. This prevents the destructor from
        // freeing resources (such as memory) multiple times
        a.c = nullptr;

        // One can use move instead of all of the above
        // *this = std::move(a);
    }

    // Define an empty assignment operator that takes an rvalue reference to the class type
    // as its parameter and returns a reference to the class type
    A &operator=(A &&a) noexcept
    {  // move assignment
        std::cout << "move assign\n";

        // [...] add a conditional statement that performs no operation if you try to
        // assign the object to itself
        if (this == &a) {
            return *this;
        }

        // free any resources (such as memory) from the object that is being assigned to
        delete c;

        // [...] transfer the data members from the source object to the object that
        // is being constructed
        // Copy the data pointer [...] from the source object
        c = a.c;

        // Release the data pointer from the source object so that the destructor does not
        // free the memory multiple times
        a.c = nullptr;

        return *this;
    }

    int getI() const
    {
        if (c == nullptr) {
            return 0;
        }
        return *c;
    }

    ~A() { delete c; }

   private:
    int *c;
};

// Allow user to pass a copy or move
void func(const A a) { assert(a.getI() == 2); }

uintptr_t addr;

A return_value()
{
    A a{4};
    addr = reinterpret_cast<uintptr_t>(&a);
    return a;  // will be moved
}

int main()
{
    A a{2};
    func(a);             // pass copy (copy constructor)
    func(std::move(a));  // move (move constructor)

    A b{};
    A c{1};
    assert(b.getI() != c.getI());

    b = c;  // copy assignment
    assert(b.getI() == c.getI());

    c = std::move(b);  // move assignment
    assert(c.getI() == 1);

    A e{};
    A f{1};
    e = std::move(f);  // move assignment
    assert(e.getI() == 1);

    auto returned_value = return_value();
    assert(returned_value.getI() == 4);
    assert(reinterpret_cast<uintptr_t>(&returned_value) == addr);

    std::vector<A> va{A{1}};  // copy constructor
    va.push_back(A{2});       // move constructor
}
