set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -Wno-gnu-zero-variadic-macro-arguments")

# Add a test
macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} curl gtest gmock gtest_main)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

# Test files
set(TEST_FILES
        apixu_test.cpp
        apixu_conditions_test.cpp
        apixu_current_test.cpp
        apixu_forecast_test.cpp
        apixu_history_test.cpp
        apixu_search_test.cpp)

# Sources
set(SOURCE_FILES
        ../include/apixu/http/exception.h
        ../src/apixu.cpp
        )

package_add_test(test ${TEST_FILES} ${SOURCE_FILES})

package_add_test(test_apixu
        apixu_test.cpp
        ../src/apixu.cpp
        )

package_add_test(test_conditions
        apixu_conditions_test.cpp
        ../src/apixu.cpp
        )

package_add_test(test_current
        apixu_current_test.cpp
        ../src/apixu.cpp
        )

package_add_test(test_search
        apixu_search_test.cpp
        ../src/apixu.cpp
        )

package_add_test(test_history
        apixu_history_test.cpp
        ../src/apixu.cpp
        )

package_add_test(test_forecast
        apixu_forecast_test.cpp
        ../src/apixu.cpp
        )

# Dependencies
include(FetchContent)

# Testing framework
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/release-1.10.0.zip)
FetchContent_Populate(googletest)
include_directories(${googletest_SOURCE_DIR}/googletest/include/)
include_directories(${googletest_SOURCE_DIR}/googlemock/include/)
add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
