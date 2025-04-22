/**
 * Weak Counter
 *
 * https://www.nextptr.com/tutorial/ta1358374985/shared_ptr-basics-and-internals-with-examples
 *
 * The following multithreaded example shows how a shared_ptr can be created from a weak_ptr
 * as long as the managed object is alive.
 *
 * A reader thread periodically tries to acquire a shared_ptr<std::atomic_int> from a
 * weak_ptr<std::atomic_int> and logs the value.
 *
 * If the reader thread cannot acquire a shared_ptr in an iteration, it exits.
 *
 * A writer thread periodically changes the shared_ptr managed std::atomic_int value a few
 * times and exits.
 *
 * When the writer thread exits, the shared_ptr held by it is destroyed, and the reader
 * thread can no longer get a shared_ptr from its weak_ptr, which makes the reader thread
 * to also exit.
 *
 * The program terminates when both the threads exit:
 */

#include <memory>
#include <atomic>
#include <thread>
#include <iostream>

int main() {
    auto sp = std::make_shared<std::atomic_int>();

    // Reader
    // A weak_ptr is created and captured
    std::thread r([wp = std::weak_ptr<std::atomic_int>(sp)] { // weak_ptr created. ref count: 1, weak count: 1
        while (true) {
            // Acquire a shared_ptr through lock()
            if (auto p = wp.lock()) {
                // shared_ptr acquired. ref count is 1 or 2
                std::cout << *p << '\n';
            } else {
                // shared_ptr could not be acquired. ref count: 0
                break;
            }

            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    // Writer
    // The shared_ptr is moved an and captured the ref count stays 1
    // If the shared_ptr is copied instead of move, this program will never
    // end because the reader would never exit (try that!).
    std::thread w([mp = move(sp)] { // shared_ptr move. ref count: 1
        for (int i = 1; i <= 5; ++i) {
            *mp = i; // change managed object
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    w.join();
    r.join();
}
