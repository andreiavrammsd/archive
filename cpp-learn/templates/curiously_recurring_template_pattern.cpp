/*
 https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern
 */

#include <iostream>
#include <cassert>

/**
 * The curiously recurring template pattern (CRTP) is an idiom in C++ in which a class X
 * derives from a class template instantiation using X itself as template argument.
 */
template<typename T>
class Base {
    // methods within Base can use template to access members of Derived
};

class Derived : public Base<Derived> {

};

/**
 * Static polymorphism
 */
template<class T>
struct PolyBase {
    void interface() {
        static_cast<T *>(this)->implementation();
    }

    static void static_func() {
        T::static_sub_func();
    }
};

struct PolyDerived : PolyBase<PolyDerived> {
    void implementation() {
        std::cout << "Derived implementation()\n";
    }

    static void static_sub_func() {
        std::cout << "Derived static_sub_func()\n";
    }
};

/**
 * Object counter
 */
template<typename T>
struct counter {
    static int objects_created;
    static int objects_alive;

    counter() {
        ++objects_created;
        ++objects_alive;
    }

    counter(const counter &) {
        ++objects_created;
        ++objects_alive;
    }

protected:
    ~counter() {
        --objects_alive;
    }
};

template<typename T> int counter<T>::objects_created{0};
template<typename T> int counter<T>::objects_alive{0};

/**
 * Polymorphic chaining
 */
class PrinterBefore {
public:
    PrinterBefore(std::ostream &pstream) : m_stream(pstream) {}

    template<typename T>
    PrinterBefore &print(T &&t) {
        m_stream << t;
        return *this;
    }

    template<typename T>
    PrinterBefore &println(T &&t) {
        m_stream << t << std::endl;
        return *this;
    }

private:
    std::ostream &m_stream;
};

enum class Color {
    red
};

class CoutPrinterBefore : public PrinterBefore {
public:
    CoutPrinterBefore() : PrinterBefore(std::cout) {}

    CoutPrinterBefore &SetConsoleColor(Color) {
        // ...
        return *this;
    }
};

template<typename ConcretePrinter>
class Printer {
public:
    Printer(std::ostream &pstream) : m_stream(pstream) {}

    template<typename T>
    ConcretePrinter &print(T &&t) {
        m_stream << t;
        return static_cast<ConcretePrinter &>(*this);
    }

    template<typename T>
    ConcretePrinter &println(T &&t) {
        m_stream << t << std::endl;
        return static_cast<ConcretePrinter &>(*this);
    }

private:
    std::ostream &m_stream;
};

class CoutPrinter : public Printer<CoutPrinter> {
public:
    CoutPrinter() : Printer(std::cout) {}

    CoutPrinter &SetConsoleColor(Color) {
        // ...
        return *this;
    }
};

int main() {
    /**
     * Static polymorphism
     */

    PolyBase<PolyDerived> poly;
    poly.interface();

    PolyBase<PolyDerived>::static_func();

    /**
     * Object counter
     */

    struct X : counter<X> {
    };
    struct Y : counter<Y> {
    };
    assert(X::objects_created == 0);
    assert(X::objects_alive == 0);
    assert(Y::objects_created == 0);
    assert(Y::objects_alive == 0);

    X x;
    Y y;
    assert(X::objects_created == 1);
    assert(X::objects_alive == 1);
    assert(Y::objects_created == 1);
    assert(Y::objects_alive == 1);

    auto x2 = x;
    auto y2 = y;
    assert(X::objects_created == 2);
    assert(X::objects_alive == 2);
    assert(Y::objects_created == 2);
    assert(Y::objects_alive == 2);

    {
        X x3;
        assert(X::objects_created == 3);
        assert(X::objects_alive == 3);
    }

    assert(X::objects_created == 3);
    assert(X::objects_alive == 2);

    /**
     * Polymorphic chaining
     */

    PrinterBefore{std::cout}.print("hello").println(500);
    // CoutPrinterBefore().print("Hello ").SetConsoleColor(Color::red).println("Printer!"); // No member named 'SetConsoleColor' in 'PrinterBefore'

    CoutPrinter().print("Hello ").SetConsoleColor(Color::red).println(" Printer");
}
