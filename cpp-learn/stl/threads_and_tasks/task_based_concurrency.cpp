/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <future>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

int task(int, int);

void sum(std::promise<int> &&promise, int x, int y);

int ff(int);

using Res = int;
using Args = int;
using PPT = Res(Args, Args);

Res f(Args, Args);

struct G {
    Res operator()(Args, Args);
};

auto h = [](Args a, Args b) {
    if (a == 1) {
        throw std::runtime_error("a == 1");
    }
    return a * b;
};

template <typename T>
class Sync_queue {
   public:
    void put(T &&val);

    void get(T &val);

    size_t size() const;

   private:
    std::timed_mutex mtx;
    std::condition_variable_any cond;
    std::list<T> q;
};

Sync_queue<std::packaged_task<PPT>> server;

void consumer();

template <typename T>
std::vector<T> wait_for_all(std::vector<std::future<T>> &vf);

/**
 * return index of ready future
 * if no future is ready, wait for d before trying again
 */
template <typename T>
int wait_for_any(std::vector<std::future<T>> &vf, std::chrono::steady_clock::duration d);

double square(int i) { return i * i; }

int main()
{
    // packaged_task<F>
    // promise<T>
    // future<T>
    // shared_future<T>
    // x=async(policy,f,args)
    // x=async(f,args)

    assert(task(1, 2) == 3);
    assert(std::async(task, 1, 2).get() == 3);

    /**
     * future and promise
     *
     * [...] communication between flags is handled by a future/promise pair.
     *
     * A task puts its result into a promise, and a task that needs the result
     * retrieves the result from the corresponding future
     *
     * At a minimum, a shared state must be able to hold:
            - A value of the appropriate type or an exception. For a future "returning void"
            the value is nothing.

            - A ready bit to indicate whether a value or exception is ready to be extracted by a future.

            - The task to be executed when a get() is called for a future for a task launched by async() with
            the launch policy deferred (§42.4.6).

            - A use count, so that the shared state can be destroyed when and only when its last potential
            user relinquishes access. In particular, if a stored value is of a class with a destructor,
            its destructor is called when the use count goes to zero.

            - Some mutual exclusion data to enable unblocking of any thread that might be waiting
            (e.g., a condition_variable).
     */

    /**
     * promise
     *
     * A promise is the handle to a shared state (§42.4.1). It is where a task can deposit its result to be
     * retrieved through a future (§42.4.4).
     */

    // promise pr {};
    // promise pr {allocator_arg_t,a};
    // promise pr {pr2};
    // pr.~promise()
    // pr2=move(pr)
    // pr.swap(pr2)
    // fu=pr.g et_future()
    // pr.set_value(x)
    // pr.set_value()
    // pr.set_exception(p)
    // pr.set_value_at_thread_exit(x)
    // pr.set_exception_at_thread_exit(p)
    // swap(pr,pr2)

    // It is only possible to transmit a single result value through a promise. That may seem restrictive,
    // but remember that the value is moved into and out of the shared state, rather than copied, so
    // that we can cheaply pass a collection of objects.

    std::promise<std::map<std::string, int>> pr;

    std::map<std::string, int> m;
    m.emplace("A", 1);
    m.emplace("B", 2);

    pr.set_value(m);

    auto res = pr.get_future().get();
    assert(res.size() == 2);
    assert(res["A"] == 1);
    assert(res["B"] == 2);

    int x = 20;
    int y = 21;
    std::promise<int> pr2;                      // declare a promise
    std::future<int> fu = pr2.get_future();     // get a future from promise
    std::thread th(sum, std::move(pr2), x, y);  // declare a thread with the computing function and its arguments
    assert(fu.get() == 41);                     // wait for future to end and get the result
    th.join();                                  // start thread

    /**
     * packaged_task
     *
     * A packaged_task holds a task and a future/promise pair.
     *
     * We pass a task (a function or a function object) that we want executed to a packaged_task. When
     * our task executes a return x, it causes a set_value(x) on the packaged_task's promise.
     * Similarly, a throw x causes a set_exception(px) where px is an exception_ptr for x.
     * Basically, the packaged_task executes its task, f(args), like this:

            try {
                pr.set_value(f(args)); // assume that the promise is called pr
            }
            catch(...) {
                pr.set_exception(current_exception());
            }
     */

    // packaged_task pt {};
    // packaged_task pt {f};
    // packaged_task pt {allocator_arg_t,a,f};
    // packaged_task pt {pt2};
    // pt=move(pt2)
    // pt.~packaged_task();
    // pt.swap(pt2)
    // pt.valid()
    // fu=pt.get_future()
    // pt()(args)
    // pt.make_ready_at_exit(args)
    // pt.reset()
    // swap(pt,pt2)
    // uses_allocator<PT,A>

    std::packaged_task<int(int)> pt1{ff};  // store ff in pt1
    std::packaged_task<int(int)> pt2{ff};  // store ff in pt2

    pt1(1);  // let pt1 call ff(1);
    pt2(0);  // let pt2 call ff(0);

    auto v1 = pt1.get_future();
    auto v2 = pt2.get_future();

    try {
        std::cout << v1.get() << '\n';  // will print
        std::cout << v2.get() << '\n';  // will throw
    }
    catch (const std::exception &e) {
        std::cout << "exception: " << e.what() << "\n\n";
    }

    // The point is that the packaged_task version works exactly like the version using ordinary function
    // calls even when the calls of the task (here ff) and the calls of the get()s are in different threads.
    // We can concentrate on specifying the flags, rather than thinking about threads and locks.

    auto task = std::async(consumer);

    std::packaged_task<PPT> job1{f};
    std::packaged_task<PPT> job2{G{}};
    std::packaged_task<PPT> job3{h};

    std::future<Res> f1 = job1.get_future();
    auto f2 = job2.get_future();
    auto f3 = job3.get_future();

    server.put(move(job1));
    server.put(move(job2));
    server.put(move(job3));

    try {
        Res r1 = f1.get();
        assert(r1 == 3);

        auto r2 = f2.get();
        assert(r2 == -1);

        f3.get();
    }
    catch (const std::exception &e) {
        std::cout << "packaged_task error = " << e.what() << "\n\n";
    }

    task.wait();

    /**
     * future
     *
     * A future is a handle to a shared state (§42.4.1). It is where a task can retrieve
     * a result deposited by a promise (§42.4.2).
     */

    // future fu {};
    // future fu {fu2};
    // fu.~future()
    // fu=move(fu2)
    // sf=fu.share()
    // x=fu.get()
    // fu.get()
    // fu.valid()
    // fu.wait()
    // fs=fu.wait_for(d)
    // fs=fu.wait_until(tp)

    std::promise<int> prom;
    prom.set_value(1);

    auto future = prom.get_future();

    assert(future.valid());

    assert(future.get() == 1);

    assert(!future.valid());

    try {
        future.get();
    }
    catch (const std::future_error &e) {
        std::cout << "future error = " << e.what() << "\n";
    }

    try {
        future.wait();
    }
    catch (const std::future_error &e) {
        std::cout << "future error = " << e.what() << "\n";
    }

    std::promise<void> prom2;
    prom2.set_exception(make_exception_ptr(std::runtime_error{"err"}));
    try {
        prom2.get_future().get();
    }
    catch (const std::runtime_error &e) {
        std::cout << "future error = " << e.what() << "\n";
    }

    // enum class future_status
    //
    // ready        The future has a value
    // timeout      The operation timed out
    // deferred     The execution of the future's task is deferred until a get()

    auto future2 = std::async([] { std::this_thread::sleep_for(std::chrono::milliseconds{100}); });

    std::future_status status = future2.wait_for(std::chrono::milliseconds{10});
    if (status == std::future_status::timeout) {
        std::cout << "future timeout\n";
    }

    // future Errors: future_errc
    //
    // broken_promise			    A promise abandoned the state before supplying a value
    // future_already_retrieved	    A second get() on a future
    // promise_already_satisfied	A second set_value() or set_exception() on a promise()
    // no_state			            An operation tried to access a promise's shared state
    // 				                before that state was created (e.g., get_future() or set_value())

    /**
     * future: wait_for_all
     */

    using task_type = int(int, int);
    using packaged_task_type = std::packaged_task<task_type>;

    auto sum = [](int a, int b) {
        std::this_thread::sleep_for(std::chrono::milliseconds{250});
        return a + b;
    };

    auto diff = [](int a, int b) {
        std::this_thread::sleep_for(std::chrono::milliseconds{50});
        return a - b;
    };

    auto task2 = std::packaged_task<task_type>(sum);
    auto task3 = std::packaged_task<task_type>(diff);

    std::vector<std::unique_ptr<packaged_task_type>> tasks;
    tasks.push_back(std::make_unique<packaged_task_type>(move(task2)));
    tasks.push_back(std::make_unique<packaged_task_type>(move(task3)));

    std::vector<std::future<int>> futures;
    for (auto &t : tasks) {
        (*t)(1, 2);
        futures.push_back((*t).get_future());
    }

    auto all_results = wait_for_all(futures);
    assert(all_results[0] == 3);
    assert(all_results[1] == -1);

    /**
     * future: wait_for_any
     */

    auto task4 = std::packaged_task<task_type>(sum);
    auto task5 = std::packaged_task<task_type>(diff);

    std::vector<std::unique_ptr<packaged_task_type>> tasks_any;
    tasks_any.push_back(std::make_unique<packaged_task_type>(move(task4)));
    tasks_any.push_back(std::make_unique<packaged_task_type>(move(task5)));

    std::vector<std::future<int>> futures_any;
    for (auto &t : tasks_any) {
        (*t)(1, 2);
        futures_any.push_back((*t).get_future());
    }

    auto any_result = wait_for_any(futures_any, std::chrono::milliseconds{1000});
    assert(any_result == 0);
    assert(futures_any[any_result].get() == 3);

    /**
     * shared_future
     *
     * The result value of a future can be read only once: it is moved. Thus, if you want to read the value
     * repeatedly or potentially have it read by multiple readers, you must copy it, and then read the copy.
     * That's what a shared_future does. Every usable shared_future is directly or indirectly initialized by
     * moving the value out of a future with the same result type.
     *
     * Obviously, shared_future is very similar to future. The key difference is that a shared_future moves
     * its value to a location where it can be repeatedly read and shared.
     */

    // shared_future sf {};
    // shared_future sf {fu};
    // shared_future sf {sf2};
    // sf.~future()
    // sf=sf2
    // sf=move(sf2)
    // x=sf.g et()
    // sf.g et()
    // sf.valid()
    // sf.wait()
    // fs=sf.wait_for(d)
    // fs=sf.wait_until(tp)

    /**
     * async()
     *
     * Given future and promise (§42.4.1) and packaged_task (§42.4.3), we can write simple flags without
     * worrying too much about threads.
     *
     * Given those, a thread is just something you give a task to run.
     *
     * However, we still need to consider how many thread s to use and whether a task is best run on the
     * current thread or on another. Such decisions can be delegated to a thread launcher, that is,
     * a function that decides whether to create a new thread, to recycle an old thread, or simply run
     * the task on the current thread.
     *
     * The async() function is basically a simple interface to a launcher of unknown sophistication.
     * A call of async() returns a future<R> where R is the type of its task's result.
     */

    // Asynchronous Task Launcher: async<F,Args>()
    // fu=async(policy,f,args)      Execute f(args) according to launch policy policy
    // fu=async(f,args)             fu=async(launch::async|launch::deferred,f,args)

    std::future<double> fd = std::async(square, 2);
    double d = fd.get();
    assert(d == 4);

    auto fd2 = std::async(square, 2);
    auto d2 = fd2.get();
    assert(d2 == 4);

    // Launch Policies: launch
    // async        Execute the task as if a new thread was created to do so
    // deferred     Execute the task at the point of a get() for the task's future
}

