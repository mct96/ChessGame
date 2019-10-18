#include "../include/piece.hpp"


namespace ch {

CPiece::CPiece(CPosition pos, EColor col):
    _position{pos},
    _color{col},
    _wasMoved{false}
{

}

CPiece::~CPiece()
{

}


bool CPiece::moveTo(CPosition pos)
{
    if (!canMoveTo(pos))
        throw new std::logic_error{"Invalid move."};

    setPosition(pos);
}

void CPiece::setPosition(CPosition pos)
{
    if (pos.i < 0 || pos.i > 7 || pos.j < 0 || pos.j > 7)
        throw new std::out_of_range{"Invalid position"};

    _position = pos;
}

CPosition CPiece::getPosition() const
{
    return _position;
}

void CPiece::setColor(EColor col)
{
    _color = col;
}

EColor CPiece::getColor() const
{
    return _color;
}

bool CPiece::wasMoved() const
{
    return _wasMoved;
}

bool CPiece::isDiagonalMove(CPosition dest) const
{
    auto di = abs(_position.i - dest.i);
    auto dj = abs(_position.j - dest.j);

    // Movimento diagonais possuem o mesmo deslocamento entre os
    // eixos em termos absolutos.
    return di == dj;
}

bool CPiece::isVerticalMove(CPosition dest) const
{
    auto di = _position.i - dest.i;

    // O movitento vertical não possui deslocamento horizontal.
    return di == 0;
}

bool CPiece::isHorizontalMove(CPosition dest) const
{
    auto dj = _position.j - dest.j;

    // O movimento horizontal não possui deslocamento vertical.
    return dj == 0;
}

bool CPiece::isForwardMove(CPosition destination) const
{
    // As peças brancas estão em i = [1 .. 2] e as peças pretas
    // estão em i = [7 .. 8].
    auto di = _position.i - destination.i;

    return (
        (di > 0 && _color == EColor::WHITE) ||
        (di < 0 && _color == EColor::BLACK)
    );
}

int CPiece::getMoveRange(CPosition dest) const
{
    auto di = abs(_position.i - dest.i);
    auto dj = abs(_position.j - dest.j);

    // Distância de Manhattan.
    return di + dj;
}

}