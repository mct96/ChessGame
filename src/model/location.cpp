#include "../../include/model/location.hpp"

namespace ch {

CLocation::CLocation(EType type, EColor color)
    :
    _isEmpty{false},
    _type{type},
    _color{color}
{

}

CLocation::CLocation()
    :
    _isEmpty{true},
    _type{EType::NULL_TYPE},
    _color{EColor::NULL_COLOR}
{

}

CLocation::~CLocation()
{

}

void CLocation::setPiece(EType type, EColor color) {
    _type = type;
    _color = color;

    _isEmpty = false;
}

EType CLocation::getType() const {
    return _type;
}

EColor CLocation::getColor() const {
    return _color;
}

void CLocation::removePiece() {
    _isEmpty = true;

    _type = EType::NULL_TYPE;
    _color = EColor::NULL_COLOR;
}

bool CLocation::isEmpty() const {
    return _isEmpty;
}

}