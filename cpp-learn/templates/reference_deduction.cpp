/*
 The C++ Programming Language, 4th Edition
 */

#include <utility>
#include <string>

template<typename T>
class Xref {
public:
    Xref(int i, T *p) // store a pointer: Xref is the owner
            : index{i}, elem{p}, owned{true} {}

    Xref(int i, T &r) // store a pointer to r, owned by someone else
            : index{i}, elem{&r}, owned{false} {}

    Xref(int i, T &&r)
            : index{i}, elem{new T{std::move(r)}}, owned{true} {}

    ~Xref() {
        if (owned) {
            delete elem;
        }
    }

private:
    int index;
    T *elem;
    bool owned;
};

int main() {
    std::string x{"There and back again"};

    Xref<std::string> r1{7, "Here"}; // r1 owns a copy of string{"Here"}
    Xref<std::string> r2{9, x}; // r2 just refers to x
    Xref<std::string> r3{3, new std::string{"There"}}; // r3 owns the string{"There"}
}
