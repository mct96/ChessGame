#include "../../include/model/piece.hpp"


namespace ch {

CPiece::CPiece(CPosition pos, EColor col):
    _position{pos},
    _color{col},
    _wasMoved{false},
    _isActive{true}
{

}

CPiece::~CPiece()
{

}


bool CPiece::moveTo(CPosition pos)
{
    setPosition(pos);
    return true;
}

void CPiece::setPosition(CPosition pos)
{
    if (pos.i < 0 || pos.i > 7 || pos.j < 0 || pos.j > 7)
        throw new std::out_of_range{"Invalid position"};

    _position = pos;
}

CPosition CPiece::getPosition() const
{
    return _position;
}

void CPiece::setColor(EColor col)
{
    _color = col;
}

EColor CPiece::getColor() const
{
    return _color;
}

bool CPiece::wasMoved() const
{
    return _wasMoved;
}

bool CPiece::isActive() const
{
    return _isActive;
}

void CPiece::setActivity(bool activity)
{
    _isActive = activity;
}

bool CPiece::isDiagonalMove(CPosition dest) const
{
    auto di = abs(_position.i - dest.i);
    auto dj = abs(_position.j - dest.j);

    // Movimento diagonais possuem o mesmo deslocamento entre os
    // eixos em termos absolutos.
    return di == dj;
}

bool CPiece::isVerticalMove(CPosition dest) const
{
    auto di = _position.i - dest.i;
    auto dj = _position.j - dest.j;

    // O movitento vertical não possui deslocamento horizontal.
    return dj == 0 && di != 0;
}

bool CPiece::isHorizontalMove(CPosition dest) const
{
    auto di = _position.i - dest.i;
    auto dj = _position.j - dest.j;

    // O movimento horizontal não possui deslocamento vertical.
    return di == 0 && dj != 0;
}

bool CPiece::isForwardMove(CPosition destination) const
{
    // As peças brancas estão em i = [1 .. 2] e as peças pretas
    // estão em i = [7 .. 8].
    auto di = destination.i - _position.i;

    return (
        (di > 0 && _color == EColor::WHITE) ||
        (di < 0 && _color == EColor::BLACK)
    );
}

int CPiece::getMoveRange(CPosition dest) const
{
    auto di = abs(_position.i - dest.i);
    auto dj = abs(_position.j - dest.j);

    // Distância de Manhattan.
    return di + dj;
}

std::vector<CPiece::CPath> CPiece::getHorizontalMoves() const
{
    auto pos = _position;
    CPath leftPath{}, rightPath{};
    std::vector<CPath> moves{};

    // Adiciona os movimentos à esquerda da posição atual.
    for (int j = pos.j - 1; j > 0; --j)
        leftPath.push_back({pos.i, j});

    // Adiciona os movimentos à direita da posição atual.
    for (int j = pos.j + 1; j <= 8; ++j)
        rightPath.push_back({pos.i, j});

    // Adiciona as direções em que há movimentos.
    if (leftPath.size()) moves.push_back(leftPath);
    if (rightPath.size()) moves.push_back(rightPath);

    return moves;
}

std::vector<CPiece::CPath> CPiece::getVerticalMoves() const
{
    auto pos = _position;
    CPath upPath{}, downPath{};
    std::vector<CPath> moves{};

    // Adiciona os movimentos abaixo da posição atual.
    for (int i = pos.i - 1; i > 0; --i)
        downPath.push_back({i, pos.j});

    // Adiciona os movimentos acima da posição atual.
    for (int i = pos.i + 1; i <= 8; ++i)
        upPath.push_back({i, pos.j});

    // Adiciona as direções em que há movimentos.
    if (upPath.size()) moves.push_back(upPath);
    if (downPath.size()) moves.push_back(downPath);

    return moves;
}

std::vector<CPiece::CPath> CPiece::getDiagonalMoves() const
{
    auto pos = _position;
    CPath diagonalLUPath{}, diagonalRUPath{},
        diagonalLDPath{}, diagonalRDPath{};

    std::vector<CPath> moves{};

    // Adiciona os movimentos à esquerda superior em relação a posição atual.
    for (int lu = 1; pos.i + lu <= 8 && pos.j - lu > 0; ++lu)
        diagonalLUPath.push_back({pos.i + lu, pos.j - lu});

    // Adiciona os movimentos à direita superior em relação a posição atual.
    for (int ru = 1; pos.i + ru <= 8 && pos.j + ru <= 8; ++ru)
        diagonalLUPath.push_back({pos.i + ru, pos.j + ru});

    // Adiciona os movimentos à esquerda inferior em relação a posição atual.
    for (int ld = 1; pos.i - ld > 0 && pos.j - ld > 0; ++ld)
        diagonalLUPath.push_back({pos.i - ld, pos.j - ld});

    // Adiciona os movimentos à direita inferior em relação a posição atual.
    for (int rd = 1; pos.i - rd > 0 && pos.j + rd <= 8; ++rd)
        diagonalLUPath.push_back({pos.i - rd, pos.j + rd});


    // Adiciona as direções em que há movimentos.
    if (diagonalLUPath.size()) moves.push_back(diagonalLUPath);
    if (diagonalRUPath.size()) moves.push_back(diagonalRUPath);
    if (diagonalLDPath.size()) moves.push_back(diagonalLDPath);
    if (diagonalRDPath.size()) moves.push_back(diagonalRDPath);

    return moves;
}

std::vector<CPiece::CPath> CPiece::getRangeBasedMoves(int range) const
{
    std::vector<CPath> moves{};

    // Certamente deve haver algum algoritmo melhor do que este...
    for (int i=1; i<=8; ++i)
        for (int j=1; j<=8; ++j)
            if (getMoveRange(CPosition{i, j}) == range) {
                    CPath path{};
                    path.push_back({i, j});
                    moves.push_back(path);
            }

    return moves;
}

}