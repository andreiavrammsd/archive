#ifndef TESTS_QUEUE
#define TESTS_QUEUE

#include <cstdlib>

template<typename T>
class Queue {
public:
    virtual void write(const T &msg) = 0;

    virtual size_t size() = 0;

    virtual T read() = 0;
};

#endif
