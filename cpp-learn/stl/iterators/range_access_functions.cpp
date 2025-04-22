/*
 The C++ Programming Language, 4th Edition
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP20.md#range-based-for-loop-with-initializer
 */

#include <vector>
#include <cassert>
#include <iostream>
#include <array>

// A user-defined container with member begin() and end() automatically gets nonmember versions
// when <iterator> is #included. To provide nonmember begin() and end() for a container, My_container,
// that doesn't have them [...]:

// https://stackoverflow.com/a/7599385/7383762

template<typename T>
class My_container {
public:
    explicit My_container(size_t sz) : sz{sz}, data{new T[sz]} {}

    My_container(std::initializer_list<T> list) : sz{list.size()}, data{new T[sz]} {
        std::copy(list.begin(), list.end(), &data[0]);
    }

    size_t size() {
        return sz;
    }

    T &operator[](size_t n) {
        return data[n];
    }

    T at(size_t n) {
        return data[n];
    }

    virtual ~My_container() {
        delete[] data;
    }

private:
    size_t sz;
    T *data;
};

template<typename T>
class Iterator {
public:
    Iterator(My_container<T> *data, size_t i) : index{i}, data{data} {}

    Iterator operator++() {
        ++index;
        return *this;
    }

    Iterator operator--();

    Iterator operator++(int);

    Iterator operator--(int);

    T operator*() {
        return data->at(index);
    }

    bool operator==(Iterator &);

    bool operator!=(Iterator &it) {
        return index != it.index;
    }

private:
    size_t index;
    My_container<T> *data;
};

template<typename T>
Iterator<T> begin(My_container<T> &arr) {
    return {&arr, 0};
}

template<typename T>
Iterator<T> end(My_container<T> &arr) {
    return {&arr, arr.size()};
}


int main() {
    std::vector<int> v{1, 2, 3};

    assert(std::begin(v) == v.begin());
    assert(std::end(v) == v.end());

    assert(std::cbegin(v) == v.cbegin());
    assert(std::cend(v) == v.cend());

    assert(std::rbegin(v) == v.rbegin());
    assert(std::rend(v) == v.rend());

    assert(std::crbegin(v) == v.crbegin());
    assert(std::crend(v) == v.crend());

    My_container<std::string> c({"1", "2"});
    c[0] = "3";
    for (auto elem : c) {
        std::cout << elem;
    }

    // Range-based for loop with initializer
    auto s = 0;
    for (auto numbers = std::array{-1, 0, 1, 1}; auto n: numbers) {
        s += n;
    }
    assert(s == 1);
}
