#include "../include/cell.h"
#include <bitset>
#include <cstdint>
#include <iostream>

Cell::Cell() : candidates_(0b1111111110) {}

Cell::Cell(int value) : candidates_(1 << value) {}

int Cell::getValue() const {
  if (!isSolved()) {
    return 0;
  }

  return __builtin_ctz(candidates_);
}

bool Cell::isSolved() const { return candidates_ != 0 && (candidates_ & (candidates_ - 1)) == 0; };

uint16_t Cell::getCandidates() const { return candidates_; }

int Cell::getCandidateAmount() const {
  return __builtin_popcount(candidates_);
}

bool Cell::eliminateCandidate(int digit) {
  // If digit not in candidates, return false
  if (!hasCandidate(digit)) {
    return false;
  }

  if (isSolved()) {
    return false;
  }

  // Remove digit from candidates
  candidates_ &= ~(1 << digit);

  return true;
}

bool Cell::hasCandidate(int digit) const { return candidates_ & (1 << digit); }
