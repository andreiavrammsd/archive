/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <stdexcept>
#include <iostream>

template<typename T>
class Vector : public std::vector<T> {
public:
    using std::vector<T>::vector; // use the constructors from vector (under the name Vector)

    T &operator[](int i) { // range check
        return std::vector<T>::at(i);
    }

    const T &operator[](int i) const { // range check const objects
        return std::vector<T>::at(i);
    }
};

int main() {
    Vector<int> vector = {1, 2, 3};

    try {
        vector[3];
    } catch (const std::out_of_range &) {
        std::cout << "out of range" << std::endl;
    }
}
