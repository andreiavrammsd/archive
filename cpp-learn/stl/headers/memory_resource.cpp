/*
 https://en.cppreference.com/w/cpp/17
 */

// C++17
#include <memory_resource>

int main() {
    auto alloc = std::pmr::get_default_resource();
    auto ptr = alloc->allocate(4);
    alloc->deallocate(ptr, 4);
}
