#include <vector>
#include <cassert>

#define assert_exception(f) { \
    try { \
        f; \
        assert(false); \
    } catch(const std::exception &e) { \
        assert(true); \
    }\
}

class VectorStackIsFullException : public std::exception {
};

class VectorStackIsEmptyException : public std::exception {
};

template<typename T>
class VectorStack {
public:
    explicit VectorStack(std::size_t size) : size(size) {}

    void push(T item) {
        if (isFull()) {
            throw VectorStackIsFullException();
        }

        container.insert(container.begin(), item);
    }

    T top() const {
        return container.front();
    }

    void pop() {
        if (isEmpty()) {
            throw VectorStackIsEmptyException();
        }

        container.erase(container.begin());
    }

    bool isEmpty() const {
        return container.empty();
    }

    bool isFull() const {
        return container.size() == size;
    }

    std::size_t getSize() const {
        return container.size();
    }

    virtual ~VectorStack() = default;

private:
    std::size_t size;
    std::vector<T> container;
};

int main() {
    // Vector
    VectorStack<int> vectorStack(3);

    assert(vectorStack.isEmpty());

    vectorStack.push(3);
    assert(!vectorStack.isEmpty());
    assert(!vectorStack.isFull());

    vectorStack.push(5);
    assert(vectorStack.getSize() == 2);

    vectorStack.push(1);
    assert(vectorStack.isFull());
    assert(vectorStack.top() == 1);

    vectorStack.pop();
    assert(vectorStack.getSize() == 2);
    assert(vectorStack.top() == 5);

    vectorStack.pop();
    vectorStack.pop();
    assert_exception(vectorStack.pop());

    vectorStack.push(1);
    vectorStack.push(2);
    vectorStack.push(3);
    assert_exception(vectorStack.push(4));
}
