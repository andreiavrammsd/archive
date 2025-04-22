#ifndef TESTS_MY_QUEUE
#define TESTS_MY_QUEUE

#include <cstdlib>
#include <queue>

#include "Queue.h"

template<typename T>
class MyQueue : public Queue<T> {
public:
    void write(const T &msg) override;

    size_t size() override;

    T read() override;

    virtual ~MyQueue() = default;

private:
    std::queue<T> queue;
};

#endif
