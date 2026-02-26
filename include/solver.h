#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"
#include "cell.h"

class Solver {
private:
  Board &board_;

  bool propagateConstraints();
  bool solveHiddenSingles();
  bool solveNakedPairs();

public:
  Solver(Board &board);
  bool solve();
};

#endif // SOLVER_H
