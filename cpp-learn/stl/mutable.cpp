#include <cassert>

class X {
    int val{};
    mutable int counter{}; // can be changed by const getValue()
public:
    explicit X(int v) : val{v} {}

    int getValue() const {
        ++counter;
        return val;
    }

    int getCounter() const {
        return counter;
    }
};

int main() {
    // Mutable data member
    X x(2);
    assert(x.getCounter() == 0);
    assert(x.getValue() == 2);
    assert(x.getCounter() == 1);

    // Mutable lambda
    auto l = [counter = 1]() mutable {
        ++counter;
        return counter;
    };
    assert(l() == 2);
}
