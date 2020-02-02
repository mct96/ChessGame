#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <iomanip>

#include "color.hpp"
#include "type.hpp"
#include "location.hpp"
#include "coordinate.hpp"
#include "specialized_pieces.hpp"
#include "move_path.hpp"

using namespace std;

extern int main();

namespace ch {

class CGame {
public:
    // Apenas para propósito de teste.
    friend int ::main();

    CGame();
    ~CGame();

    CLocation getPieceAt(CCoordinate pos) const;

    // Indica se o jogo está em check.
    bool isCheck() const;

    // Indica se foi check mate.
    bool isCheckMate() const;

    // Indica qual jogador deve se mover.
    EColor getPlayerTurn() const;

    CMoveTree possibleMoves(
        CCoordinate pos) const;

    // Move uma peça da posição "from" para a posição "to".
    // Note que a peça em "from" deve estar na lista de peça do jogador que
    // possue a vez de jogar.
    void move(CCoordinate from, CCoordinate to);
protected:
    CPiece * const factoryPiece(
        EType type,
        EColor color
    ) const;

    // Trata o caso de Promotions no jogo.
    void promotion(
        std::shared_ptr<CPiece> piece, CMoveTree& moves);

    // Inicializa todos as peças do jogo instanciando e inicializando cada uma
    // na sua respectiva posição.
    void initializeGame();

    // Inicializa apenas as peças brancas.
    void initializeWhitePieces();

    // Inicializa apenas as peças pretas.
    void initializeBlackPieces();

    // Alterna entre os jogadores.
    void swapPlayerTurn();

    // Indica se a peça "piece" específicada nos parâmetros pode mover para a
    // posição especificada pelo parâmetro "to".
    bool canMoveTo(std::shared_ptr<CPiece> piece, CCoordinate to) const;

    // Retorna o rei que possui a cor específicada no parâmetro.
    std::shared_ptr<CPiece> getKing(EColor kingColor) const;



private:

    // Todas as posições do jogo.
    CLocation _boardLocations[8][8];
    // Indica qual jogador deve jogar.

    EColor _playerTurn;

    // Guarda o histórico de jogadas do jogo. Utilizado para validar jogadas
    // como o en passant e castling.
    CHistory _history;

    static CPawn   _pawn;
    static CRook   _rook;
    static CKnight _knight;
    static CBishop _bishop;
    static CQueen  _queen;
    static CKing   _king;
};

}