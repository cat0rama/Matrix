cmake_minimum_required(VERSION 3.22)

project(Matrix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME}
    matrix.hpp
    matrix.cpp
    main.cpp
)
