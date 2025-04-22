/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <future>

/**
 * https://www.boost.org/community/exception_safety.html
 *
    The basic guarantee: the invariants of the component are preserved, and no resources are leaked.

    The strong guarantee: the operation has either completed successfully or thrown an exception,
    leaving the program state exactly as it was before the operation started.

    The no-throw guarantee: the operation will not throw an exception.
 */

class XError : public std::exception {
private:
    std::string message;
public:
    explicit XError(std::string message) : message(std::move(message)) {}

    const char *what() const noexcept override {
        return message.c_str();
    }
};

class X {
    std::vector<int> vi;
    std::vector<std::string> vs;
public:
    X(int, int);
};

X::X(int sz1, int sz2)
        : vi(sz1),
          vs(sz2) {
    if (sz1 > sz2) {
        throw XError{"err"};
    }
}

int main() {
    // Plain old data
    try {
        throw 1;
    } catch (int e) {
        std::cout << "caught: " << e << std::endl;
    }

    // Catch all (multiple catch)
    try {
        throw "";
    } catch (int e) {
        std::cout << "caught: " << e << std::endl;
    } catch (...) { // handle every exception
        std::cout << "caught: all " << std::endl;
    }

    // Rethrow
    try {
        try {
            throw std::out_of_range{"message"};
        } catch (const std::exception &ex) {
            std::cout << "caught: " << ex.what() << std::endl;
            throw;
        }
    } catch (...) {
        std::cout << "caught: all " << std::endl;
    }

    // User-defined types
    try {
        X(12, 2);
    } catch (const XError &ex) {
        std::cout << "caught: " << ex.what() << std::endl;
    }
    catch (const std::bad_alloc &) {
        std::cerr << "new ran out of memory\n";
    }

    // Catch exception from another thread
    std::promise<int> p;
    std::future<int> f = p.get_future();

    auto work = [](std::promise<int> &p) {
        try {
            throw std::out_of_range{"error from thread"};
        } catch (...) {
            p.set_exception(std::current_exception());
        }
    };

    (std::thread{work, std::ref(p)}).detach();

    try {
        f.get(); // Call get to retrieve the exception set on the other thread
    } catch (const std::exception &e) {
        std::cout << "caught: " << e.what() << std::endl;
    } catch (...) { // handle every exception
        std::cout << "caught: all " << std::endl;
    }
}
