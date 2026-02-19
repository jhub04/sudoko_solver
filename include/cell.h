#ifndef CELL_H
#define CELL_H

#include <cstdint>

class Cell {
    private:
        uint16_t candidates_;

    public:
        Cell();
        Cell(int value);

        int getValue(); // returns 0 if unsolved
        bool isSolved();
        uint16_t getCandidates();

        bool eliminateCandidate(int digit);
        bool hasCandidate(int digit);
};

#endif
        
