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

CLocation CBoardController::getPieceAt(CCoordinate position) const
{
    return _game.getPieceAt(position);
}

void CBoardController::move(CCoordinate from, CCoordinate to)
{
    _game.move(from, to);
}

std::vector<CPath> CBoardController::possibleMoves(
        CCoordinate pos) const
{
    return _game.possibleMoves(pos);
}

bool CBoardController::isCheck() const
{
    return _game.isCheck();
}

ch::EColor CBoardController::getPlayerTurn() const
{
    return _game.getPlayerTurn();
}

}