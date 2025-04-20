include(FetchContent)

if (NOT googletest_POPULATED)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.10.0.zip)
    FetchContent_Populate(googletest)
    include_directories(${googletest_SOURCE_DIR}/googletest/include/)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()

if (NOT benchmark_POPULATED)
    FetchContent_Declare(benchmark URL https://github.com/google/benchmark/archive/v1.5.0.zip)
    FetchContent_Populate(benchmark)
    add_subdirectory(${benchmark_SOURCE_DIR} ${benchmark_BINARY_DIR})
endif ()

add_executable(executor_benchmark executor_benchmark.cpp)
target_link_libraries(executor_benchmark benchmark)
