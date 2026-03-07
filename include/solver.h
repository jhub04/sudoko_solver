#ifndef SOLVER_H
#define SOLVER_H

#include "board.h"
#include "cell.h"
#include <functional>

class Solver {
private:
  Board &board_;

  bool applyToAllUnits(const std::function<bool(const Unit &)> &strategy);

  bool propagateConstraints();

  bool findHiddenSingleInUnit(const Unit &unit);
  bool findHiddenPairInUnit(const Unit &unit);

  bool findNakedPairInUnit(const Unit &unit);
  bool findNakedTripleInUnit(const Unit &unit);



public:
  explicit Solver(Board &board);
  bool solve();
};

#endif // SOLVER_H
