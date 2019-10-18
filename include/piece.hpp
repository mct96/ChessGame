#pragma once

#include <vector>
#include <memory>
#include <stdexcept>
#include <cmath>
#include <ostream>

#include "position.hpp"
#include "color.hpp"

#include <iostream>
using namespace std;

namespace ch {

class CPiece {

public:
    CPiece(CPosition position, EColor color);
    virtual ~CPiece();

    virtual bool canMoveTo(CPosition position) const = 0;
    bool moveTo(CPosition position);

    void setPosition(CPosition position);
    CPosition getPosition() const;

    void setColor(EColor color);
    EColor getColor() const;

    bool wasMoved() const;

    void print() {

        cout << "[" << _position.i << ", " << _position.j << "]" << endl;
    }

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

private:
    CPosition _position; // Indica a posição atual.
    EColor _color;       // Indica a cor da peça.
    bool _wasMoved;      // Indica se a peça já foi movida. (Peão e Torre)
};

}