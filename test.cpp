
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

    try {
        puzzle.move({2, 6}, {3, 6});
        puzzle.move({7, 5}, {6, 5});

        puzzle.move({3, 6}, {4, 6});
        puzzle.move({6, 5}, {5, 5});

        puzzle.move({4, 6}, {5, 5});
        puzzle.print();
        // puzzle.move({4, 6}, {5, 5});

        std::vector<CPiece::CPath> path{};
        puzzle.exceptionalMoves(puzzle.getPieceAt({4, 6}), path);
        showPath(path);
    } catch (std::exception * ptr) {
        cout << ptr->what() << endl;
    }


    return 0;
}