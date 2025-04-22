/*
 The C++ Programming Language, 4th Edition
 https://en.cppreference.com/w/cpp/17
 */

#include <atomic>
#include <thread>
#include <cassert>
#include <mutex>
#include <complex>

int main()
{
    // memory_order_relaxed
    // memory_order_consume
    // memory_order_acquire
    // memory_order_release
    // memory_order_acq_rel
    // memory_order_seq_cst

    {
        std::atomic<int> r1{};
        std::atomic<int> x{};

        std::atomic<int> r2{};
        std::atomic<int> y{};

        std::thread th1{[](std::atomic<int> &r1, std::atomic<int> &x, std::atomic<int> &y) {
                            r1 = y.load(std::memory_order_relaxed);
                            x.store(r1, std::memory_order_relaxed);
                        },
                        std::ref(r1), std::ref(x), std::ref(y)};

        std::thread th2{[](std::atomic<int> &r2, std::atomic<int> &x, std::atomic<int> &y) {
                            r2 = x.load(std::memory_order_relaxed);
                            y.store(42, std::memory_order_relaxed);
                        },
                        std::ref(r2), std::ref(x), std::ref(y)};

        th1.detach();
        th2.join();
    }

    // This is allowed to produce r2==42 , making it appear that time went backward in thread 2. That is,
    // memory_order_relaxed allows this execution order:
    // y.store(42,memory_order_relaxed);
    // r1 = y.load(memory_order_relaxed);
    // x.store(r1,memory_order_relaxed);
    // r2 = x.load(memory_order_relaxed);

    // To allow significant optimizations for architectures with relaxed memory models, the standard
    // provides an attribute [[carries_dependency]] for transmitting memory order dependencies across
    // function calls (§iso.7.6.4).

    /*
    [[carries_dependency]] struct foo *f(int i) {
        // let the caller use memory_order_consume for the result:
        return foo_head[i].load(std::memory_order_consume);
    }
     */

    // You can also put [[carries__dependency]] on function arguments, and there is a function kill_dependency()
    // for stopping the propagation of such dependencies.

    /**
     * atomic Types
     */

    // atomic<T>
    // atomic x;
    // atomic x {};
    // atomic x {t};

    // x=t
    // t=x
    // x.is_lock_free()
    // x.store(t)
    // x.store(t,order)
    // t=x.load()
    // t=x.load(order)
    // t2=x.exchange(t)
    // t2=x.exchange(t,order)
    // b=x.compare_exchange_weak(rt,t)
    // b=x.compare_exchange_weak(rt,t,o1,o2)
    // b=x.compare_exchange_weak(rt,t,order)
    // b=x.compare_exchange_strong(rt,t,o1,o2)
    // b=x.compare_exchange_strong(rt,t,order)
    // b=x.compare_exchange_strong(rt,t)

    std::atomic<int> a{};

    a = 2;
    assert(a == 2);

    a.store(3);
    assert(a == 3);

    assert(a.load() == 3);

    assert(a.is_lock_free());

    std::atomic<int> b = 1;

    auto c = a.exchange(b);
    assert(c == 3);
    assert(a == 1);

    c = a.exchange(b);
    assert(c == 1);
    assert(a == 1);

    a = 1;
    b = 2;
    int ax{};
    c = a.compare_exchange_weak(ax, b);
    assert(a == 1);
    assert(b == 2);
    assert(ax == 1);
    assert(c == 0);
    c = a.compare_exchange_weak(ax, b);
    assert(a == 2);
    assert(b == 2);
    assert(ax == 1);
    assert(c == 1);

    // atomic<T> for Integral T
    // z=x.fetch_add(y)
    // z=x.fetch_add(y,order)
    // z=x.fetch_sub(y)
    // z=x.fetch_sub(y,order)
    // z=x.fetch_and(y)
    // z=x.fetch_and(y,order)
    // z=x.fetch_or(y)
    // z=x.fetch_or(y,order)
    // z=x.fetch_xor(y)
    // z=x.fetch_xor(y,order)
    // ++x
    // x++
    // −−x
    // x−−
    // x+=y
    // x−=y
    // x&=y
    // x|=y
    // x^=y

    a = 1;
    b = 2;

    c = a.fetch_add(b);
    assert(c == 1);
    assert(a == 3);
    assert(b == 2);

    assert(++c == 2);

    // double-checked locking idiom
    struct X {
    };
    X x;                              // we need a lock to initialize an X
    std::mutex lx;                    // the mutex to be used to lock x during initialization
    std::atomic<bool> x_init{false};  // an atomic used to minimize locking

    {
        if (!x_init) {  // proceed if x is uninitialized
            lx.lock();

            if (!x_init) {  // proceed if x is still uninitialized
                x = X{};    // initialize x
                x_init = true;
            }

            lx.unlock();
        }

        // use x
    }

    // The double-checked locking idiom is represented in the standard library by once_flag and
    // call_once()

    // atomic<T*> for Pointers
    // z=x.fetch_add(y)
    // z=x.fetch_add(y,order)
    // z=x.fetch_sub(y)
    // z=x.fetch_sub(y,order)
    // ++x
    // x++
    // --x
    // x--
    // x+=y
    // x-=y

    // To allow the C standard library to be compatible, the atomic member function
    // types have freestanding equivalents:
    // atomic_is_lock_free(p)
    // atomic_init(p,v)
    // atomic_store(p,v)
    // x=atomic_load(p)
    // x=atomic_load(p)
    // b=atomic_compare_exchang e_weak(p,q,v)
    // and other

    // is_always_lock_free (C++17)
    static_assert(std::atomic<int>::is_always_lock_free);
    struct Atom {
        int i;
    };
    static_assert(std::atomic<Atom>::is_always_lock_free);
    static_assert(std::atomic<std::complex<int>>::is_always_lock_free);

    /**
     * Flags and Fences
     *
     * The primary use of these is to implement the lowest-level
     * atomic facilities, such as spinlocks and the atomic types.
     */

    // atomic Flags
    //
    // An atomic_flag is the simplest atomic type and the only one with operations guaranteed to be
    // atomic for every implementation.

    // atomic_flag fl;
    // atomic_flag fl {};
    // atomic_flag fl {ATOMIC_FLAG_INIT};
    // b=fl.test_and_set()
    // b=fl.test_and_set(order)
    // fl.clear()
    // fl.clear(order)
    // b=atomic_flag_test_and_set(flp)
    // b=atomic_flag_test_and_set_explicit(flp,order)
    // atomic_flag_clear(flp)
    // atomic_flag_clear_explicit(flp,order)

    // Using {} to initialize atomic_flag seems to make sense. However, there is no guarantee that 0
    // represents clear. A machine where clear is 1 is rumored to exist. Clearing using ATOMIC_FLAG_INIT
    // is the only portable and reliable way of initializing an atomic_flag. The ATOMIC_FLAG_INIT is an
    // implementation-supplied macro.

    class spin_mutex {
        std::atomic_flag flag = ATOMIC_FLAG_INIT;

       public:
        void lock()
        {
            while (flag.test_and_set())
                ;
        }

        void unlock() { flag.clear(); }
    };

    // Fences
    //
    // A fence, also known as a memory barrier, is an operation that restricts operation reordering
    // according to some specified memory ordering (§41.2.3).

    // atomic_thread_fence(order)
    // atomic_signal_fence(order)

    /**
     * volatile
     *
     * The volatile specifier is used to indicate that an object can be modified by something external to the
     * thread of control. For example:
     *      volatile const long clock_register; // updated by the hardware clock
     *
     * A volatile specifier basically tells the compiler not to optimize away apparently redundant reads and
     * writes.
     *      auto t1 {clock_register};
     *      // ... no use of clock_register here ...
     *      auto t2 {clock_register};
     * Had clock_register not been volatile, the compiler would have been perfectly entitled to eliminate
     * one of the reads and assume t1==t2.
     * Do not use volatile except in low-level code that deals directly with hardware.
     * A C++ volatile is not a synchronization mechanism
     */
}
