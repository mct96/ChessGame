#include "../../include/model/game.hpp"
#include <iostream>

using namespace std;
namespace ch {

CGame::CGame()
    :
    _playerTurn{EColor::WHITE}
{
    initializeGame();
}

CGame::~CGame()
{

}

bool CGame::isCheck() const
{
    auto king = getKing(_playerTurn);

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto location = _boardLocations[i][j];
            auto piece = location.getPiece();

            if (location.isEmpty() || piece->getColor() == king->getColor())
                continue;

            if (canMoveTo(piece, king->getPosition())) {
                return true;
            }
        }
    }

    return false;
}

bool CGame::isCheckMate() const
{
    return false;
}

EColor CGame::getPlayerTurn() const
{
    return _playerTurn;
}

void CGame::move(CCoordinate from, CCoordinate to)
{
    cout << static_cast<string>(from) << " "
         << static_cast<string>(to) << endl;
    auto piece = getPieceAt(from);

    if (piece == nullptr)
        throw new std::logic_error{
            std::string{
                "Invalid move. This position doesn't has a piece."}};

    if (piece->getColor() != _playerTurn)
        throw new std::logic_error{
            std::string{
                "Invalid move. Not your turn."}};

    auto moves = piece->getAllMoves();
    moves = movesPruning(piece, moves);

    // Adiciona os movimentos de ataque no caso do peão.
    // Adiciona o roque no caso da torre.
    exceptionalMoves(piece, moves);

    // Verifica se a posição de destino é válida.
    if (inPath(moves, to)) {
        // Remove a peça que estava na posição que ele ocupou agora.
        auto target = getPieceAt(to);
        if (!isFreePosition(to)) removePieceAt(to);

        piece->moveTo(to);
        updatePosition(piece, from);
    } else {
        throw new std::logic_error{
            "Invalid move. The destination is unreachable."};
    }

    // Salva no histórico de jogadas.
    _history.push_back(std::make_pair(from, to));

    swapPlayerTurn();
}

std::shared_ptr<CPiece> CGame::getPieceAt(CCoordinate pos) const
{
    return this->_boardLocations[pos.i][pos.j].getPiece();
}

bool CGame::isFreePosition(CCoordinate pos) const
{
    return this->_boardLocations[pos.i][pos.j].isEmpty();
}

std::vector<CPiece::CPath> CGame::movesPruning(
    std::shared_ptr<CPiece> piece,
    std::vector<CPiece::CPath> moves) const
{
    auto color = piece->getColor();
    auto enemyColor = color == EColor::WHITE ? EColor::BLACK : EColor::WHITE;

    for (auto& move: moves) {
        for (int i = 0; i < move.size(); ++i) {
            auto& pos = move[i];

            if (!isFreePosition(pos)) {
                auto target = getPieceAt(pos);

                // Se a posição estiver ocupada tem de haver a poda, porém se a
                // posição for ocupada pelo adversário ela é incluida, caso
                // contrário ela deve ser eliminada.
                int extendPosition = target->getColor() == enemyColor ? 1 : 0;
                move.erase(move.begin() + i + extendPosition, move.end());
                break;
            }
        }
    }

    return moves;
}


void CGame::removePieceAt(CCoordinate pos)
{
    auto location = this->_boardLocations[pos.i][pos.j];

    if (!location.isEmpty())
        location.removePiece();
}

bool CGame::hasAnEnemyAt(CCoordinate pos) const
{
    auto location = this->_boardLocations[pos.i][pos.j];

    return (
        !location.isEmpty() &&
        location.getPiece()->getColor() != _playerTurn
    );
}

void CGame::pawnMove(
    std::shared_ptr<CPiece> piece,
    std::vector<CPiece::CPath>& moves) const
{
    // O peão possui comportamento anomalo. Ele não ataca as posições em que ele
    // move. Ele ataca nas diagonais em direção ao inimigo, move em direção ao
    // inimigo uma casa, pode salta um ou duas posição na partida. Esta função
    // trata esses três casos.

    moves = pawnPruningVerticalAttack(piece, moves);


    auto [i, j] = piece->getPosition();

    // Gera um caminho a partir de uma única posição.
    auto genPath = [](int i, int j) -> auto {
        CPiece::CPath path{};
        path.push_back({i, j});
        return path;
    };

    // Adiciona os movimentos de ataques para as peças brancas, caso haja algum.
    if (i + 1  < 8 && _playerTurn == EColor::WHITE) {
        if (j - 1 >= 0 && hasAnEnemyAt({i + 1, j - 1}))
            moves.push_back(genPath(i + 1, j - 1));

        if (j + 1 < 8 && hasAnEnemyAt({i + 1, j + 1}))
            moves.push_back(genPath(i + 1, j + 1));
    }

    // Adiciona os movimentos de ataques para as peças pretas, caso haja algum.
    else if (i - 1 >= 0 && _playerTurn == EColor::BLACK) {
        if (j - 1 >= 0 && hasAnEnemyAt({i - 1, j - 1}))
            moves.push_back(genPath(i - 1, j - 1));

        if (j + 1 < 8 && hasAnEnemyAt({i - 1, j + 1}))
            moves.push_back(genPath(i - 1, j + 1));
    }
}

