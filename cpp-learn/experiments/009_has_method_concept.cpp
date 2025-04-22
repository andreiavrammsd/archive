#include <concepts>

template <typename T>
concept Task = requires(T&& task, int input)
{
    // clang-format off
    { task.execute(input) } -> std::same_as<int>;
    // clang-format on
};

// clang-format off
template<typename T>
requires Task<T>
int executor(int input, T&& task)
{
    return task.execute(input);
}
// clang-format on

struct Task1 {
    int execute(int input) { return input + 1; }
};

struct InvalidTask {
    int run(int input) { return input + 2; }
};

struct Task3 {
    double execute(int input) { return input + 3; }
};

struct Task4 {
    int execute(Task1 input) { return input.execute(1) + 4; }
};

int main()
{
    executor(1, Task1{});
    //    execute(2, InvalidTask{});
    //    execute(3, Task3{});
    //    execute(4, Task4{});
}
