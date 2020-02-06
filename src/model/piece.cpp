#include "../../include/model/piece.hpp"


namespace ch {

CPiece::CPiece(CCoordinate curPos, EColor col):
    _position{curPos},
    _color{col}
{
    cout << "CPiece::CPiece" << endl;
}

CPiece::CPiece()
    :
    _color{EColor::NULL_COLOR}
{
    cout << "CPiece::CPiece" << endl;
}

CPiece::~CPiece()
{
    cout << "CPiece::~CPiece" << endl;
}

void CPiece::setColor(EColor color)
{
    cout << "CPiece::setColor" << endl;
    _color = color;
}

void CPiece::setType(EType type)
{
    cout << "CPiece::setType" << endl;
    _type = type;
}

void CPiece::setPosition(CCoordinate curPos)
{
    cout << "CPiece::setPosition" << endl;
    if (curPos.i < 0 || curPos.i > 7 || curPos.j < 0 || curPos.j > 7)
        throw new std::out_of_range{"Invalid position"};

    _position = curPos;
}

CMoveTree CPiece::getPossibleMoves(
    const CLocation (*const gameState)[8],
    const CHistory& history) const
{
    cout << "CPiece::getPossibleMoves" << endl;
    cout << _position.i << ", " << _position.j << endl;
    CMoveTree allMoves = getAllMoves(gameState, history);
    movesPruning(gameState, allMoves);

    return allMoves;
}

CCoordinate CPiece::getPosition() const
{
    cout << "CPiece::getPosition" << endl;
    return _position;
}

EColor CPiece::getColor() const
{
    cout << "CPiece::getColor" << endl;
    return _color;
}

EType CPiece::getType() const
{
    cout << "CPiece::getType" << endl;
    return _type;
}

bool CPiece::wasMoved() const
{
    cout << "CPiece::wasMoved" << endl;
    return _wasMoved;
}

void CPiece::movesPruning(
            const CLocation (*const gameState)[8],
            CMoveTree& tree) const
{
    cout << "CPiece::movesPruning" << endl;

    for (auto& branch: tree) {
        for (unsigned int k = 0; k < branch.numberOfMoves(); ++k) {
            auto curPos = branch.get(k).getMove().getTo();
            CLocation location = gameState[curPos.i][curPos.j];

            if (!location.isEmpty()) {
                if (location.getColor() == _color)
                    branch.pruning(k);
                else
                    branch.pruning(k + 1);
                break;
            }
        }
    }
}

bool CPiece::isDiagonalMove(CCoordinate dest) const
{
    auto di = abs(_position.i - dest.i);
    auto dj = abs(_position.j - dest.j);

    // Movimento diagonais possuem o mesmo deslocamento entre os
    // eixos em termos absolutos.
    return di == dj;
}

bool CPiece::isVerticalMove(CCoordinate dest) const
{
    auto di = _position.i - dest.i;
    auto dj = _position.j - dest.j;

    // O movitento vertical não possui deslocamento horizontal.
    return dj == 0 && di != 0;
}

bool CPiece::isHorizontalMove(CCoordinate dest) const
{
    auto di = _position.i - dest.i;
    auto dj = _position.j - dest.j;

    // O movimento horizontal não possui deslocamento vertical.
    return di == 0 && dj != 0;
}

bool CPiece::isForwardMove(CCoordinate curPos) const
{
    // As peças brancas estão em i = [0 .. 0] e as peças pretas
    // estão em i = [6 .. 7].
    auto di = curPos.i - _position.i;

    return (
        (di > 0 && _color == EColor::WHITE) ||
        (di < 0 && _color == EColor::BLACK)
    );
}

int CPiece::getMoveRange(CCoordinate dest) const
{
    auto di = abs(_position.i - dest.i);
    auto dj = abs(_position.j - dest.j);

    // Distância de Manhattan.
    return di + dj;
}

CMoveTree CPiece::getHorizontalMoves() const
{
    auto curPos = _position;
    CMoveBranch leftBranch{}, rightBranch{};
    CMoveTree tree{};

    // Adiciona os movimentos à esquerda da posição atual.
    for (int j = curPos.j - 1; j >= 0; --j)
        leftBranch.append(CMove{CAtomicMove{curPos, {curPos.i, j}}});

    // Adiciona os movimentos à direita da posição atual.
    for (int j = curPos.j + 1; j < 8; ++j)
        rightBranch.append(CMove{CAtomicMove{curPos, {curPos.i, j}}});

    // Adiciona as direções em que há movimentos.
    tree.append(leftBranch);
    tree.append(rightBranch);

    return tree;
}

CMoveTree CPiece::getVerticalMoves() const
{
    auto curPos = _position;
    CMoveBranch upPath{}, downPath{};
    CMoveTree tree{};

    // Adiciona os movimentos abaixo da posição atual.
    for (int i = curPos.i - 1; i >= 0; --i)
        downPath.append(CMove{CAtomicMove{curPos, {i, curPos.j}}});

    // Adiciona os movimentos acima da posição atual.
    for (int i = curPos.i + 1; i < 8; ++i)
        upPath.append(CMove{CAtomicMove{curPos, {i, curPos.j}}});

    // Adiciona as direções em que há movimentos.
    tree.append(downPath);
    tree.append(upPath);

    return tree;
}

CMoveTree CPiece::getDiagonalMoves() const
{
    auto curPos = _position;
    CMoveBranch diagonalLUPath{}, diagonalRUPath{},
        diagonalLDPath{}, diagonalRDPath{};

    CMoveTree tree{};

    // Adiciona os movimentos à esquerda superior em relação a posição atual.
    for (int lu = 1; curPos.i + lu < 8 && curPos.j - lu >= 0; ++lu)
        diagonalLUPath.append(
            CMove{CAtomicMove{curPos, {curPos.i + lu, curPos.j - lu}}});

    // Adiciona os movimentos à direita superior em relação a posição atual.
    for (int ru = 1; curPos.i + ru < 8 && curPos.j + ru < 8; ++ru)
        diagonalRUPath.append(
            CMove{CAtomicMove{curPos, {curPos.i + ru, curPos.j + ru}}});

    // Adiciona os movimentos à esquerda inferior em relação a posição atual.
    for (int ld = 1; curPos.i - ld >= 0 && curPos.j - ld >= 0; ++ld)
        diagonalLDPath.append(
            CMove{CAtomicMove{curPos, {curPos.i - ld, curPos.j - ld}}});

    // Adiciona os movimentos à direita inferior em relação a posição atual.
    for (int rd = 1; curPos.i - rd >= 0 && curPos.j + rd < 8; ++rd)
        diagonalRDPath.append(
            CMove{CAtomicMove{curPos, {curPos.i - rd, curPos.j + rd}}});


    // Adiciona as direções em que há movimentos.
    tree.append(diagonalLUPath);
    tree.append(diagonalRUPath);
    tree.append(diagonalLDPath);
    tree.append(diagonalRDPath);

    return tree;
}

CMoveTree CPiece::getRangeBasedMoves(int range) const
{
    auto& curPos = _position;
    CMoveTree tree{};

    // Certamente deve haver algum algoritmo melhor do que este...
    for (unsigned int i = 0; i < 8; ++i)
        for (unsigned int j = 0; j < 8; ++j)
            if (getMoveRange(CCoordinate{i, j}) == range) {
                    tree.append(
                        CMoveBranch{CMove{CAtomicMove{curPos, {i, j}}}});
            }

    return tree;
}

}