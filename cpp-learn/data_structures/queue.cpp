#include <vector>
#include <cassert>
#include <stdexcept>
#include <sstream>

template <typename T>
class Queue {
   public:
    virtual void enqueue(T element) = 0;

    virtual T dequeue() = 0;

    virtual T peek() const = 0;

    virtual int length() const = 0;

    virtual bool full() const = 0;

    virtual bool empty() const = 0;
};

template <typename T>
class VectorQueue : public Queue<T> {
   public:
    explicit VectorQueue(size_t s) : size(s) {}

    void enqueue(T element) override
    {
        if (full()) {
            std::ostringstream what{};
            what << "queue is full (max: " << size << ")";

            throw std::overflow_error(what.str());
        }

        container.push_back(element);
    }

    T dequeue() override
    {
        if (empty()) {
            throw std::underflow_error("queue is empty");
        }

        auto element = container.at(0);

        container.erase(container.begin());

        return element;
    }

    T peek() const override { return container.at(0); }

    int length() const override { return container.size(); }

    bool full() const override { return container.size() == size; }

    bool empty() const override { return container.empty(); }

    virtual ~VectorQueue() = default;

   private:
    std::vector<T> container;
    size_t size;
};

int main()
{
    VectorQueue<int> queue(3);

    assert(queue.empty());

    queue.enqueue(9);
    queue.enqueue(-45);

    assert(!queue.empty());
    assert(queue.length() == 2);
    assert(queue.peek() == 9);

    assert(queue.dequeue() == 9);
    assert(queue.peek() == -45);
    assert(queue.length() == 1);

    queue.enqueue(2);
    assert(!queue.full());

    queue.enqueue(3);
    assert(queue.full());

    queue.dequeue();
    queue.dequeue();
    queue.dequeue();

    try {
        queue.dequeue();
        assert(false);
    }
    catch (const std::underflow_error &ex) {
        assert((std::string)ex.what() == "queue is empty");
    }

    queue.enqueue(5);
    queue.enqueue(5);
    queue.enqueue(5);

    try {
        queue.enqueue(5);
        assert(false);
    }
    catch (const std::overflow_error &ex) {
        assert((std::string)ex.what() == "queue is full (max: 3)");
    }
}
