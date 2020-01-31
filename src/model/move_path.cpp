#include "../../include/model/move_path.hpp"

namespace ch {

bool inPath(std::vector<CPath> paths, CCoordinate pos)
{
    cout << "::inPath" << endl;

    for (auto path: paths) {
        std::cout << path.size() << std::endl;
        for (auto position: path) {
            if (pos == position)
                return true;
        }
    }

    return false;
}

}