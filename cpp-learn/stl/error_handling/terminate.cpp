/*
 The C++ Programming Language, 4th Edition
 */

#include <exception>
#include <iostream>
#include <csignal>

int main() {
    signal(SIGABRT, [](int sig) {
        std::cerr << "caught signal: " << sig << '\n';
        exit(0);
    });

    std::set_terminate([] {
        std::cerr << "app was terminated\n";
    });

    std::terminate();
}
