#pragma once

#include <iostream>

#include "path.hpp"
#include "coordinate.hpp"

using namespace std;

namespace ch {

bool inPath(std::vector<CPath> paths, CCoordinate pos);

}