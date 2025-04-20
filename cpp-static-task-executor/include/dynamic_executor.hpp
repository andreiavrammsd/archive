#ifndef DYNAMIC_EXECUTOR_HPP_
#define DYNAMIC_EXECUTOR_HPP_

namespace dynamic_executor {

class Task {
   public:
    virtual void Execute() = 0;
};

template <typename T>
struct Executor {
    T& tasks;

    void Execute()
    {
        for (auto& task : tasks) {
            task->Execute();
        }
    }
};

}  // namespace dynamic_executor

#endif  // DYNAMIC_EXECUTOR_HPP_
