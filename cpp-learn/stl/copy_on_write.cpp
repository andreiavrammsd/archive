/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <iostream>

struct Descriptor {
    int data{};
};
struct Representation {
    int data{};
};

class Image {
public:
    explicit Image(int data) : rep{new Representation{data}}, shared{false} {}

    Image(const Image &a); // copy constructor
    Image &operator=(const Image &a); // copy assignment

    void write_block(Descriptor);

    int &operator*() {
        return rep->data;
    }

private:
    Representation *clone() {
        return new Representation{};
    }

    Representation *rep;
    bool shared;
};

Image::Image(const Image &a)
        : rep{a.rep},
          shared{true} {
}

Image &Image::operator=(const Image &a) {
    if (this == &a) {
        return *this;
    }

    rep = a.rep;
    shared = true;

    return *this;
}

void Image::write_block(Descriptor d) {
    if (shared) {
        rep = clone(); // make a copy of *rep
        shared = false; // no more sharing
    }

    // ... now we can safely write to our own copy of rep ...
    rep->data = d.data;
}

void read(Image i) {
    assert(*i == 1);
}

Image write(Image i, int data) {
    i.write_block(Descriptor{data});
    assert(*i == data);
    return i;
}

int main() {
    Image a{1};
    assert(*a == 1);

    read(a);
    assert(*a == 1);

    auto b = write(a, 2);
    assert(*a == 1);
    assert(*b == 2);

    auto c = b;
    assert(*c == 2);

    auto d = write(c, 3);
    assert(*c == 2);
    assert(*d == 3);
}
