/*
 The C++ Programming Language, 4th Edition
 */

#include <queue>
#include <cassert>
#include <string>
#include <mutex>
#include <iostream>

struct Message {
    std::string body;
};

void push(std::queue<Message> &q, std::mutex &m, const Message &mess);

void server(std::queue<Message> &q, std::mutex &m);

int main() {
    std::queue<int> queue;

    queue.emplace(1);
    queue.push(2);
    queue.emplace(3);

    assert(queue.front() == 1);
    assert(queue.back() == 3);
    assert(queue.size() == 3);

    queue.pop();
    assert(queue.front() == 2);
    assert(queue.back() == 3);
    assert(queue.size() == 2);

    std::queue<int> queue2;
    queue2.push(2);
    queue2.push(3);
    assert(queue2 == queue);

    std::queue<int> queue3;
    queue3.swap(queue2);
    assert(queue2 != queue);
    assert(queue2.empty());
    assert(queue3 == queue);

    // example
    std::queue<Message> q;
    std::mutex m;

    push(q, m, Message{"Hello"});
    push(q, m, Message{"World"});
    server(q, m);
}

void push(std::queue<Message> &q, std::mutex &m, const Message &mess) {
    std::lock_guard<std::mutex> lck{m};
    q.push(mess);
}

void server(std::queue<Message> &q, std::mutex &m) {
    while (!q.empty()) {
        Message mess;

        {
            std::lock_guard<std::mutex> lck{m}; // lock while extracting message
            if (q.empty()) return; // somebody else got the message
            mess = q.front();
            q.pop();
        }
        // serve request

        std::cout << mess.body << '\n';
    }
}
