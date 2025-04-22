/*
 The C++ Programming Language, 4th Edition
 */

#include <numeric>
#include <vector>
#include <future>
#include <cassert>

// compute the sum of [beg:end) starting with the initial value init
double accum(double *beg, double *end, double init) { return std::accumulate(beg, end, init); }

double compute(std::vector<double> &v)
{
    using Task_type = double(double *, double *, double);  // type of task

    std::packaged_task<Task_type> pt0{accum};  // package the task (accum)
    std::packaged_task<Task_type> pt1{accum};

    std::future<double> f0{pt0.get_future()};  // get hold of pt0's future
    std::future<double> f1{pt1.get_future()};  // get hold of pt1's future

    double *first = &v[0];

    std::thread t1{std::move(pt0), first, first + v.size() / 2, 0};  // start a thread for pt0
    t1.detach();

    std::thread t2{std::move(pt1), first + v.size() / 2, first + v.size(), 0};  // start a thread for pt1
    t2.detach();

    // The move() operations are needed because a packaged_task cannot be copied.

    return f0.get() + f1.get();
}

int main()
{
    std::vector<double> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(compute(numbers) == 45);
}
