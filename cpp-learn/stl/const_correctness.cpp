/*
 * https://isocpp.org/wiki/faq/const-correctness
 */

#include <string>
#include <cassert>

class X {
   public:
    int v{};
    bool read = false;
    std::string *name;

    X() : name{new std::string{}} {}

    // Member functions with a const suffix are called "const member functions" or "inspectors."
    [[nodiscard]] int getV() const
    {
        // read = true; // Cannot assign to non-static data member within const member function 'getV'
        return v;
    }

    //  Member functions without a const suffix are called "non-const member functions" or "mutators."
    void setV(int val)
    {
        read = true;
        v = val;
    }

    [[nodiscard]] const std::string &getName() const { return *name; }

    [[nodiscard]] std::string &getNameNonConst() const { return *name; }
};

void ptr_mut_value(X *x) { x->v = 2; }

void ptr_mut_object(X *x)
{
    x = new X();
    x->v = 3;
}

void ptr_const_ptr(const X *x)
{
    assert(x != nullptr);

    x = new (X);
    // x->v = 4; // Cannot assign to variable 'x' with const-qualified type 'const X *'
}

void ptr_const_value(X *const x)
{
    // x = new (X); // Cannot assign to variable 'x' with const-qualified type 'X *const'
    x->v = 5;
}

void ptr_const_ptr_and_value(const X *const x)
{
    assert(x != nullptr);  // can only ready x

    // x = new (X); // Cannot assign to variable 'x' with const-qualified type 'X *const'
    // x->v = 5; // Cannot assign to variable 'x' with const-qualified type 'const X *const'
}

void ref_mut(X &x) { x.v = 6; }

void ref_const(const X &x)
{
    assert(x.v > 0);

    // x.v = 7; // Cannot assign to variable 'x' with const-qualified type 'const X &'
}

void ref_const2(X const &x)
{  // same as ref_const()
    assert(x.v > 0);

    // x.v = 8; // Cannot assign to variable 'x' with const-qualified type 'const X &'
}

class Vector {
   private:
    int *data;
    mutable size_t lastReadPosition{};  // change from const method
    size_t lastGetPosition{};           // change from const method

   public:
    explicit Vector(size_t size) : data{new int[size]} {}

    const int &operator[](size_t i) const
    {
        lastReadPosition = i;
        return data[i];
    }

    int &operator[](size_t i) { return data[i]; }

    int get(size_t i) const
    {
        // Use mutable (or, as a last resort, use const_cast).
        Vector *self = const_cast<Vector *>(this);
        self->lastGetPosition = i;

        return data[i];
    }

    size_t getLastReadPosition() const { return lastReadPosition; }

    size_t getLastGetPosition() const { return lastGetPosition; }

    virtual ~Vector() { delete[] data; }
};

void write(Vector &vec, size_t idx, int value) { vec[idx] = value; }

int read(const Vector &vec) { return vec[0] + vec[1]; }

void ia(const int **) {}

void ib(const int *const *) {}

int main()
{
    X *a = new X;
    a->v = 1;

    ptr_mut_value(a);
    assert(a->v == 2);

    ptr_mut_object(a);
    assert(a->v == 2);

    ptr_const_ptr(a);
    assert(a->v == 2);

    ptr_const_value(a);
    assert(a->v == 5);

    ptr_const_ptr_and_value(a);
    assert(a->v == 5);

    ref_mut(*a);
    assert(a->v == 6);

    ref_const(*a);
    assert(a->v == 6);

    ref_const2(*a);
    assert(a->v == 6);

    a->setV(9);
    assert(a->getV() == 9);

    assert(a->getName().empty());
    // a->getName() = "name"; // const
    a->getNameNonConst() = "name";
    assert(a->getName() == "name");

    // a->getName(); // error: [[nodiscard]]

    Vector numbers(2);
    write(numbers, 0, 1);
    write(numbers, 1, 2);
    assert(read(numbers) == 3);
    assert(numbers.getLastReadPosition() == 1);

    numbers.get(0);
    assert(numbers.getLastReadPosition() == 1);
    assert(numbers.getLastGetPosition() == 0);

    int **i = reinterpret_cast<int **>(1);
    // ia(i); // error
    ib(i);
}
