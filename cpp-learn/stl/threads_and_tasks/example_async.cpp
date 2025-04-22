/*
 The C++ Programming Language, 4th Edition
 */

#include <numeric>
#include <vector>
#include <future>
#include <cassert>
#include <algorithm>

// compute the sum of [beg:end) starting with the initial value init
double accum(double *beg, double *end, double init) {
    return std::accumulate(beg, end, init);
}

double compute(std::vector<double> &v) {
    auto v0 = &v[0];
    auto sz = v.size();

    if (v.size() < 10000) {
        return accum(v0, v0 + sz, 0.0);
    }

    auto f0 = std::async(accum, v0, v0 + sz / 2, 0.0);
    auto f1 = std::async(accum, v0 + sz / 2, v0 + sz, 0.0);

    return f0.get() + f1.get(); // collect and combine results
}

int main() {
    std::vector<double> numbers(10001);

    int total = 0;
    std::generate(numbers.begin(), numbers.end(), [&total] {
        static int i = 1;
        total += i;
        return i++;
    });

    assert(compute(numbers) == total);
}
