#include <utility>
#include <cassert>

int main() {
    int a = 1;
    int b = 2;
    auto old = std::exchange(a, b);
    assert(a == 2);
    assert(old == 1);
}
