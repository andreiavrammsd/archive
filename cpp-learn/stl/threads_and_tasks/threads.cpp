/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <iostream>
#include <thread>
#include <utility>
#include <vector>

void f0() {}     // no arguments
void f1(int) {}  // one int argument

template <typename T>
class Sync_queue {  // a queue providing put() and get() without data races (§42.3.4)
};

struct Message;

struct Consumer {
    Sync_queue<Message> &head;

    Consumer(Sync_queue<Message> &q) : head{q} {}

    void operator()(){};  // get messages from head
};

struct Producer {
    Sync_queue<Message> &tail;

    Producer(Sync_queue<Message> &q) : tail{q} {}

    void operator()(){};  // put messages on tail
};

void my_task(std::vector<double> &) {}

void my_const_task(const std::vector<double> &) {}

void tick(int);

struct guarded_thread : std::thread {
    using std::thread::thread;

    ~guarded_thread()
    {
        if (joinable()) {
            join();
        }
    }
};

template <typename K, typename V>
class Map {
   public:
    Map() = default;

    static void set_default(const K &k, V &v)
    {  // set default for all Maps of type Map<K,V>
        default_value.first = k;
        default_value.second = v;
    }

   private:
    static thread_local std::pair<K, V> default_value;
};

template <typename K, typename V>
thread_local std::pair<K, V> Map<K, V>::default_value = {};

