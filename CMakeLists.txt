cmake_minimum_required(VERSION 3.14)
project(ROUTE LANGUAGES CXX)

# Use modern C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

# Add executable
add_executable(ROUTE_linux
    main.cpp
)

# Include directories
target_include_directories(ROUTE_linux PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
)

# Compiler options (scoped to target)
target_compile_options(ROUTE_linux PRIVATE -O2 -Wall -pthread)

# Link libraries
target_link_libraries(ROUTE_linux PRIVATE
    Python3::Python
    Python3::NumPy
    Eigen3::Eigen
    yaml-cpp
    readline
    ncurses
)
