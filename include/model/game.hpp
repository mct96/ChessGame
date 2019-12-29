#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <iomanip>

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

    // Indica se o jogo está em check.
    bool isCheck() const;

    // Indica se foi check mate.
    bool isCheckMate() const;

    // Indica qual jogador deve se mover.
    EColor getPlayerTurn() const;

    // Move uma peça da posição "from" para a posição "to".
    // Note que a peça em "from" deve estar na lista de peça do jogador que
    // possue a vez de jogar.
    void move(CPosition from, CPosition to);

    // Retorna a peça que está na posição "pos". Caso não haja peça é returnado
    // nullptr.
    std::shared_ptr<CPiece> getPieceAt(CPosition pos) const;

    // Indica se uma posição está vazia.
    bool isFreePosition(CPosition pos) const;

    // Indica se um enimigo ocupa a posição específicada.
    bool hasAnEnemyAt(CPosition pos) const;

    // Remove as posições invalidas para uma peça. Casa peça conhece os movimen-
    // tos possiveis, porém não possuem informações sobre o estado do jogo, por-
    // tanto este método deve remover os movimentos que não são válidos, isto é,
    // posições que possuem peças da mesma cor ou que passam por uma dessas.
    std::vector<CPiece::CPath> movesPruning(
            std::vector<CPiece::CPath> moves) const;

    // Associa todos os tipos extraordinários de movimentos, isto é, pawnMove,
    // castling, enPassant.
    void exceptionalMoves(
            std::shared_ptr<CPiece> piece,
            std::vector<CPiece::CPath>& moves) const;

protected:
    // Remove que está na posição "pos". Esse método é usado quando uma peça é
    // capturada.
    void removePieceAt(CPosition pos);

    // O peão possui um comportamento anomalo, pois a movimentação dessa peça
    // não segue o padrão do seu movimento ordinário. Para resolver é necessário
    // saber o estado das demais peças, portanto, esse problema deve ser resol-
    // vido aqui. Para isso, esse método extende a lista de movimentos possiveis
    // quando a peça for do tipo "Pawn".
    void pawnMove(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves) const;

    // Semelhante a "pawnMove". Extende as possibilidades para as peças
    // do tipo "Rock".
    void castling(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves) const;

    // Semelhante a "pawnMove". Extende as possiblidades da peça do tipo "Pawn"
    // para permitir o en Passant.
    void enPassant(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves) const;

    // Trata o caso de Promotions no jogo.
    void promotion(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves);

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
    bool canMoveTo(std::shared_ptr<CPiece> piece, CPosition to) const;

    // Retorna o rei que possui a cor específicada no parâmetro.
    std::shared_ptr<CPiece> getKing(EColor kingColor) const;

    // Apenas utilizado para testes. Deverá ser substituido por logs conforme o
    // desenvolvimento avançar.
    void print();

private:
    int posToInt(CPosition pos) const;
    void updatePosition(std::shared_ptr<CPiece> piece, CPosition old);

    // Indica qual jogador deve jogar.
    EColor _playerTurn;

    // Guarda o histórico de jogadas do jogo. Utilizado para validar jogadas
    // como o en passant e castling.
    std::vector<std::pair<CPosition, CPosition>> _history;

    // TODO usar uma array associativo. Cada posição pode ser unicamente identi-
    // ficada através do cálculo i*8+j. Isso acelerará a verificação significa-
    // tivamente.
    std::unordered_map<int, std::shared_ptr<CPiece>> _whitePieces;
    std::unordered_map<int, std::shared_ptr<CPiece>> _blackPieces;

};

}