int main()
{
    /**
     * Thread
     */

    // id
    // native_handle_type
    // thread t {};
    // thread t {t2};
    // thread t {f,args};
    // t.~thread();
    // t=move(t2)
    // t.swap(t2)
    // t.joinable()
    // t.join()
    // t.detach()
    // x=t.get_id()
    // x=t.native_handle()
    // n=hardware_concurrency()
    // swap(t,t2)

    std::cout << "hardware_concurrency = " << std::thread::hardware_concurrency() << '\n';

    /**
     * Identity
     */
    std::thread th{};
    assert(th.get_id() == std::thread::id{});

    // A thread can have its id be id{} if
    //  it has not had a task assigned,
    //  it has terminated,
    //  it has been moved from, or
    //  it has been detach()ed.

    th = std::thread{[] {
        assert(std::this_thread::get_id() != std::thread::id{});
        std::cout << "thread id = " << std::this_thread::get_id() << '\n';
    }};

    th.join();

    /**
     * Construction
     */
    std::thread t1{f0};
    t1.join();
    // std::thread t2{f0, 1}; // error: too many arguments
    // std::thread t3{f1}; // error: too few arguments
    std::thread t4{f1, 1};
    t4.join();

    // After construction, a thread starts executing its task as soon as the run-time system can acquire
    // resources for it to run. Think of that as "immediately." There is no separate "start the thread"
    // operation.

    // If you want to build a set of flags and link them together (e.g., to communicate through message queues),
    // you first build the flags as function objects and then - when they are all ready to run - start threads.
    // Trying to intersperse thread creation with the setup of connections among the flags to be run by the
    // threads can easily become complicated and error-prone.

    Sync_queue<Message> mq;
    Consumer c{mq};  // make flags and "wire them together"
    Producer p{mq};

    std::thread pro{p};  // finally: start threads
    std::thread con{c};

    pro.join();
    con.join();

    // The thread constructors are variadic templates (§28.6). This implies that to pass a reference to a
    // constructor, we must use a reference wrapper (§33.5.1).
    std::vector<double> v;
    // std::thread my_thread_1{my_task, v}; // oops: pass a copy of v
    std::thread my_thread_2{my_task, std::ref(v)};             // OK: pass v by reference
    std::thread my_const_thread{my_const_task, std::cref(v)};  // OK: pass v by const reference
    std::thread my_thread_3{[&v] { my_task(v); }};             // OK: dodge the ref() problem

    my_thread_2.join();
    my_const_thread.join();
    my_thread_3.join();

    // A default-constructed thread is primarily useful as the target for a move.
    std::vector<std::thread> worker(1000);  // 1000 default threads
    for (size_t i = 0; i != worker.size(); ++i) {
        // compute argument for worker[i] and create worker thread tmp ...
        std::thread tmp{};
        worker[i] = std::move(tmp);
    }

    std::thread th2{};
    assert(!th2.joinable());
    try {
        th.join();
    }
    catch (const std::system_error &e) {
        std::cout << "not joinable errr = " << e.what() << '\n';
    }
    /**
     * Destruction
     *
     * Obviously, the thread destructor destroys the thread object. To prevent a system thread from
     * accidentally outliving its thread, the thread destructor calls terminate() to terminate the
     * program if the thread is joinable() (that is, if get_id()!=id{}).

        void hearbeat() {
            while(true) {
                output(steady_clock::now());
                this_thread::sleep_for(second{1}); // §42.2.6
            }
        }

        void run() {
            std::thread t{hearbeat};
            t.join();
        } // terminate because heartbeat() is still running at the end of t's scope

     * If you really need to have a system thread proceed beyond the lifetime of its thread see §42.2.5.
     */

    /**
     * join()
     *
     * A t.join() tells the current thread not to proceed until t completes.
     */

    std::thread timer(tick, 5);
    timer.join();

    // [...] we can forget to join() a thread. When we view a thread as a resource, we see that
    // we should consider RAII

    /*
        void run(int i, int n) // warning: really poor code
        {
            thread t1 {f};
            thread t2;
            vector<Foo> v;
            // ...
            if (i<n) {
                thread t3 {g};
            // ...
                t2 = move(t3); // move t3 to outer scope
            }
            v[i] = Foo{}; // might throw
            // ...
            t1.join();
            t2.join();
        }

        [...] several bad mistakes:
            - We may never reach the two join()s at the end. In that case, the destructor for t1 will
            terminate the program.
            - We may reach the two join()s at the end without the move t2=move(t3) having executed. In
            that case, t2.join() will terminate the program.
     */

    // For this kind of thread use, we need a destructor that implicitly join()s.
    guarded_thread gt1{f0};
    guarded_thread gt2{};

    /*
        void run2(int i, int n) // simple use of a guard
        {
            guarded_thread t1 {f};
            guarded_thread t2;
            vector<Foo> v;
            // ...
            if (i<n) {
                thread t3 {g};
                // ...
                t2 = move(t3); // move t3 to outer scope
            }
            v[i] = Foo{}; // might throw
            // ...
        }
     */

    // But why doesn't the thread's destructor just join()? There is a long-standing tradition of using
    // system threads that "live forever" or decide for themselves when to terminate. Had it worked, the
    // timer executing tick() (§42.2.2) would have been an example of such a thread. Threads monitoring
    // data structures provide many more examples. Such threads (and processes) are often called daemons.
    // Another use for detached threads is to simply initiate a thread to complete a task and forget
    // about it. Doing so leaves the "housekeeping" to the run-time system.

    /**
     * detach()
     *
     * Accidentally letting a thread try to execute beyond its destructor is considered a very bad error.
     * If you really want a system thread to outlive its thread (handle), use detach().
     */

    /*
     * I have a philosophical problem with detached threads. Given a choice, I would prefer to
        - know exactly which threads are running,
        - be able to determine if threads are making progress as expected,
        - be able to check if threads that are supposed to delete themselves really do so,
        - be able to know whether it is safe to use the results of a thread,
        - be sure that all resources associated with a thread are properly released, and
        - be sure that a thread does not try to access objects from the scope in which it was created
        after that scope has been destroyed.
     */

    std::thread d0{f0};
    d0.detach();

    std::vector<std::thread> my_threads;  // keep otherwise detached thread here

    std::thread d1{tick, 2};
    my_threads.push_back(move(d1));
    my_threads.emplace_back(tick, 3);

    for (std::thread &t : my_threads) {
        std::cout << "thread = " << t.get_id() << '\n';
        t.detach();
    }

    /**
     * Namespace this_thread
     */

    // x=get_id()
    // yield()
    // sleep_until(tp)
    // sleep_for(d)

    /**
     * Killing a thread
     *
     * There is no simple standard way of telling a running thread that I have lost interest in its task.
     *
     * There are various historical and technical reasons for the lack of this operation
     * (called kill, cancel, and interrupt in various languages and systems).
     *
     * If needed, application programmers can write their own versions of this idea. For example,
     * many flags involve a request loop. In that case, a "kindly commit suicide" message would allow
     * the receiving thread to release all resources and then terminate. If there is no request loop, a task
     * could periodically examine a "needed" variable to see if results are still wanted.
     */

    /**
     * thread_local Data
     *
     * As indicated by its name, a thread_local variable is an object owned by a thread and not accessible
     * from other threads unless its owner (incautiously) gives them a pointer to it. In that, a thread_local
     * resembles a local variable, but a local variable has its lifetime and access limited by its scope within
     * a function, whereas a thread_local is shared among all functions of a thread and "lives" for as long
     * as the thread. A thread_local object can be extern.
     */

    std::thread thl1{[] {
        int v = 1;
        Map<std::string, int>::set_default("Heraclides", v);
    }};

    std::thread thl2{[] {
        int v = 1;
        Map<std::string, int>::set_default("Zeno", v);
    }};

    thl1.join();
    thl2.join();
}

void tick(int n)
{
    for (int i = 0; i != n; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
        std::cout << "Alive\n";
    }
}
