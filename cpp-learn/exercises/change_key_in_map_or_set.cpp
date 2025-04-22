/*
 * https://www.fluentcpp.com/2020/05/01/how-to-change-a-key-in-a-map-or-set-in-cpp/
 */

#include <cassert>
#include <map>
#include <set>
#include <string>
#include <utility>

template <typename Container>
void replaceKey(Container &container, const typename Container::key_type &oldKey,
                const typename Container::key_type &newKey)
{
    auto node = container.extract(oldKey);
    if (!node.empty()) {
        node.key() = newKey;
        container.insert(std::move(node));
    }
}

int main()
{
    std::map<std::string, int> elements{
        {"A", 1},
        {"C", 2},
    };

    assert(elements.size() == 2);
    assert(elements["A"] == 1);
    assert(elements["C"] == 2);

    replaceKey(elements, "C", "B");

    assert(elements.size() == 2);
    assert(elements["A"] == 1);
    assert(elements["B"] == 2);

    auto mySet = std::set<std::string>{"one", "two", "three"};

    assert(mySet.size() == 3);
    assert(mySet.find("two") != mySet.end());

    auto entry = mySet.find("two");
    if (entry != end(mySet)) {
        mySet.erase(entry);
        mySet.insert("dos");
    }

    assert(mySet.size() == 3);
    assert(mySet.find("two") == mySet.end());
    assert(mySet.find("dos") != mySet.end());
}
