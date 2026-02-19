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
