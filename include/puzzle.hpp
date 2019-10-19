#pragma once

#include <vector>
#include <memory>

#include <iostream>

#include "piece_behavior.hpp"
#include "move_path.hpp"

#include <istream>

extern int main();

namespace ch {

class CPuzzle {
public:
    friend int ::main();

    CPuzzle();
    ~CPuzzle();

    bool isCheck() const;
    bool isCheckMate() const;

    EColor getPlayerTurn() const;

    void move(CPosition from, CPosition to);



protected:
    std::shared_ptr<CPiece> getPieceAt(CPosition pos) const;

    bool isFreePosition(CPosition pos) const;
public:
    // Remove as posições já ocupadas da lista de possíveis movimentos.
    std::vector<CPiece::CPath> movesPruning(
            std::vector<CPiece::CPath> moves) const;

    void removePieceAt(CPosition pos);

    // O peão é diferente, pois a movimentação dessa peça não segue o padrão do
    // seu movimento ordinário. Para resolver é necessário saber o estado do jo-
    // go, portanto, esse problema deve ser resolvido aqui.
    void pawnMove(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves);

    void castling(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves);

    void enPassant(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves);

    void promotion(
        std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves);


    void exceptionalMoves(
            std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves);

    void initializeGame();
    void initializeWhitePieces();
    void initializeBlackPieces();

    void swapPlayerTurn();

    void print();

private:
    EColor _playerTurn;

    std::vector<std::shared_ptr<CPiece>> _whitePieces;
    std::vector<std::shared_ptr<CPiece>> _blackPieces;

};

}