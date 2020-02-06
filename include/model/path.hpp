#pragma once

#include <iostream>
#include <vector>

#include "coordinate.hpp"
#include "location.hpp"
#include "iterator.hpp"

using namespace std;

namespace ch {

/**
 * CAtomicMove é o elemento mínimo de um movimento. Significa mover UMA peça de
 * uma posição e colocá-lá em outra.
 */
class CAtomicMove
{
public:
    CAtomicMove(CCoordinate from, CCoordinate to);
    CAtomicMove();
    ~CAtomicMove();

    CAtomicMove * const setFrom(CCoordinate position);
    CAtomicMove * const setTo(CCoordinate location);

    CCoordinate getFrom() const;
    CCoordinate getTo() const;

private:
    CCoordinate _from;
    CCoordinate _to;
};

/**
 * Devido ao fato de que algumas jogadas não são atômicas (en passant e
 * castling), o movimento é representado como sendo constituído de até dois mo-
 * vimentos atômicos, o movimento desejado e o efeito colateral.
 */
class CMove
{
public:
    CMove(CAtomicMove move);
    CMove() = default;
    ~CMove();

    void setMove(CAtomicMove move);
    void setSideEffect(CAtomicMove sideEffectMove);

    CAtomicMove getMove() const;
    CAtomicMove getSideEffect() const;

    bool hasSideEffect() const;

private:
    CAtomicMove _move;
    CAtomicMove _sideEffect;

    bool _hasSideEffect;
};

/**
 * Os movimentos são gerados na perspectiva de branchs, pois um depende de um
 * prévio não ter sido selecionado.
 */
class CMoveBranch
{
public:
    template <typename T>using CContainer = std::vector<T>;
    using CIteratorType = CIterator<CMove, CContainer>;

    CMoveBranch(std::initializer_list<CMove> moves);
    CMoveBranch();
    ~CMoveBranch();

    CIteratorType begin();
    CIteratorType end();

    void pruning(unsigned int from);

    // Retorna a quantidade de movimentos dessa branch.
    unsigned int numberOfMoves() const;

    // Adiciona um movimento.
    void append(CMove move);

    CMove operator[](unsigned int i) const;
    CMove get(unsigned int i) const;

    // Adiciona uma sequência de movimentos.
    void append(std::initializer_list<CMove> moves);

    // Remove o elemento especificado.
    void remove(unsigned int at);
    void removeLast();

    // Remove todos elementos a partir da posição especificada.
    void removeFrom(unsigned int at);

private:
    std::vector<CMove> _moves;
};

class CMoveTree
{
public:
    template <typename T>using CContainer = std::vector<T>;
    using CIteratorType = CIterator<CMoveBranch, CContainer>;

    CMoveTree(std::initializer_list<CMoveBranch> branchs);
    CMoveTree();// = default;
    ~CMoveTree();

    void append(CMoveBranch branch);
    void remove(unsigned int at);

    unsigned int numberOfBranchs() const;

    CMoveBranch& getBranch(std::size_t i);

    CIteratorType begin() { return {_branchs}; }
    CIteratorType end() { return {_branchs, _branchs.size()}; }

    void pruningBranchFrom(unsigned int branch, unsigned int from);

private:
    std::vector<CMoveBranch> _branchs;
};

}