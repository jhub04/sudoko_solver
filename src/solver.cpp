#include "../include/solver.h"
#include <array>

Solver::Solver(Board& board) : board_(board) {};

bool Solver::propagateConstraints() {
    bool progress = false;

    for (int i = 0; i < 81; i++) {
        Cell& cell = board_.getCell(i);
        if (!cell.isSolved()) {
            continue;
        }

        int digit = cell.getValue();

        auto eliminateFromUnit = [&](std::array<int, 9> unit) {
            for (int index : unit) {
                if (index == i) {
                    continue;
                }
                if (board_.getCell(index).eliminateCandidate(digit)) {
                    progress = true;
                }
            }
        };

        int row = i / 9;
        int col = i % 9;
        int box = (row / 3) * 3 + (col / 3);

        eliminateFromUnit(board_.getRowCells(row));
        eliminateFromUnit(board_.getColCells(col));
        eliminateFromUnit(board_.getBoxCells(box));
    }

    return progress;
}
