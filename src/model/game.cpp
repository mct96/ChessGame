#include "../../include/model/game.hpp"
#include <iostream>

using namespace std;
namespace ch {

CGame::CGame()
    :
    _playerTurn{EColor::WHITE},
    _boardLocations{},
    _history{_boardLocations}
{
    initializeGame();
}

CGame::~CGame()
{

}

CLocation CGame::getPieceAt(CCoordinate pos) const
{
    cout << "CGame::getPieceAt" << endl;
    return _boardLocations[pos.i][pos.j];
}

bool CGame::isCheck() const
{
    cout << "CGame::isCheck" << endl;
    return false;
}

bool CGame::isCheckMate() const
{
    cout << "CGame::isCheckMate" << endl;
    return false;
}

EColor CGame::getPlayerTurn() const
{
    cout << "CGame::getPlayerTurn" << endl;
    return _playerTurn;
}

void CGame::move(CCoordinate from, CCoordinate to)
{
    cout << "CGame::move" << endl;

    auto location = _boardLocations[from.i][from.j];
    if (location.isEmpty())
        throw new std::logic_error{
            std::string{"Invalid move. This position doesn't has a piece."}};

    if (location.getColor() != _playerTurn)
        throw new std::logic_error{
            std::string{"Invalid move. Not your turn."}};

    auto piece = factoryPiece(location.getType(), location.getColor());
    piece->setPosition(from);

    auto moves = piece->getPossibleMoves(_boardLocations, _history);

    // Verifica se a posição de destino é válida.
    CMove move{};
    if (inPath(moves, to, &move)) {
        auto& destination = _boardLocations[to.i][to.j];

        auto oldLocation = CLocation{
            destination.getType(), destination.getColor()};
        auto newLocation = CLocation{piece->getType(), piece->getColor()};

        _history.append(nullptr, from, to, oldLocation, newLocation);

        _boardLocations[from.i][from.j].removePiece();

        destination.setPiece(newLocation.getType(), newLocation.getColor());

        if (move.hasSideEffect()) {
            auto sideEffect = move.getSideEffect();
            if (sideEffect.getFrom() == sideEffect.getTo()) {
                auto pos = sideEffect.getFrom();
                _boardLocations[pos.i][pos.j].removePiece();
            }
        }
    } else {
        throw new std::logic_error{
            "Invalid move. The destination is unreachable."};
    }

    swapPlayerTurn();
}

CPiece *const CGame::factoryPiece(
        EType type,
        EColor color
    ) const
{
    cout << "CGame::factoryPiece" << endl;
    CPiece * piece = nullptr;

    switch (type) {
    case EType::ROOK:
        piece = &CGame::_rook;
        break;
    case EType::KNIGHT:
        piece = &CGame::_knight;
        break;
    case EType::BISHOP:
        piece = &CGame::_bishop;
        break;
    case EType::KING:
        piece = &CGame::_king;
        break;
    case EType::QUEEN:
        piece = &CGame::_queen;
        break;
    case EType::PAWN:
        piece = &CGame::_pawn;
        break;
    default:
        throw std::invalid_argument{"Invalid type."};
    }

    if (color == EColor::NULL_COLOR)
        throw std::invalid_argument{"Invalid color"};

    piece->setColor(color);
    piece->setType(type);

    return piece;
}

void CGame::
    promotion(std::shared_ptr<CPiece> piece, CMoveTree& moves)
{
    cout << "CGame::promotion" << endl;
}

void CGame::initializeGame()
{
    cout << "CGame::initializeGame" << endl;
    initializeBlackPieces();
    initializeWhitePieces();
    _playerTurn = EColor::WHITE;
}

void CGame::initializeWhitePieces()
{
    cout << "CGame::initializeWhitePieces" << endl;
    _boardLocations[0][0].setPiece(EType::ROOK,   EColor::WHITE);
    _boardLocations[0][1].setPiece(EType::KNIGHT, EColor::WHITE);
    _boardLocations[0][2].setPiece(EType::BISHOP, EColor::WHITE);
    _boardLocations[0][3].setPiece(EType::QUEEN,  EColor::WHITE);
    _boardLocations[0][4].setPiece(EType::KING,   EColor::WHITE);
    _boardLocations[0][5].setPiece(EType::BISHOP, EColor::WHITE);
    _boardLocations[0][6].setPiece(EType::KNIGHT, EColor::WHITE);
    _boardLocations[0][7].setPiece(EType::ROOK,   EColor::WHITE);

    for (int j = 0; j < 8; ++j)
        _boardLocations[1][j].setPiece(EType::PAWN,   EColor::WHITE);
}

void CGame::initializeBlackPieces()
{
    cout << "CGame::initializeBlackPieces" << endl;
    _boardLocations[7][0].setPiece(EType::ROOK,   EColor::BLACK);
    _boardLocations[7][1].setPiece(EType::KNIGHT, EColor::BLACK);
    _boardLocations[7][2].setPiece(EType::BISHOP, EColor::BLACK);
    _boardLocations[7][3].setPiece(EType::KING,   EColor::BLACK);
    _boardLocations[7][4].setPiece(EType::QUEEN,  EColor::BLACK);
    _boardLocations[7][5].setPiece(EType::BISHOP, EColor::BLACK);
    _boardLocations[7][6].setPiece(EType::KNIGHT, EColor::BLACK);
    _boardLocations[7][7].setPiece(EType::ROOK,   EColor::BLACK);

    for (int j = 0; j < 8; ++j)
        _boardLocations[6][j].setPiece(EType::PAWN,   EColor::BLACK);
}

void CGame::swapPlayerTurn()
{
    cout << "CGame::swapPlayerTurn" << endl;

    if (_playerTurn == EColor::WHITE)
        _playerTurn = EColor::BLACK;
    else
        _playerTurn = EColor::WHITE;
}

bool CGame::canMoveTo(std::shared_ptr<CPiece> piece, CCoordinate to) const
{
    cout << "CGame::canMoveTo" << endl;

    const auto from = piece->getPosition();
    if (auto location = _boardLocations[from.i][from.j]; location.isEmpty()) {

    }

    return false;
}

std::shared_ptr<CPiece> CGame::getKing(EColor kingColor) const
{
    cout << "CGame::getKing" << endl;
    return nullptr;
}

CMoveTree CGame::possibleMoves(
        CCoordinate pos) const
{
    cout << "CGame::possibleMoves" << endl;

    auto loc = _boardLocations[pos.i][pos.j];

    if (loc.isEmpty()) return{};

    auto piece = factoryPiece(loc.getType(), loc.getColor());
    piece->setPosition(pos);

    return piece->getPossibleMoves(_boardLocations, _history);
}

CPawn CGame::_pawn{};
CRook CGame::_rook{};
CKnight CGame::_knight{};
CBishop CGame::_bishop{};
CQueen CGame::_queen{};
CKing CGame::_king{};

}