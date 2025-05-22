cmake_minimum_required(VERSION 3.10)

project(Puzzle15)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(Puzzle15 main.cpp puzzle.cpp)

target_link_libraries(Puzzle15 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})