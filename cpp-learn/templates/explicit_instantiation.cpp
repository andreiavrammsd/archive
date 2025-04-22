/*
 The C++ Programming Language, 4th Edition
 */

template<typename T>
class vector {
private:
    T a, b;
public:
    vector(T a, T b) : a{a}, b{b} {}

    T sum() {
        return a + b;
    }
};

// explicit instantiation
template
class vector<int>;

template
class vector<double>;

struct S {
};

// template
// class vector<S>; // compile error because operator + is not defined for struct S

// To complement explicit instantiation requests, the language provides explicit requests not to
// instantiate (usually called extern templates). The obvious use is to have one explicit instantiation
// for a specialization and use extern templates for its use in other translation units. This mirrors the
// classical use of one definition and many declarations (§15.2.3). For example:

extern template
class vector<S>; // suppresses implicit instantiation / explicitly instantiate elsewhere

void foo(vector<S> &) {
// ... use the vector in here ...
}

int main() {

}
