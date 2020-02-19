#pragma once

#include <iostream>
#include <memory>

#include "path.hpp"
#include "coordinate.hpp"

using namespace std;

namespace ch {

bool inPath(CMoveTree& paths, CCoordinate pos, unique_ptr<CMove*>& selectedMove);

}