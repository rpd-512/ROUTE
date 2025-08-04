cmake_minimum_required(VERSION 3.14)
project(ROUTE LANGUAGES CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -pthread")

# Find required packages
find_package(Python3 REQUIRED COMPONENTS Interpreter Development NumPy)
find_package(Eigen3 REQUIRED)
find_package(yaml-cpp REQUIRED)

# Source files
set(SOURCES main.cpp)

# Add executable
add_executable(ROUTE_linux ${SOURCES})

# Include directories
target_include_directories(ROUTE_linux PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include  # Optional: for your own headers
)

# Link libraries
target_link_libraries(ROUTE_linux PRIVATE
    ${Python3_LIBRARIES}
    yaml-cpp
)
