/*
 The C++ Programming Language, 4th Edition
 */

#include <exception>
#include <cassert>

class Underflow : public std::exception {
};

template<class T>
class List {
public:
    void insert(T);

    T get();

// A list is nonintrusive if it does not require modification to its elements
// (e.g., by requiring element types to have link fields). The information and
// data structures used to organize the list can be kept private:
private:
    struct Link {
        T val;
        Link *next;
    };

    Link *head;
};

template<class T>
void List<T>::insert(T val) {
    Link *lnk = new Link;
    lnk->val = val;
    lnk->next = head;
    head = lnk;
}

template<class T>
T List<T>::get() {
    if (head == nullptr) {
        throw Underflow{};
    }

    Link *p = head;
    head = p->next;
    return p->val;
}

int main() {
    List<int> list{};
    list.insert(1);
    list.insert(2);

    assert(list.get() == 2);
    assert(list.get() == 1);

    try {
        list.get();
        assert(false);
    } catch (const Underflow &) {
        static_assert(true, "underflow");
    }
}
