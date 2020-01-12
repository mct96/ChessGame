#pragma once

#include <cmath>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

#include "color.hpp"
#include "coordinate.hpp"
#include "type.hpp"

using namespace std;

namespace ch {

class CPiece {

public:
    using CPath = std::vector<CCoordinate>;

    CPiece(CCoordinate position, EColor color);
    virtual ~CPiece();

    // Indica se a peça pode se mover para uma determinada posição. Esse método
    // indica se é válido "genericamente" e não analisa a validade baseado no
    // estado do jogo.
    virtual bool canMoveTo(CCoordinate position) const = 0;

    // Gera todas as posições no qual a peça pode se mover. As posições que
    // estão na mesma direção são agrupadas, por exemplo, todos os movimentos
    // na direção diagonal superior esquerda são unidos em um único vetor.
    virtual std::vector<CPath> getAllMoves() const = 0;

    // Identifica qual o tipo da peça.
    virtual EType getType() const = 0;

    // Move a peça para a posição especificada se for possível mover para esta
    // posição.
    bool moveTo(CCoordinate position);

    CCoordinate getPosition() const;

    void setColor(EColor color);
    EColor getColor() const;

    // Indica se a peça foi movida.
    bool wasMoved() const;

    bool isActive() const;
    void setActivity(bool activity);

protected:
    void setPosition(CCoordinate position);

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
    bool _wasMoved;      // Indica se a peça já foi movida. (Peão e Torre)
    bool _isActive;      // Indica se a peça ainda está em jogo.
};

}