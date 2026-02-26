#include "../include/solver.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

Solver::Solver(Board &board) : board_(board) {}

// Applies a per-unit solving strategy to all 27 units (rows, cols, boxes).
// propagation can run before continuing. This eliminates the repeated
// for-row / for-col / for-box boilerplate that every technique would need.
bool Solver::applyToAllUnits(
    const std::function<bool(const Unit &)> &strategy) {
  for (const Unit &unit : board_.getAllUnits()) {
    if (strategy(unit)) {
      return true;
    }
  }
  return false;
}

// Eliminates candidates from peers of solved cells.
// This is the most basic deduction: if a cell is solved, no other cell in
// the same row/col/box can have that digit.
bool Solver::propagateConstraints() {
  bool progress = false;

  for (int i = 0; i < 81; i++) {
    Cell &cell = board_.getCell(i);
    if (!cell.isSolved()) {
      continue;
    }

    int digit = cell.getValue();
    int row = i / 9;
    int col = i % 9;
    int box = (row / 3) * 3 + (col / 3);

    auto eliminateFromUnit = [&](const Unit &unit) {
      for (int index : unit) {
        if (index != i && board_.getCell(index).eliminateCandidate(digit)) {
          progress = true;
        }
      }
    };

    eliminateFromUnit(board_.getRowCells(row));
    eliminateFromUnit(board_.getColCells(col));
    eliminateFromUnit(board_.getBoxCells(box));
  }

  return progress;
}

bool Solver::findHiddenSingleInUnit(const Unit &unit) {
  std::unordered_map<int, int> candidateCount;
  std::unordered_map<int, int> candidateIndex;

  for (int index : unit) {
    const Cell &cell = board_.getCell(index);
    if (cell.isSolved()) {
      continue;
    }

    for (int digit = 1; digit <= 9; digit++) {
      if (cell.hasCandidate(digit)) {
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
}

bool Solver::findNakedPairInUnit(const Unit &unit) {
  bool progress = false;

  std::unordered_map<uint16_t, std::vector<int>> pairCellMap;
  for (int index : unit) {
    const Cell &cell = board_.getCell(index);
    if (!cell.isSolved() && cell.getCandidateAmount() == 2) {
      pairCellMap[cell.getCandidates()].push_back(index);
    }
  }

  for (const auto &[candidateMask, pairCells] : pairCellMap) {
    if (pairCells.size() != 2) {
      continue;
    }

    // Eliminate the pair's digits from all other cells in the unit
    for (int otherIndex : unit) {
      if (std::find(pairCells.begin(), pairCells.end(), otherIndex) !=
          pairCells.end()) {
        continue;
      }

      for (int digit = 1; digit <= 9; digit++) {
        if (board_.getCell(pairCells[0]).hasCandidate(digit) &&
            board_.getCell(otherIndex).eliminateCandidate(digit)) {
          progress = true;
        }
      }
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

    if (applyToAllUnits(
            [this](const Unit &unit) { return findHiddenSingleInUnit(unit); })) {
      progress = true;
    }

    if (applyToAllUnits(
            [this](const Unit &unit) { return findNakedPairInUnit(unit); })) {
      progress = true;
    }

    if (!progress) {
      return false;
    }
  }
  return true;
}
