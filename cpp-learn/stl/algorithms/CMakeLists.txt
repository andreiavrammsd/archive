add_executable(stl.algorithms.nonmodifying_sequence nonmodifying_sequence.cpp)
add_executable(stl.algorithms.modifying_sequence modifying_sequence.cpp)
add_executable(stl.algorithms.partitioning partitioning.cpp)
add_executable(stl.algorithms.sorting sorting.cpp)
add_executable(stl.algorithms.binary_search binary_search.cpp)
add_executable(stl.algorithms.other_operations_on_sorted_ranges other_operations_on_sorted_ranges.cpp)
add_executable(stl.algorithms.set set.cpp)
add_executable(stl.algorithms.heap heap.cpp)
add_executable(stl.algorithms.min_max min_max.cpp)
add_executable(stl.algorithms.comparison comparison.cpp)
add_executable(stl.algorithms.permutation permutation.cpp)
add_executable(stl.algorithms.numeric numeric.cpp)
add_executable(stl.algorithms.uninitialized_memory uninitialized_memory.cpp)
add_executable(stl.algorithms.c_library c_library.cpp)

add_executable(stl.algorithms.execution_policies execution_policies.cpp)
target_link_libraries(stl.algorithms.execution_policies tbb)
