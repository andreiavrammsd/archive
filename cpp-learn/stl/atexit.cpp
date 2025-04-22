/*
 The C++ Programming Language, 4th Edition
 */

#include <iostream>

void cleanup();

void register_cleanup();

void work();

int main() {
    register_cleanup();

    try {
        work();
    } catch (const char *ex) {
        std::cerr << "error: " << ex << std::endl;
        work();
    } catch (...) {
        std::cerr << "unknown error (will exit)" << std::endl;
        exit(1);
    }
}

void cleanup() {
    std::cerr << "doing cleanup..." << std::endl;
}

void register_cleanup() {
    if (atexit(&cleanup) == 0) {
        std::cout << "cleanup registered" << std::endl;
    } else {
        std::cout << "too many atexit functions" << std::endl;
    }
}

void work() {
    static int i = 0;

    while (true) {
        try {
            ++i;

            if (i == 1) {
                throw "recoverable";
            }

            if (i == 2) {
                throw 1;
            }
        } catch (const char *ex) {
            std::cerr << "error: " << ex << std::endl;
        }
    }
}
