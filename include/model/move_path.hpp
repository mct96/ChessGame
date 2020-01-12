#pragma once

#include "coordinate.hpp"
#include "piece.hpp"

namespace ch {

bool inPath(std::vector<CPiece::CPath> paths, ch::CCoordinate pos);

}