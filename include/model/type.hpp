#pragma once

#include <string>
#include <stdexcept>

namespace ch {

enum class EType {
    PAWN,
    ROOK,
    KNIGHT,
    BISHOP,
    KING,
    QUEEN,
    NULL_TYPE
};

std::string typeToString(EType type);

}