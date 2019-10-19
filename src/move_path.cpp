#include "../include/move_path.hpp"

namespace ch {

bool inPath(std::vector<CPiece::CPath> paths, CPosition pos)
{
    for (auto path: paths) {
        for (auto position: path)
            if (pos == position)
                return true;
    }

    return false;
}

}