/*
 https://en.cppreference.com/w/cpp/17
 */

#include <execution>
#include <algorithm>

#include <atomic>
#include <vector>
#include <cassert>

/**
 * sequenced_policy
 * parallel_policy
 * parallel_unsequenced_policy
 *
 * seq
 * par
 * par_unseq
 */

template<typename T>
std::vector<T> expected(std::initializer_list<T>);

int main() {
    // When using parallel execution policy, it is the programmer's responsibility
    // to avoid data races and deadlocks

    // If the implementation cannot parallelize or vectorize (e.g. due to lack of resources),
    // all standard execution policies can fall back to sequential execution.

    // Standard library algorithms with parallelized version
    // https://www.modernescpp.com/index.php/parallel-algorithm-of-the-standard-template-library#h4-all-algorithms

    // seq
    std::vector<int> set1 = {1, 2, 3, 4, 5};
    std::vector<int> set2 = {1, 2, 3, 5};
    std::vector<int> out;
    assert(includes(std::execution::seq, set1.begin(), set1.end(), set2.begin(), set2.end()));

    // par
    std::vector<int> nums = {1, 2, 3, 4};

    assert(all_of(std::execution::par, nums.begin(), nums.end(), [](int x) { return x > 0; }));

    std::atomic<size_t> count = 0;
    std::for_each(std::execution::par, nums.begin(), nums.end(), [&count](int) {
        ++count;
    });
    assert(count == 4);

    assert(std::reduce(std::execution::par, nums.begin(), nums.end(), 1, std::multiplies{}) == 24);

    std::vector<int> needle = {2, 3};
    auto it = std::search(std::execution::par, nums.begin(), nums.end(), needle.begin(), needle.end());
    assert(it != nums.end());
    assert(it == nums.begin() + 1);

    nums = {1, 2, 3, 4, 5};
    partition(std::execution::par, nums.begin(), nums.end(), [](int x) { return x % 2 == 0; });
    assert(nums == expected({2, 4, 3, 1, 5}));
    nums = {1, 2, 3, 4};

    // par_unseq
    auto res = std::find(std::execution::par_unseq, nums.begin(), nums.end(), 2);
    assert(res == nums.begin() + 1);

    // is_execution_policy
    static_assert(std::is_execution_policy<std::execution::parallel_policy>::value);
    static_assert(std::is_execution_policy<std::execution::parallel_unsequenced_policy>::value);
    struct X {
    };
    static_assert(!std::is_execution_policy<X>::value);
}

template<typename T>
std::vector<T> expected(std::initializer_list<T> l) {
    return std::vector<T>{l};
}
