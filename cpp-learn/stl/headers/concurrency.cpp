/*
 The C++ Programming Language, 4th Edition
 */

#include <atomic> // Atomic types and operations
#include <condition_variable> // Waiting for an action
#include <future> // Asynchronous task
#include <mutex> // Mutual exclusion classes
#include <thread> // Threads

#include <cassert>

int main() {
    // Atomic types and operations
    std::atomic<int> ati = 0;
    ++ati;

    // Waiting for an action
    std::mutex mtx;
    std::condition_variable cond;
    int v = 0;
    int limit = 5;

    std::thread th1{[](std::mutex &mtx, std::condition_variable &cond, int &v, int limit) {
        std::unique_lock<std::mutex> lck{mtx};
        cond.wait(lck, [&v, limit] { return v > limit; });
    }, std::ref(mtx), std::ref(cond), std::ref(v), limit};

    std::thread th2{[](std::mutex &mtx, std::condition_variable &cond, int &v, int limit) {
        for (;;) {
            std::this_thread::sleep_for(std::chrono::milliseconds{50});
            std::lock_guard<std::mutex> lck{mtx};

            ++v;
            cond.notify_one();

            if (v > limit) {
                break;
            }
        }
    }, std::ref(mtx), std::ref(cond), std::ref(v), limit};

    th1.join();
    th2.join();

    // Asynchronous task
    auto sum = [](int a, int b) { return a + b; };
    std::future<int> task = std::async(sum, 1, 2);
    assert(task.get() == 3);

    std::promise<int> promise;
    promise.set_value(1);
    auto future = promise.get_future();
    future.wait();
    assert(future.get() == 1);

    // Mutual exclusion classes
    std::mutex m;
    m.lock();
    m.unlock();

    // Threads
    std::thread th{[]() {}};
    assert(th.joinable());
    assert(th.get_id() != std::thread::id{});
    th.join();
}
