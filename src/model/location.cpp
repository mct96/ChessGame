#include "../../include/model/location.hpp"

namespace ch {

CLocation::CLocation()
    :
    _piece{nullptr}
{

}

CLocation::~CLocation()
{

}

void CLocation::setPiece(std::shared_ptr<CPiece> piece) {
    this->_piece = piece;
}

std::shared_ptr<CPiece> CLocation::getPiece() const {
    return this->_piece;
}

void CLocation::removePiece() {
    this->_piece = nullptr;
}

bool CLocation::isEmpty() const {
    return this->_piece == nullptr;
}

}