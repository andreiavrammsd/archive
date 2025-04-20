#ifndef EXECUTOR_DYNAMIC_TASKS_HPP_
#define EXECUTOR_DYNAMIC_TASKS_HPP_

#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

#include "dynamic_executor.hpp"

namespace dynamic_tasks {

std::random_device rd;
std::mt19937 e(rd());
std::uniform_int_distribution<int> rndm(0, 10);

std::vector<int> numbers;

struct Data {
    int x;
};

class A : public dynamic_executor::Task {
   public:
    explicit A(Data &d) : d_{d} {}

    void Execute() override
    {
        d_.x += rndm(e);

        numbers.push_back(d_.x);
        std::sort(numbers.begin(), numbers.end());
    }

   private:
    Data &d_;
};

class B : public dynamic_executor::Task {
   public:
    explicit B(Data &d) : d_{d} {}

    void Execute() override
    {
        d_.x -= rndm(e);

        numbers.erase(std::remove_if(numbers.begin(), numbers.end(), [](int x) { return x > 2; }), numbers.end());
    }

   private:
    Data &d_;
};

class C : public dynamic_executor::Task {
   public:
    explicit C(Data &d) : d_{d} {}

    void Execute() override
    {
        if (d_.x % 2 == 0) {
            d_.x += rndm(e);
        }
    }

   private:
    Data &d_;
};

}  // namespace dynamic_tasks

#endif  // EXECUTOR_DYNAMIC_TASKS_HPP_
