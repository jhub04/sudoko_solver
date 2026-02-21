cmake_minimum_required(VERSION 3.10.0)

project(sudoku_solver)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(sudoku_solver src/solver.cpp src/board.cpp src/cell.cpp src/main.cpp)

target_include_directories(sudoku_solver PRIVATE ${CMAKE_SOURCE_DIR}/include)
