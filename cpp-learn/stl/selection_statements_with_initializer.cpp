/*
 Since C++17
 https://github.com/AnthonyCalandra/modern-cpp-features/blob/master/CPP17.md
 */

#include <vector>
#include <mutex>
#include <iostream>

class X {
public:
    int val() {
        return 1;
    }
};

int main() {
    std::vector<int> numbers;
    std::mutex mtx{};

    {
        std::lock_guard<std::mutex> lck(mtx);
        if (numbers.empty()) {
            numbers.push_back(1);
        }
    }
    // vs.
    if (std::lock_guard<std::mutex> lck(mtx); numbers.empty()) {
        numbers.push_back(1);
    }

    switch (X x; auto val = x.val()) {
        case 1:
            ++val;
            std::cout << val;
            break;
    }
}
