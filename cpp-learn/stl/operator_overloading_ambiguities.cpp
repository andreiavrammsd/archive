/*
 The C++ Programming Language, 4th Edition
 */

class X {
public:
    X(int) {}

    X(const char *) {}
};

class Y {
public:
    Y(int) {}
};

class Z {
public:
    Z(X) {}
};

X f(X) { return X{1}; }

Y f(Y) { return Y{1}; }

Z g(Z) { return Z{1}; }

int main() {
    // f(1); // error: ambiguous f(X(1)) or f(Y(1))?
    f(X{1}); // OK
    f(Y{1}); // OK

    // g("Mack"); // error: two user-defined conversions needed; g(Z{X{"Mack"}}) not tried
    g(X("Doc"));
    g(Z{"Suzy"});
}
