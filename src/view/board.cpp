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
    _selectedPoint{screenDimensions.left, screenDimensions.top},
    _selectedPiece{},
    _gameView{_screenDimensions},
    _gameController{}
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
    checkStateIndicator(target);
    drawSelectedPiece(target);
    showPossibleMoves(target);
    drawPieces(target);
}

sf::Vector2f CBoardView::coordToBoardLocation(int i, int j) const
{
    if (i < 0 || i > 7 || j < 0 || j > 7)
        throw std::invalid_argument{
            "The board position is from 1-8 in both directions."};

    auto& screenSize = _gameView.getSize();

    auto pieceWidth = (screenSize.x / 8);
    auto pieceHeight = (screenSize.y / 8);

    auto x = _screenDimensions.left  + j * pieceWidth;
    auto y = _screenDimensions.top + (7 - i) * pieceHeight;

    return {x, y};
}

ch::CCoordinate CBoardView::boardLocationToCoord(int x, int y) const
{
    cout << "CBoardView::boardLocationToCoord" << endl;

    auto pieceDimention = getPieceDimentions();

    auto i = 7 - static_cast<int>(
         (y - _screenDimensions.top) / pieceDimention.y);

    auto j = static_cast<int>(
        (x - _screenDimensions.left) / pieceDimention.x);

    return {i , j};
}

void CBoardView::loadPieces()
{
    cout << "CBoardView::loadPieces" << endl;

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
    cout << "CBoardView::loadBoardPositionColor" << endl;

    sf::Sprite whitePiece{_boardTex, {{0  , 0}, {200, 200}}};
    sf::Sprite blackPiece{_boardTex, {{200, 0}, {200, 200}}};

    _boardPositionColor[EColor::WHITE] = whitePiece;
    _boardPositionColor[EColor::BLACK] = blackPiece;
}


void CBoardView::scaleSprites()
{
    cout << "CBoardView::scaleSprites" << endl;

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
    cout << "CBoardView::getPieceDimentions" << endl;

    auto boardSize = _gameView.getSize();

    return {boardSize.x/8, boardSize.y/8};
}

void CBoardView::
    highlightSelectedPiece(ch::CCoordinate pos, sf::RenderTarget& target) const
{
    cout << "CBoardView::highlightSelectedPiece" << endl;

    sf::RectangleShape highlight{getPieceDimentions()};
    highlight.setFillColor(sf::Color{0, 255, 0, 50});
    highlight.setOutlineColor(sf::Color{0, 255, 0, 255});
    highlight.setOutlineThickness(-3.0f);
    highlight.setPosition(coordToBoardLocation(pos.i, pos.j));

    target.draw(highlight);
}

void CBoardView::highlightPossibleMoves(
    ch::CCoordinate pos,
    bool isOccupied,
    sf::RenderTarget& target) const
{
    cout << "CBoardView::highlightPossibleMoves" << endl;

    auto dim = getPieceDimentions();
    auto radius = 10.f;
    auto circleCenter = sf::Vector2f{dim.x/2, dim.y/2};

    // Desenha um círculo vermelho caso a posição desejada esteja ocupada, isto
    // é, um ataque e desenha um círculo amarelo caso contrário.
    sf::CircleShape highlight{radius};
    highlight.setOrigin({radius, radius});
    highlight.setFillColor({255, 255, 0, 100});
    highlight.setOutlineThickness(radius*0.4);

    highlight.setOutlineColor(
        isOccupied ? sf::Color::Red : sf::Color::Yellow);

    highlight.setPosition(
        circleCenter + coordToBoardLocation(pos.i, pos.j));

    target.draw(highlight);
}

void CBoardView::checkStateIndicator(sf::RenderTarget& target) const
{
    cout << "CBoardView::checkStateIndicator" << endl;
    auto isCheck = _gameController.isCheck();
    if (!isCheck) return;

    auto dim = _screenDimensions;
    sf::RectangleShape rect{{dim.width, dim.height}};
    rect.setPosition({dim.left, dim.top});
    rect.setFillColor({255, 255, 0, 50}); // Amarelo transparente.
    target.draw(rect);
}

void CBoardView::drawSelectedPiece(sf::RenderTarget& target) const
{
    cout << "CBoardView::drawSelectedPiece" << endl;

    if (!_selectedPiece.first.isEmpty())
        highlightSelectedPiece(_selectedPiece.second, target);
}

void CBoardView::onClick(const sf::Event& event)
{
    cout << "CBoardView::onClick" << endl;

    auto point = sf::Vector2f{event.mouseButton.x, event.mouseButton.y};

    // Verifica se a região selecionada está dentro da área do tabuleiro. Caso
    // contrário, não faz nada.
    if (!_screenDimensions.contains(point)) return;

    _selectedPoint.x = point.x;
    _selectedPoint.y = point.y;

    // Se alguma peça for selecionada, marque-a como peça selecionada.
    auto [i, j] = boardLocationToCoord(_selectedPoint.x, _selectedPoint.y);
    auto piece = _gameController.getPieceAt({i, j});


    if (!piece.isEmpty() &&
         piece.getColor() == _gameController.getPlayerTurn()) {
             _selectedPiece.first = piece;
             _selectedPiece.second = {i, j};
         }

    // Verifica se essa ação foi a de mover uma peça.
    handleMove();
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

        auto topLeftConer = coordToBoardLocation(i, j);

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
            auto piece = _gameController.getPieceAt({i, j});

            // Não há peça nesta posição.
            if (piece.isEmpty()) continue;

            sf::Vector2f pos{200.f  * j, 1600 - 200.f * (i + 1)};

            auto type = piece.getType();
            auto color = piece.getColor();

            auto pieceSprite =
                _pieces.at(std::make_tuple(type, color));

            pieceSprite.setPosition(coordToBoardLocation(i, j));
            target.draw(pieceSprite);
        }
}

void CBoardView::showPossibleMoves(sf::RenderTarget& target) const
{
    // Obtém as coordenadas do tabuleiro da posição atualmente selecionada no
    // view.
    auto [i, j] = boardLocationToCoord(_selectedPoint.x, _selectedPoint.y);

    // Obtém a peça selecionada, se houver.
    auto selectedPiece = _gameController.getPieceAt({i, j});

    // Caso não haja, retorne nulo.
    if (selectedPiece.isEmpty()) return;

    // Lista os possiveis movimentos da peça selecionada.
    auto tree = _gameController.possibleMoves(_selectedPiece.second);

    // Exibe todos esses movimentos.
    for (auto branch: tree.getBranchs()) {
        for (unsigned int i = 0; i < branch.numberOfMoves(); ++i) {
            auto pos = branch.get(i).getMove().getTo();
            auto isOccupied = !_gameController.getPieceAt(pos).isEmpty();
            highlightPossibleMoves({pos.i, pos.j}, isOccupied, target);
        }
    }
}

void CBoardView::handleMove()
{
    if (_selectedPiece.first.isEmpty()) return;

    auto [i, j] = boardLocationToCoord(_selectedPoint.x, _selectedPoint.y);

    if (auto to = CCoordinate{i, j}; _selectedPiece.second != to)
        move(_selectedPiece.second, to);
}

void CBoardView::move(CCoordinate from, CCoordinate to)
{

    try {
    _gameController.move(from, to);
    } catch(std::logic_error* msg){ cout << msg->what() << endl;}
}

}