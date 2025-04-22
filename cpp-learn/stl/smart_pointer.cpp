/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

template<typename T>
class Disk_ptr {
    string identifier;
    T *in_core_address{};

    void write_to_disk(T *, const string &) {
        cout << "write: " << in_core_address << endl;
    }

    T *read_from_disk(const string &) {
        cout << "read" << endl;
        return new T;
    }

public:
    explicit Disk_ptr(const string &s) : identifier{s} {}

    ~Disk_ptr() {
        write_to_disk(in_core_address, identifier);
        delete in_core_address;
    }

    T *operator->() {
        if (in_core_address == nullptr) {
            auto addr = read_from_disk(identifier);
            in_core_address = addr;
        }
        return in_core_address;
    }
};

struct Rec {
    string name;
};

void update(const string &s) {
    Disk_ptr<Rec> p{s}; // get Disk_ptr for s
    p->name = "Roscoe"; // update s; if necessary, first retrieve from disk
} // p's destructor writes back to disk

int main() {
    update("id");
}
