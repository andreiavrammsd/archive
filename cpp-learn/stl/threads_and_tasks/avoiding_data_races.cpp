/*
 The C++ Programming Language, 4th Edition
 */

#include <mutex>
#include <thread>
#include <iostream>
#include <list>
#include <atomic>
#include <vector>
#include <string>
#include <condition_variable>

std::mutex cout_mutex; // represent the right to use cout

template<typename Arg1, typename Arg2, typename Arg3>
void write(Arg1 a1, Arg2 a2 = {}, Arg3 = {});

struct Work {
    int i{};
};
std::mutex wqm;
std::list<Work> wq;

std::atomic<bool> stop = false;

void composer();

void reader();

void request_stop(std::chrono::seconds);

template<typename T>
void write2(T value);

template<typename Arg, typename... Args>
void write2(Arg a, Args ...tail);

std::recursive_mutex cout_recursive_mutex;

template<typename T>
void write3(T value);

template<typename Arg, typename... Args>
void write3(Arg a, Args ...tail);

void use(std::mutex &mtx, std::vector<std::string> &vs, int i);

void use_better(std::mutex &mtx, std::vector<std::string> &vs, int i);

struct Y {
};

class X {
public:
    X();

private:
    static std::once_flag static_flag;
    static Y static_data_for_class_X;

    static void init();
};

X::X() {
    std::cout << "construct multiples times\n";
    std::call_once(static_flag, init);
}

std::once_flag X::static_flag;

void X::init() {
    std::cout << "init once\n";
    static_data_for_class_X = Y{};
}

void simple_timer(int delay);

template<typename T>
class Sync_queue {
public:
    void put(const T &val);

    void put(T &&val);

    void get(T &val);

    size_t size() const;

private:
    std::mutex mtx;
    std::condition_variable cond;
    std::list<T> q;
};

struct Message {
    long i{};
};

Sync_queue<Message> mq;

void producer();

void consumer();

