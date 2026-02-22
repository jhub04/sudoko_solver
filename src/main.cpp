#include <iostream>
#include "../include/board.h"
#include "../include/solver.h"
#include <string>

int main()
{
    std::string grid1 = "003020600900305001001806400008102900700000008006708200002609500800203009005010300";
    std::string grid2 = "200080300060070084030500209000105408000000000402706000301007040720040060004010003";
    Board board(grid2);
    
    board.draw();

    Solver solver(board);

    solver.solve();

    std::cout << "\n--- Solution ---" << '\n';
    
    board.draw();
}


