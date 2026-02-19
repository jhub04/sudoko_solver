#ifndef CELL_H
#define CELL_H

#include <cstdint>

class Cell {
    private:
        uint16_t candidates_;

    public:
        Cell();
        Cell(int value);

        int getValue() const; // returns 0 if unsolved
        bool isSolved() const;
        uint16_t getCandidates() const;

        bool eliminateCandidate(int digit);
        bool hasCandidate(int digit) const;
};

#endif
        
