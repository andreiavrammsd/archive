/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <csignal>
#include <iostream>
#include <stdexcept>

// Standard-Library Exceptions

/**
 * bitset_			    Throws invalid_argument, out_of_range, overflow_error
 *
 * iostream		        Throws ios_base::failure if exceptions are enabled
 *
 * regex			    Throws regex_error
 *
 * string			    Throws length_error, out_of_range
 *
 * vector			    Throws out_of_range
 *
 * new T			    Throws bad_alloc if it cannot allocate memory for a T
 *
 * dynamic_cast<T>(r)	Throws bad_cast if it cannot convert the reference r to a T
 *
 * typeid()		        Throws bad_typeid if it cannot deliver a type_info
 *
 * thread			    Throws system_error
 *
 * call_once()		    Throws system_error
 *
 * mutex			    Throws system_error
 *
 * condition_variable	Throws system_error
 *
 * async()			    Throws system_error
 *
 * packaged_task		Throws system_error
 *
 * future and promise	Throws future_error
 */

struct My_error : std::runtime_error {
    My_error(int x) : std::runtime_error{"My_error"}, interesting_value{x} {}

    int interesting_value;
};

void handle(int i)
{
    try {
        switch (i) {
            case 1:
                throw My_error{i};
            case 2:
                throw std::runtime_error{"err"};
            case 3:
                throw std::exception{};
            default:
                throw i;
                break;
        }
    }
    catch (My_error &me) {  // a My_error happened
        // we can use me.interesting_value and me.what()
        std::cout << "\nMy_error: ";
        std::cout << me.interesting_value << ", ";
        std::cout << me.what() << '\n';
    }
    catch (std::runtime_error &re) {  // a runtime_error happened
        std::cout << "\nRuntime error: ";
        std::cout << re.what() << ", ";

        auto ep = std::make_exception_ptr(re);  // exception pointer
        std::rethrow_exception(ep);
    }
    catch (std::exception &e) {  // some standard-library exception happened
        std::cout << "\nException: ";
        // we can use e.what()

        // Exception Propagation
        auto ep = std::current_exception();
        std::rethrow_exception(ep);
    }
    catch (...) {  // Some unmentioned exception happened
        // we can do local cleanup
        std::cout << "\nUnmentioned exception: ";
        std::cout << "Some unmentioned exception happened\n";
    }
}

struct Some_error : std::runtime_error {
    Some_error(int x) : std::runtime_error{"Some_error"}, interesting_value{x} {}

    int interesting_value;
};

void my_code()
{
    try {
        throw Some_error{2};
    }
    catch (...) {
        My_error err{1};
        std::throw_with_nested(err);
    }
}

void noexception() noexcept
{
    // throw My_error{0}; // would terminate
}

int main()
{
    handle(1);

    try {
        handle(2);
    }
    catch (const std::runtime_error &e) {
        std::cout << "Runtime error caught\n";
    }

    try {
        handle(3);
    }
    catch (const std::exception &e) {
        std::cout << "Exception caught\n";
    }

    handle(4);

    // Nested exception
    try {
        my_code();
    }
    catch (My_error &err) {
        std::cout << "\nNested exception: ";
        std::cout << err.interesting_value << ", ";
        std::cout << err.what() << '\n';

        // ... clear up My_error problems ...
        try {
            std::rethrow_if_nested(err);  // re-throw the nested exception, if any
        }
        catch (Some_error &err2) {
            std::cout << "\nNested exception was rethrown: ";
            std::cout << err2.interesting_value << ", ";
            std::cout << err2.what() << '\n';
            // ... clear up Some_error problems ...
        }
    }

    // noexcept
    signal(SIGABRT, [](int sig) {
        std::cerr << "caught signal: " << sig << '\n';
        std::cerr << "exception was thrown from noexcept function\n";
        exit(0);
    });

    try {
        std::cout << '\n';
        noexception();
    }
    catch (...) {
    }

    // uncaught_exception (C++17)
    struct Foo {
        int count = std::uncaught_exceptions();

        ~Foo()
        {
            std::cout << (count == std::uncaught_exceptions() ? "~Foo() called normally\n"
                                                              : "~Foo() called during stack unwinding\n");
        }
    };

    Foo foo;
    try {
        Foo foo2;
        std::cout << "Exception thrown\n";
        throw std::runtime_error("test exception");
    }
    catch (const std::exception &e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
}
