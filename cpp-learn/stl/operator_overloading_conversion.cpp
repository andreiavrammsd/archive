/*
 The C++ Programming Language, 4th Edition
 */

#include <memory>
#include <cassert>

template<typename T, typename D = std::default_delete<T>>
class unique_ptr {
    T *ptr;
public:
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
};

struct Record {
};

class Invalid_unique_ptr : public std::exception {
};

void use(unique_ptr<Record> p, unique_ptr<int>) {
    if (!p) { // OK: we want this use
        throw Invalid_unique_ptr{};
    }

    // bool b = p; // error; suspicious use
    // int x = p + q; // error; we definitely don't want this

    //    Had unique_ptr's conversion to bool not been explicit, the last two definitions would have compiled.
    //    The value of b would have become true and the value of x would have become 1 or 2 (depending on
    //    whether q was valid or not).
}

int main() {
    unique_ptr<Record> record{};

    try {
        use(record, unique_ptr<int>{});
        assert(false);
    } catch (const Invalid_unique_ptr &) {
        static_assert(true, "exception caught");
    }
}
