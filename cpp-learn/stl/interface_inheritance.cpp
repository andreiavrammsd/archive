/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <iostream>

class lval_box {
public:
    virtual int get_value() = 0;

    virtual void set_value(int) = 0;

    virtual void prompt() = 0;

    virtual ~lval_box() = default;
};

class BBwidget {
public:
    virtual void draw() {
    }
};

class lval_slider : public lval_box, protected BBwidget {
public:
    lval_slider(int a, int b) : a{a}, b{b} {};

    ~lval_slider() override = default;

    int get_value() override {
        return val;
    }

    void set_value(int v) override {
        val = v;
    }

    void prompt() override {
        draw();
    }

protected:
    void draw() override {
        std::cout << a << ',' << b << '\n';
        BBwidget::draw();
    }

private:
    int a, b, val{};
};

int main() {
    lval_slider ls{0, 100};
    ls.prompt();
    ls.set_value(5);
    assert(ls.get_value() == 5);
}
