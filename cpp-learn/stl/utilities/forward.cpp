/*
 * https://cpppatterns.com/patterns/perfect-forwarding.html
 */

#include <iostream>

class X {
};

template<typename T>
void f(T &) {
    std::cout << "lvalue" << std::endl;
}

template<typename T>
void f(T &&) {
    std::cout << "rvalue" << std::endl;
}

template<typename T>
void func(T &&in) {
    f(std::forward<T>(in)); // Perfect forwarding
    f(in);
}

int main() {
    /**
     * Perfect forwarding
     *
     * Forward arguments of one function to another as though the wrapped function
     * had been called directly.
     *
     * Perfect forwarding allows us to preserve an argument's value category (lvalue/rvalue)
     * and const/volatile modifiers.
     *
     * Perfect forwarding is performed in two steps:
     *      receive a forwarding reference (also known as universal reference),
     *      then forward it using std::forward.
     */

    std::cout << "Object:\n";
    X x;
    func(x);

    std::cout << "\nTemporary object:\n";
    func(X());
}