std::vector<CPiece::CPath>& CGame::pawnPruningVerticalAttack(
    std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves) const
{
    // Impede que o peão ataque verticalmente.
    for (auto& path: moves) {
        for (int i = 0; i < path.size(); ++i) {
            if (hasAnEnemyAt(path[i])) {
                auto at = path.begin() + i;
                path.erase(at, path.end());
                break;
            }
        }
    }

    return moves;
}

void CGame::castling(
    std::shared_ptr<CPiece> piece,
    std::vector<CPiece::CPath>& moves) const
{
    // Necessário verificar se não houve movimentos nem da Torre e nem do Rei
    // e se a posições intermediarias não estão sobre ataque.
}

// TEST Testar ainda o enPassant
void CGame::enPassant(
    std::shared_ptr<CPiece> piece,
    std::vector<CPiece::CPath>& moves) const
{
    // Basta olhar no histórico de jogadas. Se o último movimento do adversário
    // for um peão que avançou duas posições adicione a posição intermediária,
    // ie, (endPos + begPos) / 2.
    auto inRange = [](int v, int low, int high) -> bool {
        return v >= low || v <= high;
    };

    if (_history.empty()) return;

    auto lastMove = _history.back();
    auto lastPieceMoved = getPieceAt(lastMove.second);

    // Se não for peão, não há en Passant.
    if (lastPieceMoved->getType() != EType::PAWN ||
        piece->getType() != EType::PAWN)
            return;

    // if (!dynamic_cast<ch::CPawn*>(lastPieceMoved.get())) return;
    // if (!dynamic_cast<ch::CPawn*>(piece.get())) return;

    // A posição inicial do último movimento.
    auto [iOld, jOld] = lastMove.first;

    // A posição final do último movimento.
    auto [iNew, jNew] = lastMove.second;

    const auto& i = piece->getPosition().i;
    const auto& j = piece->getPosition().j;

    // Se o peão avançou duas posições e está a uma coluna de distância da peça
    // selecionada, porém não na mesma coluna e após o último movimento ambas as
    // peças acabarem na mesma posição, então todas as codições do en Passant
    // são satizfeitas.
    if (abs(iOld - iNew) == 2 &&
        inRange(i, jNew - 1, jNew + 1) &&
        j != jNew && jNew == jOld && i == iNew) {
            ch::CPiece::CPath move;
            int newI = static_cast<int>((iNew + iOld) / 2);
            move.push_back({newI, jNew});

            moves.push_back(move);
        }
}

void CGame::
    promotion(std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves)
{

}

void CGame::exceptionalMoves(
            std::shared_ptr<CPiece> piece, std::vector<CPiece::CPath>& moves) const
{
    if (piece == nullptr) return;

    switch (piece->getType()) {
    case EType::PAWN:
        pawnMove(piece, moves);  // Movimento de ataque do peão.
        enPassant(piece, moves); // Adiciona o enPassant do peão.
        break;

    case EType::ROOK:
        castling(piece, moves);  // Adiciona o Roque.
        break;

    default:
        ;
    }

}

void CGame::initializeGame()
{
    initializeBlackPieces();
    initializeWhitePieces();
    _playerTurn = EColor::WHITE;
}

void CGame::initializeWhitePieces()
{
    // Peças Brancas.
    auto pos1w = CCoordinate{0, 0};
    _boardLocations[0][0].setPiece(
        std::shared_ptr<CPiece>{new CRook{pos1w, EColor::WHITE}});

    auto pos2w = CCoordinate{0, 7};
    _boardLocations[0][7].setPiece(
        std::shared_ptr<CPiece>{new CRook{ pos2w, EColor::WHITE}});

    auto pos3w = CCoordinate{0, 1};
    _boardLocations[0][1].setPiece(
        std::shared_ptr<CPiece>{new CKnight{ pos3w, EColor::WHITE}});

    auto pos4w = CCoordinate{0, 6};
    _boardLocations[0][6].setPiece(
        std::shared_ptr<CPiece>{new CKnight{ pos4w, EColor::WHITE}});

    auto pos5w = CCoordinate{0, 2};
    _boardLocations[0][2].setPiece(
        std::shared_ptr<CPiece>{new CBishop{ pos5w, EColor::WHITE}});

    auto pos6w = CCoordinate{0, 5};
    _boardLocations[0][5].setPiece(
        std::shared_ptr<CPiece>{new CBishop{ pos6w, EColor::WHITE}});

    auto pos7w = CCoordinate{0, 3};
    _boardLocations[0][3].setPiece(
        std::shared_ptr<CPiece>{new CQueen{ pos7w, EColor::WHITE}});

    auto pos8w = CCoordinate{0, 4};
    _boardLocations[0][4].setPiece(
        std::shared_ptr<CPiece>{new CKing{ pos8w, EColor::WHITE}});

    for (int j = 0; j < 8; ++j) {
        auto posxw = CCoordinate{1, j};
        _boardLocations[1][j].setPiece(
            std::shared_ptr<CPiece>{new CPawn{posxw, EColor::WHITE}});
    }
}

