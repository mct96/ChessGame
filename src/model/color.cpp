#include "../../include/model/color.hpp"

namespace ch {

std::string colorToString(EColor color) {
    switch (color) {
    case EColor::BLACK:
        return "Black";

    case EColor::WHITE:
        return "White";

    default:
        throw std::invalid_argument{
            "Unknow color."
        };
    }
}
}