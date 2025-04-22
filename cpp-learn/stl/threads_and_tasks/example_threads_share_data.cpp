#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <condition_variable>
#include <atomic>

int main() {
    // Generate vector of numbers
    std::vector<int> numbers = {};
    for (int i = 1; i <= 1000; i++) {
        numbers.push_back(i);
    }

    // Vector to collect even numbers
    std::vector<int> collect{};

    // Mutex to protect write to collect vector
    std::mutex mtx{};

    // Condition variable to wait for all threads to finish
    std::condition_variable condition;

    // How many numbers to check per thread
    const int threshold = 100;

    // Atomic variable used to decrement each time a thread is done
    std::atomic<ulong> steps{numbers.size() / threshold};

    // Launch threads and pass a lambda which checks ranges of numbers in vector
    for (auto iterator = numbers.begin(); iterator != numbers.end(); iterator += threshold) {
        std::thread t{[iterator, &collect, &mtx, &condition, &steps] {
            for (auto n = iterator; n != iterator + threshold; ++n) {
                if (*n % 2 == 0) {
                    // Acquire lock to safely write to collect vector
                    std::unique_lock<std::mutex> lck{mtx};
                    collect.push_back(*n);
                }
            }

            // Thread is done
            steps--;
            condition.notify_one();
        }};

        t.detach();
    }

    // Wait for all threads to finish until number of steps left is zero
    std::mutex lock;
    std::unique_lock<std::mutex> wait{lock};
    condition.wait(wait, [&steps] { return steps == 0; });

    // Print result
    std::sort(collect.begin(), collect.end());
    for (int &p : collect) {
        std::cout << p << ", ";
    }
}
