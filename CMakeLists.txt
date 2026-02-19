cmake_minimum_required(VERSION 3.10.0)

project(sudoku_solver)

add_executable(sudoku_solver src/cell.cpp src/main.cpp)

target_include_directories(sudoku_solver PRIVATE ${CMAKE_SOURCE_DIR}/include)
