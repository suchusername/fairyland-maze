cmake_minimum_required(VERSION 3.1)
project(fairyland_maze)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(fairytale)

add_library(fairyland STATIC
    src/combine_maps.cpp
    src/forest_map.cpp
    src/dfs.cpp
)
target_include_directories(fairyland PUBLIC include)
target_link_libraries(fairyland PRIVATE fairytale)

add_executable(fairyland_go src/main.cpp)
target_link_libraries(fairyland_go PRIVATE fairytale fairyland)