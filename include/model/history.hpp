#pragma once

#include <vector>

#include "player.hpp"
#include "location.hpp"
#include "coordinate.hpp"

namespace ch {

struct CTurn {
    const CPlayer *_player;

    CCoordinate _from;
    CCoordinate _to;

    // O estado do local de destino antes deste movimento.
    CLocation _stateBefore;
    CLocation _stateAfter;
};

class CHistory {
public:
    CHistory(CLocation (*const gameState)[8]);
    ~CHistory();

    void append(
        const CPlayer *player,
        CCoordinate from,
        CCoordinate to,
        CLocation stateBefore,
        CLocation stateAfter);

    void append(CTurn turn);

    unsigned int getTurnNumber() const;
    CTurn getLastTurn() const;
    CTurn getTurn(unsigned int i) const;

    void undoLastTurn();
    void redoLastTurn();

protected:

private:
    std::vector<CTurn> _turns;
    unsigned int _cursor;

    CLocation (*const _gameState)[8];
};

}