void CGame::initializeBlackPieces()
{
     // Peças Pretas.
    auto pos1b = CCoordinate{7, 0};
    _boardLocations[7][0].setPiece(
        std::shared_ptr<CPiece>{new CRook{pos1b, EColor::BLACK}});

    auto pos2b = CCoordinate{7, 7};
    _boardLocations[7][7].setPiece(
        std::shared_ptr<CPiece>{new CRook{pos2b, EColor::BLACK}});

    auto pos3b = CCoordinate{7, 1};
    _boardLocations[7][1].setPiece(
        std::shared_ptr<CPiece>{new CKnight{pos3b, EColor::BLACK}});

    auto pos4b = CCoordinate{7, 6};
    _boardLocations[7][6].setPiece(
        std::shared_ptr<CPiece>{new CKnight{pos4b, EColor::BLACK}});

    auto pos5b = CCoordinate{7, 2};
    _boardLocations[7][2].setPiece(
        std::shared_ptr<CPiece>{new CBishop{pos5b, EColor::BLACK}});

    auto pos6b = CCoordinate{7, 5};
    _boardLocations[7][5].setPiece(
        std::shared_ptr<CPiece>{new CBishop{pos6b, EColor::BLACK}});

    auto pos7b = CCoordinate{7, 3};
    _boardLocations[7][3].setPiece(
        std::shared_ptr<CPiece>{new CQueen{pos7b, EColor::BLACK}});

    auto pos8b = CCoordinate{7, 4};
    _boardLocations[7][4].setPiece(
        std::shared_ptr<CPiece>{new CKing{pos8b, EColor::BLACK}});

    for (int j = 0; j < 8; ++j) {
        CCoordinate posxb = CCoordinate{6, j};
        _boardLocations[6][j].setPiece(
            std::shared_ptr<CPiece>{new CPawn{posxb, EColor::BLACK}});
    }
}

void CGame::swapPlayerTurn()
{
    if (_playerTurn == EColor::WHITE)
        _playerTurn = EColor::BLACK;
    else
        _playerTurn = EColor::WHITE;
}

bool CGame::canMoveTo(std::shared_ptr<CPiece> piece, CCoordinate to) const
{
    auto moves = piece->getAllMoves();
    moves = movesPruning(piece, moves);
    exceptionalMoves(piece, moves);

    return inPath(moves, to);
}

std::shared_ptr<CPiece> CGame::getKing(EColor kingColor) const
{
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto location = _boardLocations[i][j];
            auto piece = location.getPiece();

            if (location.isEmpty())
                continue;

            if (piece->getColor() == kingColor &&
                piece->getType() == EType::KING)
                return location.getPiece();
        }
    }

    return nullptr;
}

void CGame::print()
{
    // Imprime o estado de todas as peças do jogo. Um snapshot do tabuleiro.
    using namespace std;

    int k = 1;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            auto location = _boardLocations[i][j];
            auto piece = location.getPiece();

            if (location.isEmpty()) continue;

            auto pos = static_cast<std::string>(piece->getPosition());
            cout << setw(2) << k++ << " - ";
            cout << setw(6) <<
                "Color: " << colorToString(piece->getColor());
            cout << setw(5) <<
                "| Pos: " << pos;
            cout << setw(8) <<
                "| Type: " << setw(8) << typeToString(piece->getType());
            cout << setw(5) << boolalpha <<
                "| Active: " << piece->isActive() << endl;
        }
    }
}

int CGame::posToInt(CCoordinate pos) const
{
    // Transforma a posição (2D) em um valor inteiro (1D).
    return pos.i * 8 + pos.j;
}

void CGame::updatePosition(
    std::shared_ptr<CPiece> piece,
    CCoordinate oldPosition)
{
    auto newPosition = piece->getPosition();

    _boardLocations[oldPosition.i][oldPosition.j].removePiece();
    _boardLocations[newPosition.i][newPosition.j].setPiece(piece);
}

}