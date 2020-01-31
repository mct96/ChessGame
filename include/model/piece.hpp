#pragma once

#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "path.hpp"
#include "color.hpp"
#include "type.hpp"
#include "location.hpp"
#include "history.hpp"

using namespace std;

namespace ch {

class CPiece {

public:
    CPiece(CCoordinate position, EColor color);
    CPiece();
    virtual ~CPiece();

    void setColor(EColor color);
    void setType(EType type);
    void setPosition(CCoordinate position);

    std::vector<CPath> getPossibleMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const;

    CCoordinate getPosition() const;
    EColor getColor() const;
    EType getType() const;

    // Indica se a peça foi movida.
    bool wasMoved() const;

protected:
    // Gera todas as posições no qual a peça pode se mover. As posições que
    // estão na mesma direção são agrupadas, por exemplo, todos os movimentos
    // na direção diagonal superior esquerda são unidos em um único vetor.
    virtual std::vector<CPath> getAllMoves(
        const CLocation (*const gameState)[8],
        const CHistory& history) const = 0;

    // Remove as posições invalidas para uma peça. Casa peça conhece os movimen-
    // tos possiveis, porém não possuem informações sobre o estado do jogo, por-
    // tanto este método deve remover os movimentos que não são válidos, isto é,
    // posições que possuem peças da mesma cor ou que passam por uma dessas.
    std::vector<CPath> movesPruning(
            const CLocation (*const gameState)[8],
            std::vector<CPath> moves) const;

    // Indica se o movimento realizado pela peça até o destino é diagonal.
    bool isDiagonalMove(CCoordinate destination) const;

    // Indica se o movimento é vertical.
    bool isVerticalMove(CCoordinate destination) const;

    // Indica se o movimento é horizontal.
    bool isHorizontalMove(CCoordinate destination) const;

    // Indica se o movimento é para "frente". "Frente" varia de acordo com a
    // cor da peça.
    bool isForwardMove(CCoordinate destination) const;

    // Retorna a distância entre a posição atual e o destino.
    int getMoveRange(CCoordinate dest) const;

    // Obtém um vetor com todos os movimentos verticais possíveis.
    std::vector<CPath> getVerticalMoves() const;

    // Obtém um vetor com todos os movimentos horizontais possíveis.
    std::vector<CPath> getHorizontalMoves() const;

    // Obtém um vetor com todos os movimentos diagonais possíveis.
    std::vector<CPath> getDiagonalMoves() const;

    // Obtém um vetor com todos os movimentos cujo a distância de manhattan
    // é igual a "range".
    std::vector<CPath> getRangeBasedMoves(int range) const;

private:
    CCoordinate _position; // Indica a posição atual.
    EColor _color;       // Indica a cor da peça.
    EType _type;         // Indica o tipo de peça.
    bool _wasMoved;      // Indica se a peça já foi movida. (Peão e Torre)
};

}