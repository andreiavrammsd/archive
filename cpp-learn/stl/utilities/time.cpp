/*
 The C++ Programming Language, 4th Edition
 */

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    /**
     * duration
     */

    std::chrono::duration<long long, std::milli> d1{7};  // 7 milliseconds
    assert(d1.count() == 7);

    std::chrono::duration<double, std::pico> d2{3.33};  // 3.33 picoseconds
    assert(d2.count() == 3.33);

    d2 = d1;
    assert(d2 == d1);
    assert(d2.count() == 7e9);

    std::chrono::duration<int, std::ratio<1, 1>> d3{};  // 0 seconds
    assert(d3.count() == 0);

    std::chrono::duration<int, std::milli> d{3};  // OK
    // std::chrono::duration<int, std::milli> d{3.5}; // error: 3.5 to int is narrowing

    std::chrono::duration<int, std::milli> ms{3};
    std::chrono::duration<int, std::micro> us{ms};  // OK
    // std::chrono::duration<int, std::milli> ms2{us}; // error: we could lose many microseconds
    assert(ms == us);

    assert(d.count() == 3);

    ++d;
    assert(d.count() == 4);

    d += d1;
    assert(d.count() == 11);

    d += decltype(d)(5);  // OK
    assert(d.count() == 16);
    // d += 5; // error

    assert(d != std::chrono::steady_clock::duration::zero());
    assert(d3 == std::chrono::steady_clock::duration::zero());
    std::cout << std::chrono::steady_clock::duration::min().count() << '\n';
    std::cout << std::chrono::steady_clock::duration::max().count() << '\n';
    assert(d > d3);

    // The standard library provides some convenience aliases using the SI units from <ratio>
    // using nanoseconds = duration<si64,nano>;
    // using microseconds = duration<si55,micro>;
    // using milliseconds = duration<si45,milli>;
    // using seconds = duration<si35>;
    // using minutes = duration<si29,ratio<60>>;
    // using hours = duration<si23,ratio<3600>>;

    int x{};
    auto f = [](int) { std::this_thread::sleep_for(std::chrono::milliseconds(10)); };

    auto t1 = std::chrono::system_clock::now();
    f(x);  // do something
    auto t2 = std::chrono::system_clock::now();

    auto dms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
    std::cout << "f(x) took " << dms.count() << " milliseconds\n";

    /**
     * time_point
     *
     * In <chrono>, the standard library provides type epoch as measured by a given clock
     */

    std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> tp{};
    assert(tp.time_since_epoch() == std::chrono::steady_clock::duration::zero());

    // https://en.cppreference.com/w/cpp/chrono/time_point/time_since_epoch
    const auto p0 = std::chrono::time_point<std::chrono::system_clock>{};
    const auto p1 = std::chrono::system_clock::now();
    const auto p2 = p1 - std::chrono::hours{24};

    std::time_t epoch_time = std::chrono::system_clock::to_time_t(p0);
    std::cout << "epoch: " << std::ctime(&epoch_time);
    std::time_t today_time = std::chrono::system_clock::to_time_t(p1);
    std::cout << "today: " << std::ctime(&today_time);

    std::cout << "hours since epoch: " << std::chrono::duration_cast<std::chrono::hours>(p1.time_since_epoch()).count()
              << '\n';
    std::cout << "yesterday, hours since epoch: "
              << std::chrono::duration_cast<std::chrono::hours>(p2.time_since_epoch()).count() << '\n';

    auto tpn = std::chrono::steady_clock::now();
    std::cout << tpn.time_since_epoch().count() << '\n';

    auto d1n = std::chrono::time_point_cast<std::chrono::hours>(tp).time_since_epoch().count() /
               24;  // days since start of epoch
    std::cout << d1n << '\n';

    using days = std::chrono::duration<long, std::ratio<24 * 60 * 60, 1>>;         // a day's duration
    auto d2n = std::chrono::time_point_cast<days>(tp).time_since_epoch().count();  // days since start of epoch
    std::cout << d2n << '\n';

    /**
     * Clocks
     *
     * The time_point and duration values_ are ultimately obtained from hardware clocks.
     * In <chrono>, the standard library provides basic interfaces for clocks.
     *
     * system_clock: The system's real-time clock; the system's clock may be reset
     * (jump forward or backward) to match external clocks
     *
     * steady_clock: A clock where time moves steadily forward; that is, time does not
     * go backward and the time between clock ticks is constant
     *
     * high_resolution_clock: A clock with the shortest time increment on a system
     *
     * These three clocks need not be different; a standard-library clock name may be an alias.
     */

    // We can determine the basic properties of the clocks like this:
    std::cout << "min " << std::chrono::system_clock::duration::min().count() << ", max "
              << std::chrono::system_clock::duration::max().count() << ", "
              << (std::chrono::treat_as_floating_point<std::chrono::system_clock::duration>::value ? "FP" : "integral")
              << '\n';

    std::cout << (std::chrono::system_clock::is_steady ? "steady\n" : "not steady\n");

    /**
     * Time Traits
     */

    std::cout << std::chrono::high_resolution_clock::duration::zero().count() << '\n';
    std::cout << std::chrono::high_resolution_clock::duration::min().count() << '\n';
    std::cout << std::chrono::high_resolution_clock::duration::max().count() << '\n';
}
