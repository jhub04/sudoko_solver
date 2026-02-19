#include "../include/cell.h"
#include <cstdint>

Cell::Cell() : candidates_(0b1111111110) {}

Cell::Cell(int value) : candidates_(1 << value) {}

int Cell::getValue() {
    if (!isSolved()) {
       return 0; 
    }

    return __builtin_ctz(candidates_);
}

bool Cell::isSolved() {
    return candidates_ != 0 && (candidates_ & (candidates_ - 1)) == 0;
};

uint16_t Cell::getCandidates() {
    return candidates_;
}

bool Cell::eliminateCandidate(int digit) {
    // If digit not in candidates, return false
    if (!hasCandidate(digit)) {
        return false;
    } 

    // Remove digit from candidates
    candidates_ = candidates_ & ~(1 << digit);
    return true;
}

bool Cell::hasCandidate(int digit) {
    return candidates_ & (1 << digit);
} 



