#pragma once

#include "../../include/model/path.hpp"

namespace ch
{

CAtomicMove::CAtomicMove(CCoordinate from, CCoordinate to)
    :
    _from{from},
    _to{to}
{

}

CAtomicMove::CAtomicMove()
    :
    _from{},
    _to{}
{

}

CAtomicMove::~CAtomicMove()
{

}

CAtomicMove * const CAtomicMove::setFrom(CCoordinate from)
{
    _from = from;
    return this;
}

CAtomicMove * const CAtomicMove::setTo(CCoordinate to)
{
    _to = to;
    return this;
}

CCoordinate CAtomicMove::getFrom() const
{
    return _from;
}

CCoordinate CAtomicMove::getTo() const
{
    return _to;
}

CMove::CMove(CAtomicMove move)
    :
    _move{move},
    _sideEffect{},
    _hasSideEffect{false}
{

}

CMove::~CMove()
{

}

void CMove::setMove(CAtomicMove move)
{
    _move = move;
}

void CMove::setSideEffect(CAtomicMove sideEffectMove)
{
    _sideEffect = sideEffectMove;
}

CAtomicMove CMove::getMove() const
{
    return _move;
}

CAtomicMove CMove::getSideEffect() const
{
    return _sideEffect;
}

bool CMove::hasSideEffect() const
{
    return _hasSideEffect;
}

CMoveBranch::CMoveBranch(std::initializer_list<CMove> moves)
    :
    _moves{moves}
{

}

CMoveBranch::CMoveBranch()
{

}

CMoveBranch::~CMoveBranch()
{

}

unsigned int CMoveBranch::numberOfMoves() const
{
    return _moves.size();
}

void CMoveBranch::append(CMove move)
{
    _moves.push_back(move);
}

CMove CMoveBranch::operator[](unsigned int i) const
{
    return get(i);
}

CMove CMoveBranch::get(unsigned int i) const
{
    return _moves[i];
}

void CMoveBranch::append(std::initializer_list<CMove> moves)
{
    for (auto move: moves)
        append(move);
}

void CMoveBranch::remove(unsigned int at)
{
    _moves.erase(_moves.begin() + at);
}

void CMoveBranch::removeFrom(unsigned int at)
{
    _moves.erase(_moves.begin() + at, _moves.end());
}
/*
CPath::CPath(std::initializer_list<CMoveBranch> branchs)
    :
    _branchs{branchs}
{

}

CPath::CPath()
{

}

CPath::~CPath()
{

}

void CPath::append(CMoveBranch branch)
{
    _branchs.push_back(branch);
}

void CPath::remove(unsigned int at)
{
    _branchs.erase(_branchs.begin() + at);
}

unsigned int CPath::numberOfBranchs() const
{
    return _branchs.size();
}

std::vector<CMoveBranch> CPath::getBranchs() const
{
    return _branchs;
}
 */
}