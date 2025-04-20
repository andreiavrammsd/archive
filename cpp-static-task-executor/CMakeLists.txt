cmake_minimum_required(VERSION 3.12)
project(static_executor)
set(PROJECT_VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CXX_EXTENSIONS NO)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra -Wpedantic -Werror -fsanitize=undefined")

include_directories(include)
add_subdirectory(tests)
