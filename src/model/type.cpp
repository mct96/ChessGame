#include "../../include/model/type.hpp"

namespace ch {

std::string typeToString(EType type) {
    switch (type) {
    case EType::PAWN:
        return "Pawn";

    case EType::ROOK :
        return "Rook";

    case EType::KNIGHT:
        return "Knight";

    case EType::BISHOP:
        return "Bishop";

    case EType::QUEEN:
        return "Queen";

    case EType::KING :
        return "King";

    default:
        throw std::invalid_argument{
            "Unknow type."
        };
    }
}
}