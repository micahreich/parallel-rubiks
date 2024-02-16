# Specify the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(MyProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add compiler options
add_compile_options(-Wall)

# Add include directories
include_directories(include)

# Add the executable target for the main program
add_executable(main src/main.cpp src/rubiks_cube.cpp)

# Add the executable target for the tests
add_executable(test_rubiks_cube tests/test_rubiks_cube.cpp src/rubiks_cube.cpp)

# Add pthread to the linker for the test executable
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(test_rubiks_cube Threads::Threads)