#include <iostream>
#include "../include/board.h"
#include "../include/solver.h"
#include <string>

int main()
{
    std::string initial_state = "003020600900305001001806400008102900700000008006708200002609500800203009005010300";
    Board board(initial_state);
    
    board.draw();

    Solver solver(board);

    solver.solve();

    std::cout << "\n--- Solution ---" << '\n';
    
    board.draw();
}


