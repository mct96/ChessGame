#include "../../include/view/board.hpp"

namespace ch {

CBoardView::CBoardView(std::string pieceTexFile)
    :
    _piecesTex{},
    _pieceTexFile{pieceTexFile}
{
    init();
}

CBoardView::~CBoardView()
{
}


void CBoardView::init()
{
    _piecesTex.loadFromFile(_pieceTexFile);
    loadPieces();

    drawBoard();
    drawPieces();
}

void CBoardView::loadPieces()
{

}

void CBoardView::onClick(sf::Event event)
{

}

void CBoardView::drawBoard()
{

}

void CBoardView::drawPieces()
{

}

void CBoardView::showPossibleMoves()
{

}

}