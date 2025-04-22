/*
 https://www.dailycodingproblem.com/

 You run an e-commerce website and want to record the last N order ids in a log.
 Implement a data structure to accomplish this, with the following API:

 - record(order_id): adds the order_id to the log
 - get_last(i): gets the ith last element from the log. i is guaranteed to be smaller than or equal to N.
 */

#include <cstdlib>
#include <cassert>

class Orders {
public:
    explicit Orders(size_t size) : size(size), orders(new int[size]) {
    }

    Orders(const Orders &o) = delete;

    Orders &operator=(const Orders &o) = delete;

    void record(int order_id) {
        if (index > size - 1) {
            for (size_t i = 0; i < size; ++i) {
                orders[i] = orders[i + 1];
            }
            index = size - 1;
        }

        orders[index] = order_id;
        ++index;
    }

    int get_last(size_t i) const {
        return orders[index - i];
    }

    virtual ~Orders() {
        delete[] orders;
    }

private:
    size_t size;
    size_t index{};
    int *orders;
};

int main() {
    Orders orders(3);

    orders.record(67);
    assert(orders.get_last(1) == 67);

    orders.record(68);
    assert(orders.get_last(1) == 68);
    assert(orders.get_last(2) == 67);

    orders.record(69);
    assert(orders.get_last(1) == 69);
    assert(orders.get_last(2) == 68);
    assert(orders.get_last(3) == 67);

    orders.record(70);
    assert(orders.get_last(1) == 70);
    assert(orders.get_last(2) == 69);
    assert(orders.get_last(3) == 68);
}
