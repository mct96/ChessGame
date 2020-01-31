#pragma once

#include "piece.hpp"
#include "history.hpp"
#include "location.hpp"
#include "path.hpp"

namespace ch {

class CPawn: public CPiece {
public:
    CPawn();
    virtual ~CPawn();

    std::vector<CPath> getAllMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const override;

    // Semelhante a "pawnMove". Extende as possiblidades da peça do tipo "Pawn"
    // para permitir o en Passant.
    void enPassant(
        const CHistory& history,
        std::vector<CPath>& moves) const;

    // O peão possui um comportamento anômalo, pois a movimentação dessa peça
    // não segue o padrão do seu movimento ordinário. Para resolver é necessário
    // saber o estado das demais peças, portanto, esse problema deve ser resol-
    // vido aqui. Para isso, esse método extende a lista de movimentos possiveis
    // quando a peça for do tipo "Pawn".
    void pawnMove(
        const CLocation (*const gameState)[8],
        std::vector<CPath>& moves) const;


    std::vector<CPath>& pawnPruningVerticalAttack(
        const CLocation (*const gameState)[8],
        std::vector<CPath>& moves) const;

};

class CRook: public CPiece {
public:
    CRook();
    virtual ~CRook();

    std::vector<CPath> getAllMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const override;

    // Semelhante a "pawnMove". Extende as possibilidades para as peças
    // do tipo "Rock".
    void castling(std::vector<CPath>& moves) const;

};

class CKnight: public CPiece {
public:
    CKnight();
    virtual ~CKnight();

    std::vector<CPath> getAllMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const override;

};

class CBishop: public CPiece {
public:
    CBishop();
    virtual ~CBishop();

    std::vector<CPath> getAllMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const override;
};

class CQueen: public CPiece {
public:
    CQueen();
    virtual ~CQueen();

    std::vector<CPath> getAllMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const override;
};

class CKing: public CPiece {
public:
    CKing();
    virtual ~CKing();

    std::vector<CPath> getAllMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const override;
};
}