int main() {
    // mutex
    // recursive_mutex
    // timed_mutex
    // recursive_timed_mutex
    // lock_guard<M>
    // unique_lock<M>

    std::thread th1{write<int, std::string, double>, 1, "2", 3.0};
    std::thread th2{write<int, std::string, double>, 4, "5", 6.0};

    th1.join();
    th2.join();

    /**
     * mutex and recursive_mutex
     */

    // mutex m {};
    // m.~mutex()
    // m.lock()
    // m.try_lock()
    // m.unlock()
    // native_handle_type
    // nh=m.native_handle()

    // The try_lock() operation is used when we have some other work we might usefully do if some
    // other thread is using a resource.
    std::thread th3{composer};
    std::thread th4{reader};
    std::thread th5{request_stop, std::chrono::seconds{1}};

    th3.join();
    th4.join();
    th5.join();

    // When using locks, we have to bew are of deadlock. That is, we must not wait for a lock that can
    // never be released. The simplest form of deadlock requires only one lock and one thread.
    std::cout << '\n';
    // write2(1, 2, 3); // deadlock
    write3(1, 2, 3);

    /**
     * mutex Errors
     *
     * Trying to manipulate a mutex can fail. If so, the mutex operation throws a system_error.
     */

    // resource_deadlock_would_occur
    // resource_unavailable_tr y_again
    // operation_not_permitted
    // device_or_resource_busy
    // invalid_argument

    std::cout << '\n';

    std::mutex mtx;
    try {
        mtx.lock();
        // mtx.lock(); // try to lock a second time
    } catch (const std::system_error &e) {
        mtx.unlock();
        std::cout << e.what() << ", " << e.code() << '\n';
    }

    /**
     * timed_mutex and recursive_timed_mutex
     */

    // timed_mutex m {};
    // m.~timed_mutex()
    // m.lock()
    // m.try_lock()
    // m.try_lock_for(d)
    // m.try_lock_until(tp)
    // m.unlock()
    // native_handle_type
    // nh=m.native_handle()

    std::timed_mutex tmtx;
    if (tmtx.try_lock_for(std::chrono::milliseconds{10})) {
        std::cout << "\ntimed mutex...\n";
    }
    tmtx.unlock();

    // As an example, consider updating an output buffer with a new image (e.g., in a video game
    // or a visualization)
    /*
        extern timed_mutex imtx;
        extern Image buf;

        void next()
        {
            while (true) {
                Image next_image;
                // ... compute ...

                if (imtx.try_lock(milliseconds{100})) {
                    buf = next_image;
                    imtx.unlock();
                }
            }
        }
     */

    /**
     * lock_guard and unique_lock
     *
     * A lock is a resource, so we must not forget to release it. That is, each m.lock() operation must be
     * matched by an m.unlock() operation.
     */

    // lock_guard lck {m};
    // lock_guard lck {m,adopt_lock_t};
    // lck.~lock_guard()

    std::mutex lck_mtx;
    std::vector<std::string> vs{"abc"};

    use(lck_mtx, vs, -1);
    use(lck_mtx, vs, 0);

    use_better(lck_mtx, vs, -1);
    use_better(lck_mtx, vs, 0);

    // unique_lock lck {};
    // unique_lock lck {m};
    // unique_lock lck {m,defer_lock_t};
    // unique_lock lck {m,try_to_lock_t};
    // unique_lock lck {m,adopt_lock_t};
    // unique_lock lck {m,tp};
    // unique_lock lck {m,d};
    // unique_lock lck {lck2};
    // lck.~unique_lock()
    // lck2=move(lck)
    // lck.lock()
    // lck.tr y_lock()
    // lck.tr y_lock_for(d)
    // lck.tr y_lock_until(tp)
    // lck.unlock()
    // lck.swap(lck2)
    // pm=lck.release()
    // lck.owns_lock()
    // bool b {lck};
    // pm=lck.mutex()
    // swap(lck,lck2)

    std::mutex lmtx;
    lmtx.lock();
    lmtx.unlock();

    std::timed_mutex ltmtx;

    {
        // error: mutex does not have member try_lock_for()
        // std::unique_lock<std::mutex> lck{lmtx, std::chrono::milliseconds{3}};
    }

    {
        std::unique_lock<std::timed_mutex> lck2{ltmtx, std::defer_lock};
        (void)lck2.try_lock_for(std::chrono::milliseconds{300});

        if (lck2.owns_lock()) {
            std::cout << "\nacquisition succeeded: do something\n";
        }

        try {
            (void)lck2.try_lock_for(std::chrono::milliseconds{300});
        } catch (const std::system_error &e) {
            std::cout << "error = " << e.what() << '\n';
        }
    }

    /**
     * Multiple Locks
     */

    // x=try_lock(locks)
    // lock(locks)

    {
        std::mutex m1{};
        std::unique_lock<std::mutex> l1{m1, std::defer_lock};

        std::mutex m2{};
        std::unique_lock<std::mutex> l2{m2, std::defer_lock};

        std::lock(l1, l2);
    }

    // since C++17
    {
        std::mutex lmtx1;
        std::mutex lmtx2;
        std::scoped_lock lck{lmtx1, lmtx2};
    }

    /**
     * call_once()
     *
     * We often want to initialize an object without getting into a race condition.
     */

    // once_flag fl {};
    // call_once(fl,f,args)

    std::cout << '\n';
    X a;
    X b;

    /**
     * Condition Variables
     *
     * A thread can wait (block) on a condition_variable until some event
     */

    // condition_variable cv {};
    // cv.~condition_variable()
    // cv.notify_one()
    // cv.notify_all()
    // cv.wait(lck)
    // cv.wait(lck,pred)
    // x=cv.wait_until(lck,tp)
    // b=cv.wait_until(lck,tp,pred)
    // x=cv.wait_for(lck,d)
    // b=cv.wait_for(lck,d,pred)
    // native_handle_type
    // nh=cv.native_handle()

    std::cout << '\n';
    simple_timer(100);

    std::cout << '\n';

    std::thread t1{producer};
    std::thread t2{consumer};
    std::thread t3{consumer};

    t1.join();
    t2.join();
    t3.join();

    // condition_variable_any
    //
    // A condition_variable is optimized for unique_lock<mutex>. A condition_variable_any is functionally
    // equivalent to a condition_variable but can use any lockable object for its operations
}