int task(int a, int b) { return a + b; }

void sum(std::promise<int> &&promise, int x, int y) { promise.set_value(x + y); }

int ff(int i)
{
    if (i) return i;
    throw std::runtime_error{"ff(0)"};
}

Res f(Args a, Args b) { return a + b; }

Res G::operator()(Args a, Args b) { return a - b; }

template <typename T>
void Sync_queue<T>::put(T &&val)
{
    std::lock_guard<std::timed_mutex> lck{mtx};
    q.push_back(move(val));
    cond.notify_one();
}

template <typename T>
void Sync_queue<T>::get(T &val)
{
    std::unique_lock<std::timed_mutex> lck{mtx, std::chrono::milliseconds{10}};
    cond.wait(lck, [this] { return !q.empty(); });

    val = move(q.front());
    q.pop_front();
}

template <typename T>
size_t Sync_queue<T>::size() const
{
    return q.size();
}

void consumer()
{
    while (true) {
        std::packaged_task<PPT> func;
        server.get(func);
        func(1, 2);

        if (server.size() == 0) {
            break;
        }
    }
}

template <typename T>
std::vector<T> wait_for_all(std::vector<std::future<T>> &vf)
{
    std::vector<T> res;
    res.reserve(vf.size());
    for (auto &fu : vf) {
        res.push_back(fu.get());
    }
    return res;
}

template <typename T>
int wait_for_any(std::vector<std::future<T>> &vf, std::chrono::steady_clock::duration d)
{
    while (true) {
        for (size_t i = 0; i != vf.size(); ++i) {
            if (!vf[i].valid()) {
                continue;
            }

            switch (vf[i].wait_for(std::chrono::seconds{0})) {
                case std::future_status::ready:
                    return i;
                case std::future_status::timeout:
                    break;
                case std::future_status::deferred:
                    throw std::runtime_error{"wait_for_any(): deferred future"};
            }
        }

        std::this_thread::sleep_for(d);
    }

    // Note the check for valid(). Trying wait_for() on an invalid future (e.g., a future on which you have
    // already done a get()) will cause a hard-to-find error. At best, you can hope for a (probably surprising)
    // exception being thrown.
}
