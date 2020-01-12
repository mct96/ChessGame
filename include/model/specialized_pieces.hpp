#pragma once

#include "piece.hpp"

namespace ch {

class CPawn: public CPiece {
public:
    using CPiece::CPiece;
    bool canMoveTo(CCoordinate position) const override;
    std::vector<CPiece::CPath> getAllMoves() const override;
    EType getType() const override;

};

class CRook: public CPiece {
public:
    using CPiece::CPiece;
    bool canMoveTo(CCoordinate position) const override;
    std::vector<CPiece::CPath> getAllMoves() const override;
    EType getType() const override;

};

class CKnight: public CPiece {
public:
    using CPiece::CPiece;
    bool canMoveTo(CCoordinate position) const override;
    std::vector<CPiece::CPath> getAllMoves() const override;
    EType getType() const override;
};

class CBishop: public CPiece {
public:
    using CPiece::CPiece;
    bool canMoveTo(CCoordinate position) const override;
    std::vector<CPiece::CPath> getAllMoves() const override;
    EType getType() const override;
};

class CQueen: public CPiece {
public:
    using CPiece::CPiece;
    bool canMoveTo(CCoordinate position) const override;
    std::vector<CPiece::CPath> getAllMoves() const override;
    EType getType() const override;
};

class CKing: public CPiece {
public:
    using CPiece::CPiece;
    bool canMoveTo(CCoordinate position) const override;
    std::vector<CPiece::CPath> getAllMoves() const override;
    EType getType() const override;
};
}