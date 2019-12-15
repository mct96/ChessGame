#include "../../include/model/specialized_pieces.hpp"


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


std::vector<CPiece::CPath> CPawn::getAllMoves() const
{
    auto verticalMove = getVerticalMoves();
    auto forwardMove = std::vector<CPath>{};

    // Seleciona o caminho vertical que avança.
    for (auto move: verticalMove)
        if (isForwardMove(move[0]))
            forwardMove.push_back(move);

    // forwardMove sempre tem no máximo um CPath.

    // Move uma ou duas posições dependendo se já moveu ou não.
    while (forwardMove[0].size() > 2)
        forwardMove[0].pop_back();

    if (wasMoved())
        forwardMove[0].pop_back();

    return forwardMove;
}

EType CPawn::getType() const {
    return EType::PAWN;
}

/* -------------------------------------------------------------------------- */

bool CRook::canMoveTo(CPosition pos) const
{
    return isHorizontalMove(pos) || isVerticalMove(pos);
}

std::vector<CPiece::CPath> CRook::getAllMoves() const
{
    auto verticalMove = getVerticalMoves();
    auto horizontalMove = getHorizontalMoves();

    // Extende os resultados dos movimentos verticais e horizontais.
    auto& result = verticalMove;
    for (auto element: horizontalMove)
        result.push_back(element);

    return result;

}

EType CRook::getType() const {
    return EType::ROOK;
}

/* -------------------------------------------------------------------------- */

bool CKnight::canMoveTo(CPosition pos) const
{
    return (
        !isHorizontalMove(pos) &&
        !isVerticalMove(pos) &&
        getMoveRange(pos) == 2
    );
}

std::vector<CPiece::CPath> CKnight::getAllMoves() const
{
    auto rangeMoves = getRangeBasedMoves(3);
    auto result = decltype(rangeMoves){};

    // Adiciona os movimentos com distância igual a 3 e que não são
    // nem verticais e nem horizontais.
    for (auto& move: rangeMoves) {
        if (!isVerticalMove(move[0]) && !isHorizontalMove(move[0]))
            result.push_back(move);
    }

    return result;
}

EType CKnight::getType() const {
    return EType::KNIGHT;
}

/* -------------------------------------------------------------------------- */

bool CBishop::canMoveTo(CPosition pos) const
{
    return isVerticalMove(pos);
}

std::vector<CPiece::CPath> CBishop::getAllMoves() const
{
    return getDiagonalMoves();
}

EType CBishop::getType() const {
    return EType::BISHOP;
}

/* -------------------------------------------------------------------------- */

bool CQueen::canMoveTo(CPosition pos) const
{
    return (
        isVerticalMove(pos) ||
        isHorizontalMove(pos));
}

std::vector<CPiece::CPath> CQueen::getAllMoves() const
{
    auto verticalMoves = getVerticalMoves();
    auto horizontalMoves = getHorizontalMoves();
    auto diagonalMoves = getDiagonalMoves();

    // Concatena todos os movimentos.
    auto& result = verticalMoves;
    for (auto path: horizontalMoves) result.push_back(path);
    for (auto path: diagonalMoves) result.push_back(path);

    return result;
}

EType CQueen::getType() const {
    return EType::QUEEN;
}

/* -------------------------------------------------------------------------- */

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

std::vector<CPiece::CPath> CKing::getAllMoves() const
{
    auto verticalMove = getVerticalMoves();
    auto horizontalMove = getHorizontalMoves();
    auto diagonalMove = getDiagonalMoves();

    auto& result = verticalMove;
    for (auto move: horizontalMove) result.push_back(move);
    for (auto move: diagonalMove) result.push_back(move);

    // Apenas 1 movimento em cada caminho.
    for (auto& move: result)
        while (move.size() > 1) move.pop_back();

    return result;
}

EType CKing::getType() const {
    return EType::KING;
}

}