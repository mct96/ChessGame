#include "../include/puzzle.hpp"

namespace ch {

CPuzzle::CPuzzle()
    :
    _playerTurn{EColor::WHITE},
    _whitePieces{},
    _blackPieces{}
{
    initializeGame();
}

CPuzzle::~CPuzzle()
{

}

bool CPuzzle::isCheck() const
{

}

bool CPuzzle::isCheckMate() const
{

}

void CPuzzle::castling()
{

}

void CPuzzle::enPassant()
{

}

void CPuzzle::promotion()
{

}

EColor CPuzzle::getPlayerTurn() const
{
    return _playerTurn;
}

void CPuzzle::move(CPosition from, CPosition to)
{
    auto piece = getPieceAt(from);

    if (piece.get() == nullptr)
        throw new std::logic_error{"Invalid move."};



    swapPlayerTurn();
}

std::shared_ptr<CPiece> CPuzzle::getPieceAt(CPosition pos) const
{
    auto verify =
        [&](std::vector<std::shared_ptr<CPiece>> pieces) ->
            std::shared_ptr<CPiece> {
            for (auto& piece: _whitePieces) {
                auto position = piece->getPosition();
                if (pos.i == position.j && pos.j == position.j)
                    return std::shared_ptr<CPiece>{piece};
            }
            return std::shared_ptr<CPiece>{};
        };

    auto piece = verify(_whitePieces);

    if (piece.use_count() == 0)
        piece = verify(_blackPieces);

    return piece;

}

void CPuzzle::initializeGame()
{
    initializeBlackPieces();
    initializeWhitePieces();
    _playerTurn = EColor::WHITE;
}

void CPuzzle::initializeWhitePieces()
{
    // Peças Brancas.
    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CRook{{1, 1}, EColor::WHITE}});

    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CRook{{1, 8}, EColor::WHITE}});

    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CKnight{{1, 2}, EColor::WHITE}});

    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CKnight{{1, 7}, EColor::WHITE}});

    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CBishop{{1, 3}, EColor::WHITE}});

    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CBishop{{1, 6}, EColor::WHITE}});

    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CQueen{{1, 4}, EColor::WHITE}});

    _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CKing{{1, 5}, EColor::WHITE}});

    for (int j = 1; j <= 8; ++j)
        _whitePieces.push_back(
        std::shared_ptr<CPiece>{new CPawn{{2, j}, EColor::WHITE}});
}

void CPuzzle::initializeBlackPieces()
{
     // Peças Pretas.
    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CRook{{8, 1}, EColor::BLACK}});

    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CRook{{8, 8}, EColor::BLACK}});

    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CKnight{{8, 2}, EColor::BLACK}});

    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CKnight{{8, 7}, EColor::BLACK}});

    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CBishop{{8, 3}, EColor::BLACK}});

    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CBishop{{8, 6}, EColor::BLACK}});

    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CQueen{{8, 4}, EColor::BLACK}});

    _blackPieces.push_back(
        std::shared_ptr<CPiece>{new CKing{{8, 5}, EColor::BLACK}});

    for (int j = 1; j <= 8; ++j)
        _blackPieces.push_back(
            std::shared_ptr<CPiece>{new CPawn{{7, j}, EColor::BLACK}});
}

void CPuzzle::swapPlayerTurn()
{
    if (_playerTurn == EColor::WHITE)
        _playerTurn = EColor::BLACK;
    else
        _playerTurn = EColor::WHITE;
}

void CPuzzle::print()
{
    using namespace std;

    for (const auto& ptr: _whitePieces) {
        auto [i, j] = ptr->getPosition();
        cout << "w[" << i << ", " << j << "]" << endl;
    }
    for (const auto& ptr: _blackPieces) {
        auto [i, j] = ptr->getPosition();
        cout << "b[" << i << ", " << j << "]" << endl;
    }
}

}