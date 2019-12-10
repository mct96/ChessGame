#include "../../include/controller/boardController.hpp"

namespace ch {

CBoardController::CBoardController()
    :
    _game{}
{

}

CBoardController::~CBoardController()
{

}


std::shared_ptr<CPiece> CBoardController::getPieceAt(CPosition position) const
{
    return _game.getPieceAt(position);
}

void CBoardController::move(CPosition from, CPosition to)
{
    _game.move(from, to);
}

std::vector<CPiece::CPath> CBoardController::
    possibleMoves(std::shared_ptr<CPiece> piece) const
{
    if (piece->getColor() != _game.getPlayerTurn())
        return std::vector<CPiece::CPath>{};

    auto moves = piece->getAllMoves();
    return _game.movesPruning(moves);
}

}