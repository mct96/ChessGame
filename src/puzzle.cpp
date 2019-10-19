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
    return false;
}

bool CPuzzle::isCheckMate() const
{
    return false;
}

void CPuzzle::
    castling(std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves)
{
}

void CPuzzle::
    enPassant(std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves)
{
    // TODO implementar o histórico de jogadas para detectar se o enPassant é
    // válido.
}

void CPuzzle::
    promotion(std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves)
{
}

EColor CPuzzle::getPlayerTurn() const
{
    return _playerTurn;
}

void CPuzzle::move(CPosition from, CPosition to)
{
    auto piece = getPieceAt(from);

    if (piece == nullptr || piece->getColor() != _playerTurn)
        throw new std::logic_error{"Invalid move."};

    auto moves = piece->getAllMoves();
    moves = movesPruning(moves);

    exceptionalMoves(piece, moves);

    // Verifica se a posição de destino é válida.
    if (inPath(moves, to)) {
        auto target = getPieceAt(to);

        // Remove a peça que estava na posição que ele ocupou agora.
        if (!isFreePosition(to)) removePieceAt(to);
    } else {
        throw new std::logic_error{"Invalid move."};
    }

    swapPlayerTurn();
}

std::shared_ptr<CPiece> CPuzzle::getPieceAt(CPosition pos) const
{
    auto searchOn =
        [&](std::vector<std::shared_ptr<CPiece>> pieces) ->
            std::shared_ptr<CPiece> {

            for (auto& piece: pieces) {
                auto position = piece->getPosition();

                if (pos == position)
                    return std::shared_ptr<CPiece>{piece};
            }

            return std::shared_ptr<CPiece>{nullptr};
        };

    auto piece = searchOn(_whitePieces);

    if (piece == nullptr)
        piece = searchOn(_blackPieces);

    return piece;

}

bool CPuzzle::isFreePosition(CPosition pos) const
{
    auto piece = getPieceAt(pos);

    return piece == nullptr;
}

std::vector<CPiece::CPath> CPuzzle::
    movesPruning(std::vector<CPiece::CPath> moves) const
{
    for (auto& move: moves) {
        for (int i=0;i<move.size(); ++i) {
            auto& pos = move[i];

            if (!isFreePosition(pos)) {
                auto piece = getPieceAt(pos);

                // Se a posição estiver ocupada tem de haver a poda, porém se a
                // posição for ocupada pelo adversário ela é incluida, caso
                // contrário ela deve ser eliminada.
                int extendPosition = piece->getColor() == _playerTurn ? 0 : 1;
                move.erase(move.begin() + i + extendPosition, move.end());
                break;
            }
        }
    }

    return moves;
}


void CPuzzle::removePieceAt(CPosition pos)
{
    auto findAndRemovePiece =
        [&](std::vector<std::shared_ptr<CPiece>>& pieces) -> void {

        for (int i = 0; i < pieces.size(); ++i) {
            if (pieces[i]->getPosition() == pos) {
                auto bg = pieces.begin();
                pieces.erase(bg + i, bg + i + 1);
                break;
            }
        }
    };

    findAndRemovePiece(_blackPieces);
    findAndRemovePiece(_whitePieces);
}

void CPuzzle::exceptionalMoves(
            std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves)
{
    // TODO Tratar todos os casos especiais aqui.
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