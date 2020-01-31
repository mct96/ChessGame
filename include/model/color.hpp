#pragma once

#include <string>
#include <stdexcept>

namespace ch {

enum class EColor {
    BLACK,
    WHITE,
    NULL_COLOR
};

std::string colorToString(EColor color);

}