#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
#include <array>
#include <string>
#include <vector>

using Unit = std::array<int, 9>;

class Board {
private:
  std::array<Cell, 81> board_;

public:
  explicit Board(const std::string &initial_state);

  Cell &getCell(int index);
  Cell &getCell(int row, int col);

  Unit getBoxCells(int box) const;
  Unit getRowCells(int row) const;
  Unit getColCells(int col) const;
  std::vector<Unit> getAllUnits() const;

  void draw() const;
  bool isSolved() const;
};

#endif // BOARD_H
