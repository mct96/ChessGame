#include "../../include/model/history.hpp"

namespace ch {

CHistory::CHistory(CLocation (*const gameState)[8])
    :
    _turns{},
    _cursor{0},
    _gameState{gameState}
{
}

CHistory::~CHistory()
{

}

void CHistory::append(
    const CPlayer *player,
    CCoordinate from,
    CCoordinate to,
    CLocation stateBefore,
    CLocation stateAfter)
{
    append(CTurn{player, from, to, stateBefore, stateAfter});
}

void CHistory::append(CTurn turn)
{
    while (_cursor) {
        _turns.pop_back();
        --_cursor;
    }

    _turns.push_back(turn);
}

unsigned int CHistory::getTurnNumber() const
{
    return _turns.size();
}

CTurn CHistory::getLastTurn() const
{
    return getTurn(0);
}

CTurn CHistory::getTurn(unsigned int i) const
{
    auto lastIndex = _turns.size() - 1;
    return _turns[lastIndex - i];
}

void CHistory::undoLastTurn() {
    if (_cursor + 1 >= _turns.size()) return;

    auto lastTurn = getLastTurn();

    _gameState[lastTurn._to.i][lastTurn._to.j].setPiece(
        lastTurn._stateBefore.getType(),
        lastTurn._stateBefore.getColor()
    );

    _gameState[lastTurn._from.i][lastTurn._from.j].setPiece(
        lastTurn._stateAfter.getType(),
        lastTurn._stateAfter.getColor()
    );

    _cursor++;
}

void CHistory::redoLastTurn() {
    if (_cursor <= 0) return;

    _cursor--;

    auto turn = getTurn(_cursor);

    _gameState[turn._from.i][turn._from.j].removePiece();

    _gameState[turn._to.i][turn._to.j].setPiece(
        turn._stateAfter.getType(),
        turn._stateAfter.getColor()
    );
}

}