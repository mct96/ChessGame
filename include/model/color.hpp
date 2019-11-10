#pragma once

#include <string>
#include <stdexcept>

namespace ch {

enum class EColor {
    BLACK,
    WHITE
};

std::string colorToString(EColor color);

}