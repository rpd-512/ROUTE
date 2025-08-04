cmake_minimum_required(VERSION 3.14)
project(ROUTE LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -pthread")

# Find Python3
find_package(Python3 COMPONENTS Interpreter Development NumPy REQUIRED)

# Find Eigen
find_package(Eigen3 REQUIRED)

# Source files
set(SOURCES main.cpp)

# Executable
add_executable(ROUTE_linux ${SOURCES})

# Includes
target_include_directories(ROUTE_linux PRIVATE
    ${Python3_INCLUDE_DIRS}
    ${Python3_NumPy_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include  # Add your own headers
)

# Link libraries
target_link_libraries(ROUTE_linux PRIVATE
    ${Python3_LIBRARIES}
)
