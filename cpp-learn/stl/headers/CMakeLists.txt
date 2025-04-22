add_executable(stl.headers.containers containers.cpp)
add_executable(stl.headers.general_utilities general_utilities.cpp)

add_executable(stl.headers.algorithms algorithms.cpp)
target_link_libraries(stl.headers.algorithms tbb)

add_executable(stl.headers.diagnostics diagnostics.cpp)
add_executable(stl.headers.strings_and_characters strings_and_characters.cpp)
add_executable(stl.headers.input_output input_output.cpp)
add_executable(stl.headers.localization localization.cpp)
add_executable(stl.headers.language_support language_support.cpp)
add_executable(stl.headers.numerics numerics.cpp)

add_executable(stl.headers.concurrency concurrency.cpp)
target_link_libraries(stl.headers.concurrency pthread)

add_executable(stl.headers.c_compatibility c_compatibility.cpp)

add_executable(stl.headers.memory_resource memory_resource.cpp)
add_executable(stl.headers.filesystem filesystem.cpp)
