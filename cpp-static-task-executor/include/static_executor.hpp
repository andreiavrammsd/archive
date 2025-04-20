#ifndef STATIC_EXECUTOR_HPP_
#define STATIC_EXECUTOR_HPP_

#include <tuple>

namespace static_executor {

template <typename T, std::size_t S = std::tuple_size<T>::value, std::size_t I = S - 1>
struct Executor {
    T &tasks;

    void Execute()
    {
        std::get<S - I - 1>(tasks).Execute();
        Executor<T, S, I - 1>{tasks}.Execute();
    }
};

template <typename T, std::size_t S>
struct Executor<T, S, 0> {
    T &tasks;

    void Execute() { std::get<S - 1>(tasks).Execute(); }
};

}  // namespace static_executor

#endif  // STATIC_EXECUTOR_HPP_
