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

CMoveTree CPawn::getAllMoves(
    const  CLocation (*const gameState)[8],
    const CHistory& history) const
{
    cout << "CPawn::getAllMoves" << endl;

    auto forwardMove = CMoveTree{};
    auto branch = CMoveBranch{};
    auto curPos = getPosition();

    if (getColor() == EColor::WHITE && curPos.i < 7) {
        branch.append(CMove{CAtomicMove{curPos, {curPos.i + 1, curPos.j}}});

        if (curPos.i == 1)
            branch.append(CMove{CAtomicMove{curPos, {curPos.i + 2, curPos.j}}});

    } else if (getColor() == EColor::BLACK && curPos.i > 0) {
        branch.append(CMove{CAtomicMove{curPos, {curPos.i - 1, curPos.j}}});

        if (curPos.i == 6)
            branch.append(CMove{CAtomicMove{curPos, {curPos.i - 2, curPos.j}}});
    }

    forwardMove.append(branch);

    pawnPruningVerticalAttack(gameState, forwardMove);
    enPassant(history, forwardMove);
    pawnMove(gameState, forwardMove);

    return forwardMove;
}

void CPawn::enPassant(
    const CHistory& history, CMoveTree& moves) const
{
    cout << "CPawn::enPassant" << endl;

    auto curPos = getPosition();

    if (history.getTurnNumber() == 0) return;

    auto lastMove = history.getLastTurn();
    auto wasPawn = lastMove._stateAfter.getType() == EType::PAWN;
    auto distance = lastMove._from.i - lastMove._to.i;

    if (wasPawn && abs(distance) == 2 && lastMove._to.i == curPos.i) {
        auto branch = CMoveBranch{};
        int mid = static_cast<int>(lastMove._from.i + lastMove._to.i)/2;
        CAtomicMove atomicMove{curPos, {mid, lastMove._to.j}};
        CAtomicMove sideEffect{lastMove._to, lastMove._to};

        CMove move{atomicMove};
        move.setSideEffect(sideEffect);

        branch.append(move);

        moves.append(branch);
    }
}

void CPawn::pawnMove(
    const CLocation (*const gameState)[8], CMoveTree& moves) const
{

    // O peão curPossui comportamento anomalo. Ele não ataca as curPosições em que ele
    // move. Ele ataca nas diagonais em direção ao inimigo, move em direção ao
    // inimigo uma casa, pode salta um ou duas curPosição na partida. Esta função
    // trata esses três casos.
    cout << "CPawn::pawnMove" << endl;

    auto curPos = getPosition();

    cout << curPos.i << ", "  << curPos.j << endl;

    auto branch = CMoveBranch{};

    if (getColor() == EColor::WHITE) {
        if (curPos.i + 1 <= 7 && curPos.j + 1 <= 7 &&
            gameState[curPos.i + 1][curPos.j + 1].getColor() == EColor::BLACK)
            branch.append(
                CMove{CAtomicMove{curPos, {curPos.i + 1 , curPos.j + 1}}});

        if (curPos.i + 1 <= 7 && curPos.j - 1 >= 0 &&
            gameState[curPos.i + 1][curPos.j - 1].getColor() == EColor::BLACK)
            branch.append(
                CMove{CAtomicMove{curPos, {curPos.i + 1 , curPos.j - 1}}});

    } else if (getColor() == EColor::BLACK ) {
        if (curPos.i - 1 >= 0 && curPos.j + 1 <= 7 &&
            gameState[curPos.i - 1][curPos.j + 1].getColor() == EColor::WHITE)
            branch.append(
                CMove{CAtomicMove{curPos, {curPos.i - 1 , curPos.j + 1}}});

        if (curPos.i - 1 >= 0 && curPos.j - 1 >= 0 &&
            gameState[curPos.i - 1][curPos.j - 1].getColor() == EColor::WHITE)
            branch.append(
                CMove{CAtomicMove{curPos, {curPos.i - 1 , curPos.j - 1}}});
    }

    moves.append(branch);
}

void CPawn::pawnPruningVerticalAttack(
    const CLocation (*const gameState)[8],
    CMoveTree& moves) const
{
    cout << "CPawn::pawnPruningVerticalAttack" << endl;

    auto& branch = moves.getBranch(0);
    for (unsigned int k = 0; k < branch.numberOfMoves(); ++k) {
        auto target = branch[k].getMove().getTo();
        auto targetState = gameState[target.i][target.j];

        if (!targetState.isEmpty() && targetState.getColor() != getColor()) {
            branch.pruning(k);
            break;
        }
   }


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

CMoveTree CRook::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
{
    auto verticalMove = getVerticalMoves();
    auto horizontalMove = getHorizontalMoves();

    // Extende os resultados dos movimentos verticais e horizontais.
    auto& result = verticalMove;
    for (auto& branch: horizontalMove)
        result.append(branch);

    return result;

}

void CRook::castling(CMoveTree& moves) const
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

CMoveTree CKnight::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
{
    auto rangeMoves = getRangeBasedMoves(3);
    auto result = decltype(rangeMoves){};

    // Adiciona os movimentos com distância igual a 3 e que não são
    // nem verticais e nem horizontais.
    for (auto& branch: rangeMoves) {
        auto destination = branch[0].getMove().getTo();
        if (!isVerticalMove(destination) && !isHorizontalMove(destination))
            result.append(branch);
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

CMoveTree CBishop::getAllMoves(
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

CMoveTree CQueen::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
{
    auto verticalMoves = getVerticalMoves();
    auto horizontalMoves = getHorizontalMoves();
    auto diagonalMoves = getDiagonalMoves();

    // Concatena todos os movimentos.
    auto& result = verticalMoves;
    for (auto& branch: horizontalMoves) result.append(branch);
    for (auto& branch: diagonalMoves) result.append(branch);

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

CMoveTree CKing::getAllMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
{
    auto verticalMove = getVerticalMoves();
    auto horizontalMove = getHorizontalMoves();
    auto diagonalMove = getDiagonalMoves();

    auto& tree = verticalMove;
    for (auto& branch: horizontalMove) tree.append(branch);
    for (auto& branch: diagonalMove) tree.append(branch);

    for (auto& branch: tree) {
        if (branch.numberOfMoves() > 1)
            branch.pruning(1);
    }
    return tree;
}

}