#include "../include/piece_behavior.hpp"

namespace ch {

bool CPawn::canMoveTo(CPosition pos) const
{
    if (isVerticalMove(pos) && isForwardMove(pos))
        return (
            (getMoveRange(pos) == 1) ||
            (getMoveRange(pos) == 2 && !wasMoved())
        );

    return false;
}

bool CRook::canMoveTo(CPosition pos) const
{
    return isHorizontalMove(pos) || isVerticalMove(pos);
}

bool CKnight::canMoveTo(CPosition pos) const
{
    return (
        !isHorizontalMove(pos) &&
        !isVerticalMove(pos) &&
        getMoveRange(pos) == 2
    );
}

bool CBishop::canMoveTo(CPosition pos) const
{
    return isVerticalMove(pos);
}

bool CQueen::canMoveTo(CPosition pos) const
{
    return (
        isVerticalMove(pos) ||
        isHorizontalMove(pos));
}

bool CKing::canMoveTo(CPosition pos) const
{
    // Dá para simplificar a expressão, porém é preferível manter a
    // legibilidade do código.

    // O Rei se move em todas as direções. As distância diagonal é igual
    // a 2 por causa da distância de Manhattan.
    return (
        (isVerticalMove(pos)   && getMoveRange(pos) == 1) ||
        (isHorizontalMove(pos) && getMoveRange(pos) == 1) ||
        (isDiagonalMove(pos)   && getMoveRange(pos) == 2)
    );
}

}