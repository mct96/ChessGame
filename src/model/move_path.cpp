#include "../../include/model/move_path.hpp"

namespace ch {

bool inPath(CMoveTree tree, CCoordinate pos, CMove *selectedMove)
{
    cout << "::inPath" << endl;

    for (auto branch: tree.getBranchs()) {
        for (unsigned int i = 0; i < branch.numberOfMoves(); ++i) {
            auto position = branch[i].getMove().getTo();
            if (pos == position) {
                if (selectedMove) *selectedMove = branch[i];
                return true;
            }
        }
    }

    return false;
}

}