template<typename Arg1, typename Arg2, typename Arg3>
void write(Arg1 a1, Arg2 a2, Arg3 a3) {
    std::thread::id name = std::this_thread::get_id();

    cout_mutex.lock();
    std::cout << "From thread " << name << ": " << a1 << a2 << a3 << '\n';
    cout_mutex.unlock();
}

void composer() {
    std::list<Work> requests;

    while (true) {
        for (int i = 0; i != 10; ++i) {
            Work w{i};
            // ... compose work request ...
            requests.push_back(w);
        }

        if (wqm.try_lock()) {
            wq.splice(wq.begin(), requests); // splice() requests into the list (§31.4.2)
            wqm.unlock();
        }

        if (stop) {
            break;
        }
    }
}

void reader() {
    while (true) {
        if (wqm.try_lock()) {
            if (wq.empty()) {
                continue;
            }

            auto w = wq.back();
            std::cout << "working:" << w.i << '\n';
            wq.pop_back();

            wqm.unlock();
        } else {
            std::cout << "doing something else...\n";
        }

        if (stop) {
            break;
        }
    }
}

void request_stop(std::chrono::seconds s) {
    std::this_thread::sleep_for(s);
    stop = true;
}

template<typename T>
void write2(T value) {
    std::cout << value;
}

template<typename Arg, typename... Args>
void write2(Arg a, Args ...tail) {
    cout_mutex.lock();
    std::cout << a;
    write2(tail...);
    cout_mutex.unlock();
}

template<typename T>
void write3(T value) {
    std::cout << value;
}

template<typename Arg, typename... Args>
void write3(Arg a, Args ...tail) {
    cout_recursive_mutex.lock();
    std::cout << a;
    write3(tail...);
    cout_recursive_mutex.unlock();
}

void use(std::mutex &mtx, std::vector<std::string> &vs, int i) {
    std::lock_guard<std::mutex> g{mtx};

    if (i < 0) {
        return; // The lock_guard's destructor does the necessary unlock() on its argument.
    }

    std::string s = vs[i];
}

void use_better(std::mutex &mtx, std::vector<std::string> &vs, int i) {
    if (i < 0) {
        return; // The lock_guard's destructor does the necessary unlock() on its argument.
    }

    std::string s;
    // Minimizing the size of critical sections is in general a useful thing to do.
    {
        std::lock_guard<std::mutex> g{mtx};
        s = vs[i];
    }
}

void simple_timer(int delay) {
    std::condition_variable timer;
    std::mutex mtx; // mutex protecting timer

    auto t0 = std::chrono::steady_clock::now();

    std::unique_lock<std::mutex> lck{mtx}; // acquire mtx
    timer.wait_for(lck, std::chrono::milliseconds{delay}); // release and reacquire mtx

    auto t1 = std::chrono::steady_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << " milliseconds passed\n";
} // implicitly release mtx

template<typename T>
void Sync_queue<T>::put(const T &val) {
    std::lock_guard<std::mutex> lck{mtx};
    q.push_back(val);
    cond.notify_one();
}

template<typename T>
void Sync_queue<T>::get(T &val) {
    std::unique_lock<std::mutex> lck{mtx};
    cond.wait(lck, [this] { return !q.empty(); });

    val = q.front();
    q.pop_front();
}

template<typename T>
size_t Sync_queue<T>::size() const {
    return q.size();
}

void producer() {
    while (true) {
        Message m{std::chrono::steady_clock::now().time_since_epoch().count()};
        mq.put(m);

        if (mq.size() == 10) {
            break;
        }
    }
}

void consumer() {
    while (true) {
        Message m;
        mq.get(m);
        std::cout << m.i << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds{100});

        if (mq.size() == 0) {
            break;
        }
    }
}
