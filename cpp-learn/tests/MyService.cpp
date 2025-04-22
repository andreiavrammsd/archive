#include <string>

#include "MyService.h"

template<typename T>
MyService<T>::MyService(Queue<T> &queue) : queue(queue) {}

template<typename T>
T MyService<T>::run() {
    T data{};

    while (queue.size() > 0) {
        data += queue.read();
    }

    return data;
}

template<typename T>
void MyService<T>::insert(T msg) {
    queue.write(msg);
}

template MyService<std::string>::MyService(Queue<std::string> &queue);

template std::string MyService<std::string>::run();

template void MyService<std::string>::insert(std::string);

template MyService<int>::MyService(Queue<int> &queue);

template int MyService<int>::run();

template void MyService<int>::insert(int);
