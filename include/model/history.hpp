#pragma once

#include <vector>

#include "player.hpp"
#include "location.hpp"
#include "coordinate.hpp"
#include "path.hpp"

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

    void addMove(CMove move);
    void undoLastMove();
    void redoLastMove();

    CMove getLastMove() const;

    std::size_t numberOfMoves() const;

protected:

private:
    std::vector<CMove> _moves;
    unsigned int _cursor;

    CLocation (*const _gameState)[8];
};

}