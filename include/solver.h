#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"
#include "cell.h"

class Solver {
    private:
        Board& board_;

        bool solveOpenSingles();

    public:
        Solver(Board& board_);
        bool solve();
};

#endif // SOLVER_H
