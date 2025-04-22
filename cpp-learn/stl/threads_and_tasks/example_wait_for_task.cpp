/*
 https://stackoverflow.com/questions/24301009/c11-future-wait-for-always-returns-future-statustimeout
 http://coliru.stacked-crooked.com/a/cb4e4b3f642f79f5
 */

// future example
#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <chrono>         // std::chrono::milliseconds


const int number = 444444443;

// a non-optimized way of checking for prime numbers:
bool is_prime(int x) {
    for (int i = 2; i < x; ++i) if (x % i == 0) return false;
    return true;
}

int main() {
    // call function asynchronously:
    std::future<bool> fut = std::async(/*std::launch::async,*/ is_prime, number);

    // do something while waiting for function to set future:
    std::cout << "checking, please wait";
    std::chrono::milliseconds span(100);
    //std::chrono::duration<int> span (1);
    bool x;
    // if task is deferred...
    if (fut.wait_for(std::chrono::milliseconds(0)) == std::future_status::deferred) {
        // ...use wait or get on fut
        // to call is_prime synchronously
        x = fut.get();     // retrieve return value
    } else { // task isn't deferred
        // infinite loop not possible (assuming is_prime finishes)
        while (fut.wait_for(span) != std::future_status::ready) {
            // task is neither deferred nor ready,
            // so do concurrent work until it's ready
            std::cout << '.';
            std::cout.flush();
        }
        // fut is ready
        x = fut.get();     // retrieve return value
    }

    std::cout << "\n" << number << " " << (x ? "is" : "is not") << " prime.\n";

    return 0;
}
