#include <array>
#include <cstdint>
#include <tuple>

#include "benchmark/benchmark.h"
#include "dynamic_tasks.hpp"
#include "static_executor.hpp"
#include "static_tasks.hpp"

/**
    Results on release build with CPU scaling disabled

    2020-09-18 17:41:43
    Running executor_benchmark
    Run on (8 X 4000 MHz CPU s)
    CPU Caches:
      L1 Data 32K (x4)
      L1 Instruction 32K (x4)
      L2 Unified 256K (x4)
      L3 Unified 8192K (x1)
    Load Average: 0.66, 1.26, 1.31
 */

constexpr int kThreads = 1;
constexpr benchmark::IterationCount kIterations = 2000000;
constexpr int kRepetitions = 100;
constexpr bool kReportAggregatesOnly = true;
constexpr std::int64_t kBytesProcessed = 1;

static void BM_StaticExecutor(benchmark::State &state)
{
    using namespace static_tasks;

    Data data{};

    using Tasks = std::tuple<A, A, B, A, C>;
    Tasks tasks{
        A{data}, A{data}, B{data}, A{data}, C{data},
    };

    static_executor::Executor<Tasks> executor{tasks};

    for (auto _ : state) {
        executor.Execute();
    }
    state.SetBytesProcessed(kBytesProcessed);

    assert(data.x > 0);
}

BENCHMARK(BM_StaticExecutor)
    ->UseRealTime()
    ->MeasureProcessCPUTime()
    ->Threads(kThreads)
    ->Iterations(kIterations)
    ->Repetitions(kRepetitions)
    ->ReportAggregatesOnly(kReportAggregatesOnly);

static void BM_DynamicExecutor(benchmark::State &state)
{
    using namespace dynamic_tasks;

    Data data{};

    using Tasks = std::array<dynamic_executor::Task *, 5>;
    A a{data};
    B b{data};
    C c{data};
    Tasks tasks{
        &a, &a, &b, &a, &c,
    };

    dynamic_executor::Executor<Tasks> executor{tasks};

    for (auto _ : state) {
        executor.Execute();
    }
    state.SetBytesProcessed(kBytesProcessed);

    assert(data.x > 0);
}

BENCHMARK(BM_DynamicExecutor)
    ->UseRealTime()
    ->MeasureProcessCPUTime()
    ->Threads(kThreads)
    ->Iterations(kIterations)
    ->Repetitions(kRepetitions)
    ->ReportAggregatesOnly(kReportAggregatesOnly);

BENCHMARK_MAIN();
