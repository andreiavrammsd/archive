/*
 The C++ Programming Language, 4th Edition
 */

#include <string>
#include <map>

template<typename Key, typename Value>
class Map {
    std::map<Key, Value> items;
public:
    void add(Key k, Value v) {
        items[k] = v;
    }
};

template<typename Value>
using String_map = Map<std::string, Value>;

int main() {
    String_map<int> m; // m is a Map<string,int>
    m.add("k", 1);
}
