#pragma once

#include <memory>

#include "../model/game.hpp"
#include "../model/piece.hpp"

namespace ch {

class CBoardController {

public:
    CBoardController();
    ~CBoardController();

    /**
     *  Obtém a peça que está na posição específicada pelo parâmetro. Caso não
     *  haja nenhuma peça na posição indica é retornado nullptr.
     */
    CLocation getPieceAt(CCoordinate position) const;

    /**
     * Move a peça que está na posição "from" para a posição "to". Caso o movi-
     * mento seja inválido uma exceção é emitida.
     */
    void move(CCoordinate from, CCoordinate to);

    /**
     */
    CMoveTree possibleMoves(
        CCoordinate pos) const;

    bool isCheck() const;

    EColor getPlayerTurn() const;
protected:


private:
    CGame _game;

};


}