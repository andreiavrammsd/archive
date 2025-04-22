/*
 The C++ Programming Language, 4th Edition
 */

#include <exception> // Exception class
#include <stdexcept> // Standard exceptions
#include <cassert> // Assert macro
#include <cerrno> // C-style error handling
#include <system_error> // System error support

#include <vector>

int main() {
    // Exception class
    class myex : public std::exception {
    };

    // Standard exceptions
    class myerr : public std::runtime_error {
    };

    std::vector<int> vec(1);
    try {
        vec.at(2);
    } catch (const std::out_of_range &) {
    }

    // Assert macro
    assert(vec.size() == 1);

    // C-style error handling
    errno = ERANGE;
    assert(errno == ERANGE);

    // System error support
    try {
        std::error_code err{ENOTDIR, std::system_category()};
        throw std::system_error{err};
    } catch (const std::system_error &err) {
        assert(err.code().value() == ENOTDIR);
        assert(err.code().category() == std::system_category());
    }
}
