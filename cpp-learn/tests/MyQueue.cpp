#include <string>
#include <cstdlib>

#include "MyQueue.h"

template<typename T>
void MyQueue<T>::write(const T &msg) {
    queue.push(msg);
}

template<typename T>
size_t MyQueue<T>::size() {
    return queue.size();
}

template<typename T>
T MyQueue<T>::read() {
    auto element = std::move(queue.front());
    queue.pop();
    return element;
}

template
class MyQueue<std::string>;

template
class MyQueue<int>;
