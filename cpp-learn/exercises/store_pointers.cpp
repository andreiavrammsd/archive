/*
 https://youtu.be/j0_u26Vpb4w?t=1632
 */

#include <vector>
#include <memory>

struct SomeObject {
    explicit SomeObject(int x) : a{x} {}

    int a{};
};

int main() {
    // Raw pointers
    std::vector<SomeObject *> vecObjects;

    for (int i = 0; i < 100000; ++i) {
        for (int j = 0; j < 10; ++j) {
            vecObjects.push_back(new SomeObject{j});
        }

        for (auto &o : vecObjects) {
            delete o;
        }

        vecObjects.clear();
    }

    // Smart pointers
    std::vector<std::unique_ptr<SomeObject>> vecSmartObjects;

    for (int i = 0; i < 100000; ++i) {
        for (int j = 0; j < 10; ++j) {
            vecSmartObjects.push_back(std::make_unique<SomeObject>(j));
        }

        vecSmartObjects.clear();
    }
}
