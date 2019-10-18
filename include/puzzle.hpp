#pragma once

#include <vector>
#include <memory>

#include <iostream>

#include "piece_behavior.hpp"

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

    void castling();
    void enPassant();
    void promotion();

    EColor getPlayerTurn() const;

    void move(CPosition from, CPosition to);



protected:
    std::shared_ptr<CPiece> getPieceAt(CPosition pos) const;

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