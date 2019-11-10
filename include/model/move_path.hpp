#pragma once

#include "position.hpp"
#include "piece.hpp"

namespace ch {

bool inPath(std::vector<CPiece::CPath> paths, ch::CPosition pos);

}