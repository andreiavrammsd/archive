#include <queue>
#include <string>
#include <mutex>
#include <cassert>
#include <iostream>

struct Message {
    std::string body;
    int priority{};

    friend bool operator<(const Message &a, const Message &b) {
        return a.priority < b.priority;
    }
};

void server(std::priority_queue<Message> &q, std::mutex &m);

int main() {
    std::priority_queue<Message> queue;

    queue.push(Message{"A"});
    queue.push(Message{"B", 1});
    queue.push(Message{"C", -2});
    queue.push(Message{"D", -2});

    assert(queue.top().body == "B");

    std::mutex m;
    server(queue, m);
}

void server(std::priority_queue<Message> &q, std::mutex &m) {
    while (!q.empty()) {
        Message mess;

        {
            std::lock_guard<std::mutex> lck{m}; // lock while extracting message
            if (q.empty()) return; // somebody else got the message
            mess = q.top();
            q.pop();
        }
        // serve request

        std::cout << mess.body << '\n';
    }
}
