#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>

#include "position.hpp"
#include "color.hpp"

using namespace std;

namespace ch {

class CPiece {

public:
    using CPath = std::vector<CPosition>;

    CPiece(CPosition position, EColor color);
    virtual ~CPiece();

    virtual bool canMoveTo(CPosition position) const = 0;
    virtual std::vector<CPath> getAllMoves() const = 0;
    bool moveTo(CPosition position);

    void setPosition(CPosition position);
    CPosition getPosition() const;

    void setColor(EColor color);
    EColor getColor() const;

    bool wasMoved() const;


protected:
    // Indica se o movimento realizado pela peça até o destino
    // é diagonal.
    bool isDiagonalMove(CPosition destination) const;

    // Indica se o movimento é vertical.
    bool isVerticalMove(CPosition destination) const;

    // Indica se o movimento é horizontal.
    bool isHorizontalMove(CPosition destination) const;

    // Indica se o movimento é para "frente". "Frente" varia
    // de acordo com a cor da peça.
    bool isForwardMove(CPosition destination) const;

    // Retorna a distância entre a posição atual e o destino.
    int getMoveRange(CPosition dest) const;

    std::vector<CPath> getVerticalMoves() const;

    std::vector<CPath> getHorizontalMoves() const;

    std::vector<CPath> getDiagonalMoves() const;

    std::vector<CPath> getRangeBasedMoves(int range) const;

private:
    CPosition _position; // Indica a posição atual.
    EColor _color;       // Indica a cor da peça.
    bool _wasMoved;      // Indica se a peça já foi movida. (Peão e Torre)
};

}