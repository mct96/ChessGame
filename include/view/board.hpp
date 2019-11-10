#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics.hpp>


namespace ch {

class CBoardView: public sf::View {
public:
    CBoardView(std::string pieceTexFile);
    ~CBoardView();

    // Inicializa o jogo.
    void init();

    // Carrega as imagens das peças.
    void loadPieces();

    // Trata eventos como cliques do mouse.
    void onClick(sf::Event mousePosition);

    // Desenha o tabuleiro.
    void drawBoard();

    // Desenha as peças.
    void drawPieces();

    // Mostra os possiveis movimentos de uma peça, quando alguma for seleciona-
    // da.
    void showPossibleMoves();

protected:
    // Armazena a textura que contém todas as peças.
    sf::Texture _piecesTex;

    // Contém cada peça individualmente. A mesma peça pode ser utilizada diver-
    // sas vezes, por exemplo, quando ocorre a promoção o peão pode se tornar
    // qualquer peça.
    std::vector<sf::Sprite> _pieces;

private:
    std::string _pieceTexFile;

};

}