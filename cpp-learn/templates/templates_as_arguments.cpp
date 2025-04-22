/*
 The C++ Programming Language, 4th Edition
 */

#include <vector>
#include <set>

template<typename T, template<typename> class C>
class Xrefd {
public:
    C<T> mems;
    C<T *> refs;
};

template<typename T>
using My_vec = std::vector<T>; // use default allocator

struct Entry {
};

Xrefd<Entry, My_vec> x1; // store cross references for Entrys in a vector

template<typename T>
class My_container {
public:
    void push(T) {}
};

struct Record {
};

Xrefd<Record, My_container> x2; // store cross references for Records in a My_container

// The common case in which a template needs only a container or two is often better handled by
// passing the container types (§31.5.1).
template<typename C, typename C2>
class Xrefd2 {
public:
    C mems;
    C2 refs;
};

Xrefd2<std::vector<Entry>, std::set<Entry *>> x;

int main() {
    x1.mems.push_back(Entry{});
    x1.refs.push_back(new Entry{});

    x2.mems.push(Record{});
    x2.refs.push(new Record{});

    x.mems.push_back(Entry{});
    x.refs.emplace(new Entry{});
}
