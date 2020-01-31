#pragma once

#include <string>

#include "color.hpp"

namespace ch {

class CPlayer {
public:
    CPlayer();
    ~CPlayer();

protected:

private:
    std::string _name;

    ch::EColor _color;

    double _rate;
};
}