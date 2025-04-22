/*
 * https://www.geeksforgeeks.org/deadlock-starvation-and-livelock/
 */

#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <vector>

int main() {
    /**
     * Livelock
     */

    /*
    std::mutex mtx1;
    std::mutex mtx2;

    std::thread th1{[&] {
        while(true) {
            if (!mtx1.try_lock()) {
                std::cout << "wait 1.1" << std::endl;
                continue;
            }

            if (!mtx2.try_lock()) {
                std::cout << "wait 1.2" << std::endl;
                continue;
            }

            std::cout << "work 1" << std::endl;
        }
    }};

    std::thread th2{[&] {
        while(true) {
            if (!mtx2.try_lock()) {
                std::cout << "wait 2.1" << std::endl;
                continue;
            }

            if (!mtx1.try_lock()) {
                std::cout << "wait 2.2" << std::endl;
                continue;
            }

            std::cout << "work 2" << std::endl;
        }
    }};

    th1.join();
    th2.join();
    //*/

    /*
    std::timed_mutex mtx1;
    std::timed_mutex mtx2;

    std::thread th1{[&] {
        while(true) {
            std::unique_lock<std::timed_mutex> lck1{mtx1};
            if (!lck1.try_lock_for(std::chrono::milliseconds{100})) {
                std::cout << "wait 1.1" << std::endl;
                continue;
            }

            std::unique_lock<std::timed_mutex> lck2{mtx2};
            if (!lck2.try_lock_for(std::chrono::milliseconds{100})) {
                std::cout << "wait 1.2" << std::endl;
                continue;
            }

            std::cout << "work 1" << std::endl;
        }
    }};

    std::thread th2{[&] {
        while(true) {
            std::unique_lock<std::timed_mutex> lck2{mtx2};
            if (!lck2.try_lock_for(std::chrono::milliseconds{100})) {
                std::cout << "wait 2.1" << std::endl;
                continue;
            }

            std::unique_lock<std::timed_mutex> lck1{mtx1};
            if (!lck1.try_lock_for(std::chrono::milliseconds{100})) {
                std::cout << "wait 2.2" << std::endl;
                continue;
            }

            std::cout << "work 2" << std::endl;
        }
    }};

    th1.join();
    th2.join();
    //*/

    /**
     * Deadlock
     */

    /*
    std::mutex mtx1;
    std::mutex mtx2;

    std::thread th1{[&] {
        mtx1.lock();
        mtx2.lock();
        std::cout << "work 1" << std::endl;
    }};

    std::thread th2{[&] {
        mtx2.lock();
        mtx1.lock();
        std::cout << "work 2" << std::endl;
    }};

    th1.join();
    th2.join();
    //*/

    /*
    std::mutex mtx1;
    std::mutex mtx2;

    std::thread th1{[&] {
        using namespace std::chrono_literals;

        std::lock_guard lck1{mtx1};
        std::this_thread::sleep_for(1ms);
        std::lock_guard lck2{mtx2};

        std::cout << "work 1" << std::endl;
    }};

    std::thread th2{[&] {
        using namespace std::chrono_literals;

        std::lock_guard lck2{mtx2};
        std::this_thread::sleep_for(1ms);
        std::lock_guard lck1{mtx1};

        std::cout << "work 2" << std::endl;
    }};

    th1.join();
    th2.join();
    //*/

    /*
    std::mutex mtx;

    mtx.lock();
    {
        // mtx.unlock();
        mtx.lock();
    }
    //*/

    /**
     * Starvation
     */

    /*
    std::vector<int> numbers;
    std::timed_mutex mtx;

    std::thread th1{[&] {
        using namespace std::chrono_literals;

        while (true) {
            std::lock_guard<std::timed_mutex> lck{mtx};
            std::cout << "work 1" << std::endl;
            numbers.push_back(1);
            std::this_thread::sleep_for(1s);
        }
    }};

    std::thread th2{[&] {
        while (true) {
            std::lock_guard<std::timed_mutex> lck{mtx};
            std::cout << "work 2" << std::endl;
            numbers.push_back(2);
        }
    }};

    std::thread th3{[&] {
        while (true) {
            std::lock_guard<std::timed_mutex> lck{mtx};
            std::cout << "work 3" << std::endl;
            numbers.push_back(3);
        }
    }};

    th1.join();
    th2.join();
    th3.join();
    //*/
}
