#include "../include/cell.h"

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


