#include "../../include/view/board.hpp"

namespace ch {
std::size_t CBoardView::key_hash::operator() (const TypeColor& tp) const{
    return (static_cast<int>(std::get<0>(tp)) *
        static_cast<int>(std::get<1>(tp)));
}

CBoardView::CBoardView(
    sf::FloatRect screenDimensions,
    std::string pieceTexFile,
    std::string boardTexFile)
    :
    _screenDimensions{screenDimensions},
    _piecesTex{},
    _boardTex{},
    _pieceTexFile{pieceTexFile},
    _boardTexFile{boardTexFile},
    _selectedPiece{0, 0},
    _gameView{_screenDimensions},
    _game{}
{
    init();
    // _gameView.setCenter(800, 800);
}

CBoardView::~CBoardView()
{
}


void CBoardView::init()
{
    // Carrega as texturas.
    _piecesTex.loadFromFile(_pieceTexFile);
    _boardTex.loadFromFile(_boardTexFile);

    // Cria objetos do tipo Sprite.
    loadBoardPositionColor();
    loadPieces();

    // Escalas os Sprites para a dimensão específicada pelo usuário.
    scaleSprites();
}

void CBoardView::draw(sf::RenderTarget& target, sf::RenderStates state) const
{
    drawBoard(target);
    drawSelectedPiece(target);
    drawPieces(target);
}

sf::Vector2f CBoardView::coordToBoardLocation(int i, int j) const
{
    if (i <= 0 || i > 8 || j <= 0 || j > 8)
        throw std::invalid_argument{
            "The board position is from 1-8 in both directions."};

    auto& screenSize = _gameView.getSize();

    auto pieceWidth = (screenSize.x / 8);
    auto pieceHeight = (screenSize.y / 8);

    auto x = (j - 1) * pieceWidth;
    auto y = (8 - i) * pieceHeight;

    return {x, y};
}

void CBoardView::loadPieces()
{
    auto whiteKing   = sf::Sprite{_piecesTex, {{0 * 200, 0}, {200, 200}}};
    auto whiteQueen  = sf::Sprite{_piecesTex, {{1 * 200, 0}, {200, 200}}};
    auto whiteBishop = sf::Sprite{_piecesTex, {{2 * 200, 0}, {200, 200}}};
    auto whiteKnight = sf::Sprite{_piecesTex, {{3 * 200, 0}, {200, 200}}};
    auto whiteRook   = sf::Sprite{_piecesTex, {{4 * 200, 0}, {200, 200}}};
    auto whitePawn   = sf::Sprite{_piecesTex, {{5 * 200, 0}, {200, 200}}};

    auto blackKing   = sf::Sprite{_piecesTex, {{0 * 200, 200}, {200, 200}}};
    auto blackQueen  = sf::Sprite{_piecesTex, {{1 * 200, 200}, {200, 200}}};
    auto blackBishop = sf::Sprite{_piecesTex, {{2 * 200, 200}, {200, 200}}};
    auto blackKnight = sf::Sprite{_piecesTex, {{3 * 200, 200}, {200, 200}}};
    auto blackRook   = sf::Sprite{_piecesTex, {{4 * 200, 200}, {200, 200}}};
    auto blackPawn   = sf::Sprite{_piecesTex, {{5 * 200, 200}, {200, 200}}};

    _pieces[std::make_tuple(EType::KING,   EColor::WHITE)] = whiteKing;
    _pieces[std::make_tuple(EType::QUEEN,  EColor::WHITE)] = whiteQueen;
    _pieces[std::make_tuple(EType::BISHOP, EColor::WHITE)] = whiteBishop;
    _pieces[std::make_tuple(EType::KNIGHT, EColor::WHITE)] = whiteKnight;
    _pieces[std::make_tuple(EType::ROOK,   EColor::WHITE)] = whiteRook;
    _pieces[std::make_tuple(EType::PAWN,   EColor::WHITE)] = whitePawn;

    _pieces[std::make_tuple(EType::KING,   EColor::BLACK)] = blackKing;
    _pieces[std::make_tuple(EType::QUEEN,  EColor::BLACK)] = blackQueen;
    _pieces[std::make_tuple(EType::BISHOP, EColor::BLACK)] = blackBishop;
    _pieces[std::make_tuple(EType::KNIGHT, EColor::BLACK)] = blackKnight;
    _pieces[std::make_tuple(EType::ROOK,   EColor::BLACK)] = blackRook;
    _pieces[std::make_tuple(EType::PAWN,   EColor::BLACK)] = blackPawn;

}

void CBoardView::loadBoardPositionColor()
{
    sf::Sprite whitePiece{_boardTex, {{0  , 0}, {200, 200}}};
    sf::Sprite blackPiece{_boardTex, {{200, 0}, {200, 200}}};

    _boardPositionColor[EColor::WHITE] = whitePiece;
    _boardPositionColor[EColor::BLACK] = blackPiece;
}


void CBoardView::scaleSprites()
{
    auto screenSize = _gameView.getSize();
    auto pieceDimentions = getPieceDimentions();

    auto xScaleFactor = pieceDimentions.x / 200;
    auto yScaleFactor = pieceDimentions.y / 200;

    // Escala os Sprites das peças.
    for (auto& piece: _pieces) {
        piece.second.scale({xScaleFactor, yScaleFactor});
    }

    // Escala a textura das posições do tabuleiro.
    for (auto& boardColor: _boardPositionColor) {
        boardColor.second.scale({xScaleFactor, yScaleFactor});
    }

}

sf::Vector2f CBoardView::getPieceDimentions() const
{
    auto boardSize = _gameView.getSize();

    return {boardSize.x/8, boardSize.y/8};
}

void CBoardView::drawSelectedPiece(sf::RenderTarget& target) const
{
    auto coord = sf::Vector2i{_selectedPiece.x, _selectedPiece.y};
    auto pieceDimention = getPieceDimentions();
    auto pos = target.mapPixelToCoords(coord);

    auto i = 7 - static_cast<int>(pos.y / pieceDimention.y);
    auto j = static_cast<int>(pos.x / pieceDimention.x);

    if (i >= 0 && i < 8 && j >= 0 && j < 8) {
        sf::RectangleShape highlight{pieceDimention};
        highlight.setFillColor(sf::Color{0, 255, 0, 50});
        highlight.setOutlineColor(sf::Color{0, 255, 0, 255});
        highlight.setOutlineThickness(-5.0f);
        highlight.setPosition(coordToBoardLocation(i + 1, j + 1));

        target.draw(highlight);
    }
}

void CBoardView::onClick(const sf::Event& event)
{
    _selectedPiece.x = event.mouseButton.x;
    _selectedPiece.y = event.mouseButton.y;
}

void CBoardView::setBoardDimensions(sf::FloatRect screenDimensions)
{
    _screenDimensions = screenDimensions;
    _gameView.reset(_screenDimensions);
}

sf::FloatRect CBoardView::getBoardDimensions() const
{
    return _screenDimensions;
}

void CBoardView::drawBoard(sf::RenderTarget& target) const
{
    auto isOdd = [&](int n) -> bool { return n % 2 != 0; };
    auto isEven = [&](int n) -> bool { return !isOdd(n); };

    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {

        auto topLeftConer = coordToBoardLocation(i + 1, j + 1);

        auto color =
         ((isOdd(i) && isOdd(j)) || (isEven(i) && isEven(j))) ?
            EColor::BLACK : EColor::WHITE;

        auto colorToDraw = _boardPositionColor.at(color);
        colorToDraw.setPosition(topLeftConer);
        target.draw(colorToDraw);

    }
}

void CBoardView::drawPieces(sf::RenderTarget& target) const
{
    for (int i = 0; i < 8; ++i)
        for (int j = 0; j < 8; ++j) {
            auto piece = _game.getPieceAt({i + 1, j + 1});

            if (piece == nullptr) continue;

            sf::Vector2f pos{200.f  * j, 1600 - 200.f * (i + 1)};

            auto type = piece->getType();
            auto color = piece->getColor();

            auto pieceSprite =
                _pieces.at(std::make_tuple(type, color));

            pieceSprite.setPosition(coordToBoardLocation(i + 1, j + 1));
            target.draw(pieceSprite);
        }
}

void CBoardView::showPossibleMoves()
{

}

}