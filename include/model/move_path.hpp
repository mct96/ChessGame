#pragma once

#include <iostream>

#include "path.hpp"
#include "coordinate.hpp"

using namespace std;

namespace ch {

bool inPath(CMoveTree paths, CCoordinate pos, CMove * selectedMove = nullptr);

}