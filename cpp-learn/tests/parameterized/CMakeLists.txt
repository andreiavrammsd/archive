cmake_minimum_required(VERSION 3.15)
project(calculator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wmissing-field-initializers -Wextra -pedantic -Werror -fsanitize=undefined")

include_directories(include)

enable_testing()
add_subdirectory(tests)
