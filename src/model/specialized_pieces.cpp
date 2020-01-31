#include "../../include/model/specialized_pieces.hpp"


namespace ch {

CPawn::CPawn()
    :
    CPiece{}
{

}

CPawn::~CPawn()
{

}

std::vector<CPath> CPawn::getAllMoves(
    const  CLocation (*const gameState)[8],
    const CHistory& history) const
{
    cout << "CPawn::getAllMoves" << endl;

    auto forwardMove = std::vector<CPath>{};
    auto path = CPath{};
    auto pos = getPosition();

    if (getColor() == EColor::WHITE && pos.i < 7) {
        path.push_back({pos.i + 1, pos.j});
        if (pos.i == 1) path.push_back({pos.i + 2, pos.j});

    } else if (getColor() == EColor::BLACK && pos.i > 0) {
        path.push_back({pos.i - 1, pos.j});
        if (pos.i == 6) path.push_back({pos.i - 2, pos.j});
    }

    forwardMove.push_back(path);

    forwardMove = pawnPruningVerticalAttack(gameState, forwardMove);
    enPassant(history, forwardMove);
    pawnMove(gameState, forwardMove);

    return forwardMove;
}

void CPawn::enPassant(
    const CHistory& history, std::vector<CPath>& moves) const
{
    cout << "CPawn::enPassant" << endl;

    if (history.getTurnNumber() == 0) return;

    auto lastMove = history.getLastTurn();
    auto wasPawn = lastMove._stateAfter.getType() == EType::PAWN;
    auto distance = lastMove._from.i - lastMove._to.i;

    if (wasPawn && abs(distance) == 2 && lastMove._to.i == getPosition().i) {
        auto path = CPath{};
        int mid = static_cast<int>(lastMove._from.i + lastMove._to.i)/2;
        path.push_back({mid, lastMove._to.j});
        moves.push_back(path);
    }
}

void CPawn::pawnMove(
    const CLocation (*const gameState)[8], std::vector<CPath>& moves) const
{

    // O peão possui comportamento anomalo. Ele não ataca as posições em que ele
    // move. Ele ataca nas diagonais em direção ao inimigo, move em direção ao
    // inimigo uma casa, pode salta um ou duas posição na partida. Esta função
    // trata esses três casos.
    cout << "CPawn::pawnMove" << endl;

    auto pos = getPosition();

    auto path = CPath{};

    if (getColor() == EColor::WHITE) {
        if (pos.i + 1 <= 7 && pos.j + 1 <= 7 &&
            gameState[pos.i + 1][pos.j + 1].getColor() == EColor::BLACK)
            path.push_back({pos.i + 1 , pos.j + 1});

        if (pos.i + 1 <= 7 && pos.j - 1 >= 0 &&
            gameState[pos.i + 1][pos.j - 1].getColor() == EColor::BLACK)
            path.push_back({pos.i + 1 , pos.j - 1});

    } else if (getColor() == EColor::BLACK ) {
        if (pos.i - 1 >= 0 && pos.j + 1 <= 7 &&
            gameState[pos.i - 1][pos.j + 1].getColor() == EColor::WHITE)
            path.push_back({pos.i - 1 , pos.j + 1});

        if (pos.i - 1 >= 0 && pos.j - 1 >= 0 &&
            gameState[pos.i - 1][pos.j - 1].getColor() == EColor::WHITE)
            path.push_back({pos.i - 1 , pos.j - 1});
    }

    moves.push_back(path);
}

std::vector<CPath>& CPawn::pawnPruningVerticalAttack(
    const CLocation (*const gameState)[8],
    std::vector<CPath>& moves) const
{
    cout << "CPawn::pawnPruningVerticalAttack" << endl;
    auto& path = moves[0];

    for (unsigned int k = 0; k < path.size(); ++k) {
        auto position = gameState[path[k].i][path[k].j];
        if (!position.isEmpty() && position.getColor() != getColor()) {
            path.erase(path.begin() + k);
            break;
        }
   }

    return moves;
}

/* -------------------------------------------------------------------------- */

CRook::CRook()
    :
    CPiece{}
{

}

CRook::~CRook()
{

}

std::vector<CPath> CRook::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
{
    auto verticalMove = getVerticalMoves();
    auto horizontalMove = getHorizontalMoves();

    // Extende os resultados dos movimentos verticais e horizontais.
    auto& result = verticalMove;
    for (auto element: horizontalMove)
        result.push_back(element);

    return result;

}

void CRook::castling(std::vector<CPath>& moves) const
{

}

/* -------------------------------------------------------------------------- */

CKnight::CKnight()
    :
    CPiece{}
{

}

CKnight::~CKnight()
{

}

std::vector<CPath> CKnight::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
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

/* -------------------------------------------------------------------------- */

CBishop::CBishop()
    :
    CPiece{}
{

}

CBishop::~CBishop()
{

}

std::vector<CPath> CBishop::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
{
    return getDiagonalMoves();
}

/* -------------------------------------------------------------------------- */

CQueen::CQueen()
    :
    CPiece{}
{

}

CQueen::~CQueen()
{

}

std::vector<CPath> CQueen::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
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

/* -------------------------------------------------------------------------- */

CKing::CKing()
    :
    CPiece{}
{

}

CKing::~CKing()
{

}

std::vector<CPath> CKing::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
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

}