set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-gnu-zero-variadic-macro-arguments")

# Include framework
include(FetchContent)
if (NOT googletest_POPULATED)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.16.0.zip)
    FetchContent_Populate(googletest)
    include_directories(${googletest_SOURCE_DIR}/googletest/include/)
    include_directories(${googletest_SOURCE_DIR}/googlemock/include/)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# Add test macro
macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} gtest gmock gtest_main)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    list(APPEND SOURCES ${ARGN})
endmacro()

# Test MyQueue
set(TEST_FILES MyQueueTest.cpp)
set(SOURCE_FILES MyQueue.cpp)
package_add_test(tests.my_queue ${TEST_FILES} ${SOURCE_FILES})

# Test MyService
set(TEST_FILES MyServiceTest.cpp)
set(SOURCE_FILES MyService.cpp)
package_add_test(tests.my_service ${TEST_FILES} ${SOURCE_FILES})

# Test all
add_executable(tests.all_tests ${SOURCES})
target_link_libraries(tests.all_tests gtest gmock gtest_main)
