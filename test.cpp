
#include "include/puzzle.hpp"

#include <iostream>
#include <iomanip>
#include <string>

using namespace std;



void showPath(std::vector<ch::CPiece::CPath> paths)
{
    for (auto path: paths) {
        for (auto pos: path) {
            cout << static_cast<std::string>(pos) << "->";
        }
        cout << endl;
    }
}

int main()
{
    using namespace ch;
    ch::CPuzzle puzzle{};
    puzzle.print();
    auto res = puzzle.getPieceAt(CPosition{1, 2});
    cout << boolalpha << puzzle.isFreePosition({5, 5}) << endl;
    showPath(puzzle.movesPruning(res->getAllMoves()));

    return 0;
}