add_executable(example_conditions
        conditions.cpp
        ../src/apixu.cpp
        )
target_link_libraries(example_conditions curl)

add_executable(example_current
        current.cpp
        ../src/apixu.cpp
        )
target_link_libraries(example_current curl)

add_executable(example_search
        search.cpp
        ../src/apixu.cpp
        )
target_link_libraries(example_search curl)

add_executable(example_forecast
        forecast.cpp
        ../src/apixu.cpp
        )
target_link_libraries(example_forecast curl)

add_executable(example_history
        history.cpp
        ../src/apixu.cpp
        )
target_link_libraries(example_history curl)
