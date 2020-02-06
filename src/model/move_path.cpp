#include "../../include/model/move_path.hpp"

namespace ch {

bool inPath(CMoveTree tree, CCoordinate pos, CMove *selectedMove)
{
    cout << "::inPath" << endl;

    for (auto& branch: tree) {
        for (auto& move: branch) {
            auto position = move.getMove().getTo();
            if (pos == position) {
                if (selectedMove) *selectedMove = move;
                return true;
            }
        }
    }

    return false;
}

}