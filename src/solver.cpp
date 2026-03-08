#include "../include/solver.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
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

bool Solver::findHiddenPairInUnit(const Unit &unit) {
  bool progress = false;
  // For each digit 1-9, collect which cells in that unit contain it
  std::unordered_map<int, std::vector<int>> digitToCells;

  for (int index : unit) {
    const Cell &cell = board_.getCell(index);
    if (cell.isSolved()) {
      continue;
    }

    for (int digit = 1; digit <= 9; digit++) {
      if (cell.hasCandidate(digit)) {
        digitToCells[digit].push_back(index);
      }
    }
  }

  // Find two digits that appear in exactly the same two cells
  std::unordered_map<int, std::vector<int>> cellsToDigits;
  for (const auto &[digit, cells] : digitToCells) {
    if (cells.size() != 2) {
      continue;
    }

    int max = (cells[0] > cells[1]) ? cells[0] : cells[1];
    int min = (cells[0] > cells[1]) ? cells[1] : cells[0];

    int key = min * 81 + max;

    cellsToDigits[key].push_back(digit);
  }

  // Hidden pair: cellsToDigits has exactly two digits
  for (const auto &[encodedCells, digits] : cellsToDigits) {
    if (digits.size() != 2) {
      continue;
    }

    // Hidden pair detected
    int max = encodedCells % 81;
    int min = (encodedCells - max) / 81;

    Cell &cellOne = board_.getCell(max);
    Cell &cellTwo = board_.getCell(min);

    // Eliminate all other candidates from the pair cells
    for (int digit = 1; digit <= 9; digit++) {
      if (std::find(digits.begin(), digits.end(), digit) == digits.end()) {
        if (cellOne.eliminateCandidate(digit)) {
          progress = true;
        }
        if (cellTwo.eliminateCandidate(digit)) {
          progress = true;
        }
      }
    }
  }


  return progress;
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

bool Solver::findNakedTripleInUnit(const Unit &unit) {
  bool progress = false;

  // Collect unsolved cells with 2 or 3 candidates
  std::vector<int> candidateCells;
  for (int index : unit) {
    Cell &cell = board_.getCell(index);
    if (!cell.isSolved() &&
        (cell.getCandidateAmount() == 2 || cell.getCandidateAmount() == 3)) {
      candidateCells.push_back(index);
    }
  }

  if (candidateCells.size() < 3) return false;

  // Try all C(n,3) combinations using three nested loops (i < j < k)
  int n = candidateCells.size();
  for (int i = 0; i < n - 2; i++) {
    for (int j = i + 1; j < n - 1; j++) {
      for (int k = j + 1; k < n; k++) {
        uint16_t combined = board_.getCell(candidateCells[i]).getCandidates() |
                            board_.getCell(candidateCells[j]).getCandidates() |
                            board_.getCell(candidateCells[k]).getCandidates();

        if (__builtin_popcount(combined) != 3) {
          continue;
        }

        // Eliminate those digits from all other cells in the unit
        for (int otherIndex : unit) {
          if (otherIndex == candidateCells[i] ||
              otherIndex == candidateCells[j] ||
              otherIndex == candidateCells[k]) {
            continue;
          }

          for (int digit = 1; digit <= 9; digit++) {
            if ((combined >> digit) & 1) {
              if (board_.getCell(otherIndex).eliminateCandidate(digit)) {
                progress = true;
              }
            }
          }
        }
      }
    }
  }

  return progress;
}

bool Solver::solve() {
  while (!board_.isSolved()) {
    if (propagateConstraints()) {
      continue;
    }

    if (applyToAllUnits(
            [this](const Unit &unit) { return findHiddenSingleInUnit(unit); })) {
      continue; // re-propagate before trying next strategy
    }

    if (applyToAllUnits(
            [this](const Unit &unit) { return findNakedPairInUnit(unit); })) {
      continue;
    }

    if (applyToAllUnits(
            [this](const Unit &unit) { return findNakedTripleInUnit(unit); })) {
      continue;
    }

    if (applyToAllUnits([this](const Unit &unit) { return findHiddenPairInUnit(unit); })) {
      continue;
    }

    // No strategy made progress — puzzle cannot be solved with current techniques
    return false;
  }
  return true;
}
