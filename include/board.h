#ifndef BOARD_H
#define BOARD_H

#include <array>
#include <string>
#include "cell.h"

class Board {
    private:
        std::array<Cell, 81> board_;

    public:
        Board(const std::string& initial_state);

        Cell& getCell(int index);
        Cell& getCell(int row, int col);
        std::array<int, 9> getBoxCells(int box) const;
        std::array<int, 9> getRowCells(int row) const;
        std::array<int, 9> getColCells(int col) const;

        void draw() const;
        bool isSolved() const;
};

#endif // BOARD_H
