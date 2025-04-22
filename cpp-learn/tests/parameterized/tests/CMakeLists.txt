# Include testing framework
include(FetchContent)
if (NOT googletest_POPULATED)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.11.0.zip)
    FetchContent_Populate(googletest)
    include_directories(${googletest_SOURCE_DIR}/googletest/include/)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif ()

# Add tests
function(set_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} gtest gtest_main)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME} WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

set_test(calculator_simple_test calculator_simple_test.cpp)
set_test(calculator_table_driven_test calculator_table_driven_test.cpp)
set_test(calculator_parameterized_test calculator_parameterized_test.cpp)
