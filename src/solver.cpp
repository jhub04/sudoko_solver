#include "../include/solver.h"
#include <array>
#include <iostream>
#include <unordered_map>

Solver::Solver(Board &board) : board_(board) {};

bool Solver::propagateConstraints() {
  bool progress = false;

  for (int i = 0; i < 81; i++) {
    Cell &cell = board_.getCell(i);
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

  auto findHiddenSingleInUnit = [&](std::array<int, 9> unit, std::string unitType) -> bool {
    std::unordered_map<int, int> candidateCount;
    std::unordered_map<int, int> candidateIndex;

    for (int index : unit) {
      Cell &unitCell = board_.getCell(index);
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
        Cell &cell = board_.getCell(candidateIndex[digit]);

        for (int other = 1; other <= 9; other++) {
          if (other != digit) {
            cell.eliminateCandidate(other);
          }
        }

        return true;
      }
    }
    return false;
  };

  for (int row = 0; row < 9; row++) {
    if (findHiddenSingleInUnit(board_.getRowCells(row), "row " + std::to_string(row))) {
      return true;
    }
  }

  for (int col = 0; col < 9; col++) {
    if (findHiddenSingleInUnit(board_.getColCells(col), "col " + std::to_string(col))) {
      return true;
    }
  }

  for (int box = 0; box < 9; box++) {
    if (findHiddenSingleInUnit(board_.getBoxCells(box), "box " + std::to_string(box))) {
      return true;
    }
  }

  return progress;
}

bool Solver::solve() {
  while (!board_.isSolved()) {
    bool progress = false;

    while (propagateConstraints()) {
      progress = true;
    }

    if (solveHiddenSingles()) {
      progress = true;
    }

    if (!progress) {
      return false;
    }
  }
  return true;
}
