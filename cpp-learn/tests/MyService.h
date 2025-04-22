#ifndef TESTS_MY_SERVICE
#define TESTS_MY_SERVICE

#include <vector>

#include "Queue.h"

// 100% coverage is not always 100% - scenario based; lines - branches - scenarios
inline bool all_even(const std::vector<int> &numbers)
{
    bool all_even = true;
    for (auto num : numbers) {
        if (num % 2 != 0) {
            all_even = false;
            break;
        }
    }

    return all_even;
}

template <typename T>
class MyService {
   public:
    explicit MyService(Queue<T> &queue);

    T run();

    void insert(T msg);

    virtual ~MyService() = default;

   private:
    Queue<T> &queue;
};

#endif
