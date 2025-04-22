#include <iostream>

#include "benchmark/benchmark.h"

// !! -> https://godbolt.org/z/6ev3qK
// https://godbolt.org/z/vq3Pon

inline double f1(int a0, int b0, double c0, bool d0,
                 int a1, int b1, double c1, bool d1,
                 int a2, int b2, double c2, bool d2) {
    double s = a0 + b0 + c0 + d0 +
               a1 + b1 + c1 + d1 +
               a2 + b2 + c2 + d2;
    return s;
}

constexpr int kThreads = 1;
constexpr benchmark::IterationCount kIterations = 2000000;
constexpr int kRepetitions = 100;
constexpr bool kReportAggregatesOnly = true;

static void BM_params(benchmark::State &state) {
    volatile int a = 1;
    volatile int b = 2;
    volatile double c = 3;
    volatile bool d = true;
//    std::cin >> a >> b >> c >> d;

    for (auto _ : state) {
        benchmark::DoNotOptimize(f1(a, b, c, d, a, b, c, d, a, b, c, d));
    }
}

struct input {
    int a0;
    int b0;
    double c0;
    bool d0;
    int a1;
    int b1;
    double c1;
    bool d1;
    int a2;
    int b2;
    double c2;
    bool d2;
};

//struct input {
//    double c0;
//    double c1;
//    double c2;
//    int a0;
//    int b0;
//    int a1;
//    int b1;
//    int a2;
//    int b2;
//    bool d0;
//    bool d1;
//    bool d2;
//};

inline double f2(input in) {
    double s = in.a0 + in.b0 + in.c0 + in.d0 +
               in.a1 + in.b1 + in.c1 + in.d1 +
               in.a2 + in.b2 + in.c2 + in.d2;
    return s;
}


static void BM_struct(benchmark::State &state) {
    volatile int a = 1;
    volatile int b = 2;
    volatile double c = 3;
    volatile bool d = true;
//    std::cin >> a >> b >> c >> d;

    input in{a, b, c, d, a, b, c, d, a, b, c, d};
//    input in{c, c, c, a, b, a, b, a, b, d, d, d};
    for (auto _ : state) {
        benchmark::DoNotOptimize(f2(in));
    }
}

BENCHMARK(BM_params)
        ->UseRealTime()
        ->MeasureProcessCPUTime()
        ->Threads(kThreads)
        ->Iterations(kIterations)
        ->Repetitions(kRepetitions)
        ->ReportAggregatesOnly(kReportAggregatesOnly);

BENCHMARK(BM_struct)
        ->UseRealTime()
        ->MeasureProcessCPUTime()
        ->Threads(kThreads)
        ->Iterations(kIterations)
        ->Repetitions(kRepetitions)
        ->ReportAggregatesOnly(kReportAggregatesOnly);

BENCHMARK_MAIN();
