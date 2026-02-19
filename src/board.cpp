#include "../include/board.h"
#include <stdexcept>

Board::Board(const std::string& initial_state) {
    if (initial_state.empty()) {
        throw std::invalid_argument("Initial_state can't be empty");
    } 

    for (int i = 0; i < 81; i++) {
        if (initial_state[i] == '0') {
            board_[i] = Cell();
        } else {
            board_[i] = Cell(initial_state[i] - '0');
        }
    }
    
}

Cell& Board::getCell(int index) {
    return board_[index];
}

Cell& Board::getCell(int row, int col) {
    int index = row * 9 + col;
    return board_[index];
}

std::array<int, 9> Board::getBoxCells(int box) const {
   // 0 1 2 (box 0)
   // 9 10 11
   // 18 19 20
   
   std::array<int, 9> result;
   int start_row = (box / 3) * 3;
   int start_col = (box % 3) * 3;

   int index = 0;
   for (int row = 0; row < 3; row++) {
       for (int col = 0; col < 3; col++) {
           result[index++] = (start_row + row) * 9 + (start_col + col); 
       }
   }

   return result;
}
