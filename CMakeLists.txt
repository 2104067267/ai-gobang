
cmake_minimum_required(VERSION 3.26)
project(untitled3)
include_directories("C:/Users/21040/vcpkg1/include")
link_directories("C:/Users/21040/vcpkg1/lib")
set(CMAKE_PREFIX_PATH "C:/Users/21040/vcpkg1/cmake")
find_package(raylib CONFIG REQUIRED)
add_executable(untitled3 main.cpp
        五子棋.h)
target_link_libraries(${PROJECT_NAME} raylib)
