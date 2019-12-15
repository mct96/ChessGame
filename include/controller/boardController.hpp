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
    std::shared_ptr<CPiece> getPieceAt(CPosition position) const;

    /**
     * Move a peça que está na posição "from" para a posição "to". Caso o movi-
     * mento seja inválido uma exceção é emitida.
     */
    void move(CPosition from, CPosition to);

    /**
     *  Retorna todos os possiveis movimentos que a peça "piece" pode realizar.
     */
    std::vector<CPiece::CPath> possibleMoves(
        std::shared_ptr<CPiece> piece) const;

    EColor getPlayerTurn() const;
protected:


private:
    CGame _game;

};


}