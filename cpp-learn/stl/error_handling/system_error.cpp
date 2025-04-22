/*
 The C++ Programming Language, 4th Edition
 */

#include <ostream>
#include <string>
#include <fstream>
#include <functional>
#include <iostream>

std::pair<std::string, std::string> split_into_directory_and_name(const std::string &path);

std::error_code does_directory_exist(const std::string &path);

std::ostream &open_file(const std::string &path) {
    auto dn = split_into_directory_and_name(path); // split into {path, name}

    std::error_code err{does_directory_exist(dn.first)}; // ask "the system" about the path
    if (err) { // err != 0 means error
        // ... see if anything can be done ...

        bool cannot_handle_err = true;

        if (cannot_handle_err) {
            throw std::system_error(err);
        }
    }

    std::ofstream f{path};

    return std::ref(f);
}

std::pair<std::string, std::string> split_into_directory_and_name(const std::string &) {
    std::pair<std::string, std::string> parts;
    parts.first = "a";
    parts.second = "b";
    return parts;
}

std::error_code does_directory_exist(const std::string &) {
    return {ENOTDIR, std::system_category()};
}

// Mapping Error Codes
enum class future_errc {
    broken_promise = 1,
    future_already_retrieved,
    promise_already_satisfied,
    no_state
};

class future_cat : public std::error_category {
    const char *name() const noexcept override { return "future"; }

    std::string message(int ec) const override;
};

const future_cat &future_category() noexcept {
    static future_cat obj;
    return obj;
}

std::string future_cat::message(int ec) const {
    auto cat = static_cast<future_errc >(ec);

    switch (cat) {
        default:
            return "bad future_error code";
        case future_errc::broken_promise:
            return "future_error: broken promise";
        case future_errc::future_already_retrieved:
            return "future_error: future already retrieved";
        case future_errc::promise_already_satisfied:
            return "future_error: promise already satisfied";
        case future_errc::no_state:
            return "future_error: no state";
    }
}

std::error_code make_error_code(future_errc e) noexcept {
    return {int(e), future_category()};
}

std::error_code lib() {
    return make_error_code(future_errc::future_already_retrieved);
}

void f() {
    auto err = lib();
    if (err) {
        throw std::system_error(err);
    }
}

int main() {
    try {
        open_file("/path/to/file.ext");
    } catch (const std::system_error &er) {
        std::cout << er.code() << '\n';
        std::cout << er.what() << '\n';
    }

    std::cout << '\n';

    try {
        f();
    } catch (const std::system_error &er) {
        std::cout << er.code() << '\n';
        std::cout << er.what() << '\n';
    }
}
