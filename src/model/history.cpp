#include "../../include/model/history.hpp"

namespace ch {

CHistory::CHistory(CLocation (*const gameState)[8])
    :
    _cursor{0},
    _gameState{gameState}
{
}

CHistory::~CHistory()
{

}

void CHistory::addMove(CMove lastMove)
{
    while (_cursor) {
        _moves.pop_back();
        --_cursor;
    }

    _moves.push_back(lastMove);
}

void CHistory::undoLastMove()
{
    _moves[_cursor++].undoMove(_gameState);
}

void CHistory::redoLastMove()
{
    _moves[--_cursor].doMove(_gameState);
}

CMove CHistory::getLastMove() const
{
    return _moves.back();
}

std::size_t CHistory::numberOfMoves() const
{
    return _moves.size() - _cursor;
}

}