#include "../include/solver.h"
#include <array>
#include <iostream>
#include <unordered_map>

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

bool Solver::solveHiddenSingles() {
    bool progress = false;

    auto findHiddenSingleInUnit = [&](std::array<int, 9> unit) {
        std::unordered_map<int, int> candidateCount;
        std::unordered_map<int, int> candidateIndex;

        for (int index : unit) {
            Cell& unitCell = board_.getCell(index);
            if (unitCell.isSolved()) {
                continue;
            }

            for (int digit = 1; digit <= 9; digit++) {
                if (unitCell.hasCandidate(digit)) {
                    candidateCount[digit]++;
                    candidateIndex[digit] = index;
                }
            }
        }

        for (int digit = 1; digit <= 9; digit++) {
            if (candidateCount[digit] == 1) {
                Cell& cell = board_.getCell(candidateIndex[digit]);
                for (int other = 1; other <= 9; other++) {
                    if (other != digit) {
                        if (cell.eliminateCandidate(other)) {
                            progress = true;
                        }
                    }
                }
            }
        }
    };


    for (int i = 0; i < 81; i++) {
        int row = i / 9;
        int col = i % 9;
        int box = (row / 3) * 3 + (col / 3);

        findHiddenSingleInUnit(board_.getRowCells(row));
        findHiddenSingleInUnit(board_.getColCells(col));
        findHiddenSingleInUnit(board_.getBoxCells(box));
    }
    return progress;
}

bool Solver::solve() {
    while (!board_.isSolved()) {
        bool progress = false;
        progress = progress | propagateConstraints();
        progress = progress | solveHiddenSingles();
        if (!progress) {
            return false;
        } 
    }
    return true;
}
