#include <iostream>
#include "../include/cell.h"

int main()
{
    Cell cell(4);

    std::cout << cell.getValue() << '\n';

    Cell another_cell;

    std::cout << another_cell.getCandidates() << '\n'; // should be 1022

    another_cell.eliminateCandidate(9);

    std::cout << another_cell.getCandidates() << '\n'; // should now be 510
    another_cell.eliminateCandidate(8);
    another_cell.eliminateCandidate(7);
    another_cell.eliminateCandidate(6);
    another_cell.eliminateCandidate(5);
    another_cell.eliminateCandidate(4);
    another_cell.eliminateCandidate(3);
    another_cell.eliminateCandidate(2);

    std::cout << "Solved = " << another_cell.isSolved() << '\n';
}


