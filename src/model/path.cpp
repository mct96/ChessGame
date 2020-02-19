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
    _move{move}
{

}

CMove::~CMove()
{

}

void CMove::setMove(CAtomicMove move)
{
    _move = move;
}

CAtomicMove CMove::getMove() const
{
    return _move;
}

void CMove::doMove(CLocation(*board)[8])
{
    auto [targetI, targetJ] = _move.getTo();
    auto [sourceI, sourceJ] = _move.getFrom();

    // Salva o estado.
    _oldDestinyState = board[targetI][targetJ];

    // Faz o movimento.
    board[targetI][targetJ] = board[sourceI][sourceJ];
    board[sourceI][sourceJ] = CLocation::emptyLocation();

}

void CMove::undoMove(CLocation(*board)[8])
{
    auto [targetI, targetJ] = _move.getTo();
    auto [sourceI, sourceJ] = _move.getFrom();

    board[sourceI][sourceJ] = board[targetI][targetJ];
    board[targetI][targetJ] = _oldDestinyState;
}


CMoveEnPassant::CMoveEnPassant(CAtomicMove move, CCoordinate pawn)
    :
    CMove{move},
    _pawnAttackedCoord{pawn}
{

}

void CMoveEnPassant::doMove(CLocation(*board)[8])
{
    CMove::doMove(board);
    pawnToRemove(board);
}

void CMoveEnPassant::undoMove(CLocation(*board)[8])
{
    CMove::undoMove(board);
    restorePawn(board);
}

void CMoveEnPassant::pawnToRemove(CLocation(*board)[8])
{
    auto [posI, posJ] = _pawnAttackedCoord;

    _stateBefore = board[posI][posJ];

    board[posI][posJ] = CLocation::emptyLocation();
}

void CMoveEnPassant::restorePawn(CLocation(*board)[8])
{
    auto [posI, posJ] = _pawnAttackedCoord;

    board[posI][posJ] = _stateBefore;
}

CMoveCastling::CMoveCastling(CAtomicMove move, CAtomicMove rookCastling)
    :
    CMove{move},
    _rookCasting{rookCastling}
{

}

void CMoveCastling::doMove(CLocation(*board)[8])
{
    CMove::doMove(board);
    castling(board);
}

void CMoveCastling::undoMove(CLocation(*board)[8])
{
    CMove::undoMove(board);
    undoCastling(board);
}

void CMoveCastling::castling(CLocation(*board)[8])
{
    auto [targetI, targetJ] = _rookCasting.getTo();
    auto [sourceI, sourceJ] = _rookCasting.getFrom();

    board[targetI][targetJ] = board[sourceI][sourceJ];
    board[sourceI][sourceJ] = CLocation::emptyLocation();
}

void CMoveCastling::undoCastling(CLocation(*board)[8])
{
    auto [targetI, targetJ] = _rookCasting.getTo();
    auto [sourceI, sourceJ] = _rookCasting.getFrom();

    board[sourceI][sourceJ] = board[targetI][targetJ];
    board[targetI][targetJ] = CLocation::emptyLocation();
}

CMoveBranch::CMoveBranch(std::initializer_list<CMove> moves)
{
    for (auto&& move: moves)
        append(move);
}

CMoveBranch::CMoveBranch(std::initializer_list<CMoveEnPassant> moves)
{
    for (auto&& move: moves)
        append(move);
}

CMoveBranch::CMoveBranch(std::initializer_list<CMoveCastling> moves)
{
    for (auto&& move: moves)
        append(move);
}

CMoveBranch::CMoveBranch()
{

}

CMoveBranch::~CMoveBranch()
{
    _moves.clear();
}

CMoveBranch::CIteratorType CMoveBranch::begin()
{
    return CIteratorType{_moves};
}

CMoveBranch::CIteratorType CMoveBranch::end()
{
    return CIteratorType{_moves, _moves.size()};
}

void CMoveBranch::pruning(unsigned int from)
{
    removeFrom(from);
}

unsigned int CMoveBranch::numberOfMoves() const
{
    return _moves.size();
}

void CMoveBranch::append(CMove move)
{
    _moves.push_back(new CMove{move});
}

void CMoveBranch::append(CMoveEnPassant move)
{
    _moves.push_back(new CMoveEnPassant{move});
}

void CMoveBranch::append(CMoveCastling move)
{
    _moves.push_back(new CMoveCastling{move});
}

CMove& CMoveBranch::operator[](unsigned int i) const
{
    return get(i);
}

CMove& CMoveBranch::get(unsigned int i) const
{
    return *_moves[i];
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
    if (at < _moves.size())
        _moves.erase(_moves.begin() + at, _moves.end());
}

void CMoveBranch::removeLast()
{
    _moves.pop_back();
}

CMoveTree::CMoveTree(std::initializer_list<CMoveBranch> branchs)
    :
    _branchs{branchs}
{

}

CMoveTree::CMoveTree()
{

}

CMoveTree::~CMoveTree()
{

}

void CMoveTree::append(CMoveBranch branch)
{
    _branchs.push_back(branch);
}

void CMoveTree::remove(unsigned int at)
{
    _branchs.erase(_branchs.begin() + at);
}

unsigned int CMoveTree::numberOfBranchs() const
{
    return _branchs.size();
}

CMoveBranch& CMoveTree::getBranch(std::size_t i)
{
    return _branchs[i];
}

void CMoveTree::pruningBranchFrom(unsigned int branch, unsigned int from)
{
    cout << "CMoveTree::pruningBranchFrom" << endl;
    CMoveBranch newBranch = _branchs[branch];
    newBranch.removeFrom(from);

    remove(branch);

    _branchs.insert(_branchs.begin() + branch, newBranch);
}

}