#include "game.hpp"

#include <utility>
#include <iterator>

namespace ch {

history_entry_t history_entry_t::
    make_simple_move(pos_t from, pos_t to, piece_t piece)
{
    return history_entry_t{type_t::simple, from, to, piece_t::empt, piece};
}

history_entry_t history_entry_t::
    make_attack_move(pos_t from, pos_t to, piece_t st_bef, piece_t st_aft)
{
    return history_entry_t{type_t::attack, from, to, st_bef, st_aft};
}

history_entry_t history_entry_t::
    make_en_passant_move(pos_t from, pos_t to)
{
    return history_entry_t{type_t::en_passant, from, to};
}

history_entry_t history_entry_t::
    make_castling_move(pos_t from, pos_t to)
{
    return history_entry_t{type_t::castling, from, to};
}

history_entry_t history_entry_t::
    make_promotion_move(pos_t from, pos_t to, piece_t st_bef, piece_t st_aft)
{
    return history_entry_t{type_t::castling, from, to, st_bef, st_aft};
}

history_entry_t::~history_entry_t()
{

}

history_entry_t::history_entry_t(
    type_t type, pos_t from, pos_t to, piece_t piece_bef, piece_t piece_aft)
    :
    _type{type},
    _state{piece_bef, piece_aft},
    _from{from}, _to{to}
{

}

game_t::game_t()
    :
    _history(10),
    _history_head{0},
    _board{},
    _w_pieces{},
    _b_pieces{}
{
    reset();
}

game_t::~game_t()
{
    // save some statistics or logs?
}

#ifndef NDEBUG

std::string game_t::piece_to_str(piece_t p) const
{
    using p_t = piece_t;
    switch (p) {
    case p_t::bp:
        return "♟";
    case p_t::br:
        return "♜";
    case p_t::bh:
        return "♞";
    case p_t::bb:
        return "♝";
    case p_t::bk:
        return "♚";
    case p_t::bq:
        return "♛";
    case p_t::wp:
        return "♙";
    case p_t::wr:
        return "♖";
    case p_t::wh:
        return "♘";
    case p_t::wb:
        return "♗";
    case p_t::wk:
        return "♔";
    case p_t::wq:
        return "♕";
    case p_t::empt:
        return " ";
    }
}

void game_t::print_board() const
{
    for (int i = 7; i >= 0; --i) {
        for (int j = 0; j < 8; ++j) {
            cout << piece_to_str(_board[i][j]) << " ";
        }
        cout << endl;
    }
}
#endif

std::size_t game_t::hash() const
{
    std::size_t result = 0;

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            result += static_cast<int>(_board[i][j]) ^ result;
        }
    }

    return result;
}

bool game_t::test_check_mate(color_t color) const
{
    // Emulates the move of every piece.
    struct temporary_move_t {
        temporary_move_t(const game_t* game_context, pos_t from, pos_t to)
            : _this{const_cast<game_t*>(game_context)}, _from{ from }, _to{ to }
        {
            _done = _this->move(_from, _to);
        }

        ~temporary_move_t() {
            if (_done) _this->undo();
        }

    private:
        game_t * _this;
        pos_t _from, _to;
        bool _done = false;
    };

    if (!test_check(color)) return false;
    auto pieces_position = color == color_t::w ? _w_pieces : _b_pieces;

    for (auto piece_pos: pieces_position) {
        auto moves = list_moves(piece_pos);

        for (auto move_pos: moves) {
            temporary_move_t tmove{this, piece_pos, move_pos};
            // print_board(); cout << "\n" << endl;
            if (!test_check(color))
                return false;
        }
    }

    return true;
}

bool game_t::test_check(color_t color) const
{
    auto king_pos = find_king(color);
    auto pieces = color == color_t::w ? _b_pieces : _w_pieces;
    auto b = pieces.begin(), e = pieces.end();

    return std::any_of(b, e,
        [=, this](pos_t p) { return can_move(p, king_pos); });
}

void game_t::reset()
{
    using p = piece_t;

    _board[0][0] = p::wr; _board[0][1] = p::wh; _board[0][2] = p::wb;
    _board[0][3] = p::wq; _board[0][4] = p::wk; _board[0][5] = p::wb;
    _board[0][6] = p::wh; _board[0][7] = p::wr;

    _board[7][0] = p::br; _board[7][1] = p::bh; _board[7][2] = p::bb;
    _board[7][3] = p::bq; _board[7][4] = p::bk; _board[7][5] = p::bb;
    _board[7][6] = p::bh; _board[7][7] = p::br;

    auto w_pawn_ptr = &_board[1][0];
    std::for_each(w_pawn_ptr, w_pawn_ptr + 8, [](piece_t& v){ v = p::wp; });

    auto b_pawn_ptr = &_board[6][0];
    std::for_each(b_pawn_ptr, b_pawn_ptr + 8, [](piece_t& v){ v = p::bp; });

    auto e_ptr = &_board[2][0];
    std::for_each(e_ptr, e_ptr + 8 * 4, [](piece_t& v) { v = p::empt; });

    reset_iterators();
}

void game_t::undo()
{
    if (_history_head == 0) return;

    history_entry_t past_move = pop_history();

    switch (past_move._type) {
    case history_entry_t::type_t::simple:
        undo_simple_move(past_move);
        break;
    case history_entry_t::type_t::attack:
        undo_attack_move(past_move);
        break;
    case history_entry_t::type_t::en_passant:
        undo_en_passant_move(past_move);
        break;
    case history_entry_t::type_t::castling:
        undo_castling_move(past_move);
        break;
    case history_entry_t::type_t::promotion:
        undo_promotion_move(past_move);
        break;
    }

}

void game_t::redo()
{
    if (_history_head >= _history.size()) return;

    history_entry_t new_move = _history[_history_head];

    switch (new_move._type) {
    case history_entry_t::type_t::simple:
        redo_simple_move(new_move);
        break;
    case history_entry_t::type_t::attack:
        redo_attack_move(new_move);
        break;
    case history_entry_t::type_t::en_passant:
        redo_en_passant_move(new_move);
        break;
    case history_entry_t::type_t::castling:
        redo_castling_move(new_move);
        break;
    case history_entry_t::type_t::promotion:
        redo_promotion_move(new_move);
        break;
    }

    ++_history_head;
}

bool game_t::move(pos_t from, pos_t to, bool commit)
{
    if (from == to
        || !from.check_bounds(0, 7, 0, 7)
        || !to.check_bounds(0, 7, 0, 7)
        || is_empty(from)
        || (!is_empty(to) && same_col(from, to) && !is_castling(from, to)))
        return false;

    auto piece = get_piece(from);

    switch (piece) {
    case piece_t::bp: case piece_t::wp:
        return move_pawn(from, to, commit);
        break;
    case piece_t::br: case piece_t::wr:
        return move_rook(from, to, commit);
        break;
    case piece_t::bh: case piece_t::wh:
        return move_knight(from, to, commit);
        break;
    case piece_t::bb: case piece_t::wb:
        return move_bishop(from, to, commit);
        break;
    case piece_t::bq: case piece_t::wq:
        return move_queen(from, to, commit);
        break;
    case piece_t::bk: case piece_t::wk:
        return move_king(from, to, commit);
        break;
    }

    return false;
}

bool game_t::can_move(pos_t from, pos_t to) const
{
    auto self = const_cast<game_t*>(this);
    return self->move(from, to, false);
}

game_t::list_pos_t game_t::list_moves(pos_t pos) const
{
    list_pos_t moves{};

    auto piece = get_piece(pos);
    if (piece == piece_t::empt) return moves;

    switch (piece) {
    case piece_t::wp: case piece_t::bp:
        return list_pawn_moves(pos);

    case piece_t::wr: case piece_t::br:
        return list_rook_moves(pos);

    case piece_t::wh: case piece_t::bh:
        return list_knight_moves(pos);

    case piece_t::wb: case piece_t::bb:
        return list_bishop_moves(pos);

    case piece_t::wq: case piece_t::bq:
        return list_queen_moves(pos);

    case piece_t::wk: case piece_t::bk:
        return list_king_moves(pos);
    }
}

game_t::list_pos_t game_t::can_move_to(pos_t to, color_t color) const
{
    const auto& container = color == color_t::w ? _w_pieces : _b_pieces;
    auto b = container.begin(), e = container.end();
    list_pos_t piece_positions{};
    std::back_insert_iterator<list_pos_t> insert_it{piece_positions};

    std::copy_if(b, e, insert_it,
        [=, this](pos_t pos) {
            auto v = this->can_move(pos, to);
            return v;});

    return piece_positions;
}

game_t::list_pos_t game_t::can_move_to(pos_t to, piece_t piece) const
{
    auto piece_pos = can_move_to(to, get_piece_color(piece));

    auto b = piece_pos.begin(), e = piece_pos.end();
    auto new_e = std::remove_if(b, e,
                [&, this](pos_t pos){ return this->get_piece(pos) != piece; });
    piece_pos.erase(new_e, piece_pos.end());

    return piece_pos;
}

void game_t::set_piece(pos_t pos, piece_t piece)
{
    _board[pos._i][pos._j] = piece;
}

piece_t game_t::get_piece(pos_t pos) const
{
    return _board[pos._i][pos._j];
}

game_t::list_pos_t game_t::list_king_moves(pos_t origin) const
{
    return list_moves(origin, 1, true, true);
}

game_t::list_pos_t game_t::list_queen_moves(pos_t origin) const
{
    return list_moves(origin, 7, true, true);
}

game_t::list_pos_t game_t::list_bishop_moves(pos_t origin) const
{
    return list_moves(origin, 7, false, true);
}

game_t::list_pos_t game_t::list_knight_moves(pos_t origin) const
{
    list_pos_t positions{};
    positions.push_back(origin.u(2).l());
    positions.push_back(origin.u(2).r());
    positions.push_back(origin.d(2).l());
    positions.push_back(origin.d(2).r());
    positions.push_back(origin.l(2).u());
    positions.push_back(origin.l(2).d());
    positions.push_back(origin.r(2).u());
    positions.push_back(origin.r(2).d());

    auto it = std::remove_if(
        positions.begin(), positions.end(), [&](pos_t pos) {
            return !can_move(origin, pos);
        });

    positions.erase(it, positions.end());

    return positions;
}

game_t::list_pos_t game_t::list_rook_moves(pos_t origin) const
{
    return list_moves(origin, 7, true, false);
}

game_t::list_pos_t game_t::list_pawn_moves(pos_t origin) const
{
    list_pos_t positions{};
    auto piece = get_piece(origin);
    auto piece_color = get_piece_color(piece);

    // A little ugly. en passant is treated implicitly.
    if (piece_color == color_t::w) {
        if (can_move(origin, origin.u())) positions.push_back(origin.u());
        if (can_move(origin, origin.u(2))) positions.push_back(origin.u(2));
        if (can_move(origin, origin.lu())) positions.push_back(origin.lu());
        if (can_move(origin, origin.ru())) positions.push_back(origin.ru());
    } else {
        if (can_move(origin, origin.d())) positions.push_back(origin.d());
        if (can_move(origin, origin.d(2))) positions.push_back(origin.d(2));
        if (can_move(origin, origin.ld())) positions.push_back(origin.ld());
        if (can_move(origin, origin.rd())) positions.push_back(origin.rd());
    }

    return positions;
}

game_t::list_pos_t game_t::list_moves(
    pos_t origin, std::size_t range, bool parallel, bool diagonal) const
{
    // HYPOTHESIS I used pointer to member to avoid 8 different loops.
    // WORKS!!!
    using func_ptr = pos_t(pos_t::*)(uint8_t) const;
    func_ptr d_moves[] = {
        &pos_t::ru, &pos_t::lu, &pos_t::ld, &pos_t::rd };

    func_ptr p_moves[] = {
        &pos_t::r, &pos_t::u, &pos_t::l, &pos_t::d};

    std::vector<func_ptr> directions{};

    if (parallel)
        directions.insert(directions.begin(), p_moves, p_moves + 4);

    if (diagonal)
        directions.insert(directions.begin(), d_moves, d_moves + 4);

    auto positions = list_pos_t{};
    for (auto direction: directions) {
        for (auto k = 1; k <= range; ++k) {
            auto to = (origin.*direction)(k);

            if (!to.check_bounds(0, 7, 0, 7))
                break;

            if (!is_empty(to))
                if (!same_col(origin, to)){
                    positions.push_back(to);
                    break;
                } else {
                    break;
                }


            positions.push_back(to);
        }
    }

    return positions;
}

void game_t::undo_simple_move(history_entry_t past_move)
{
    auto from = past_move._from, to = past_move._to;

    update_iterator(to, from);

    set_piece(from, past_move._state[1]);
    set_piece(to, past_move._state[0]);
}

void game_t::undo_attack_move(history_entry_t past_move)
{
    auto from = past_move._from, to = past_move._to;

    undo_simple_move(past_move);

    add_to_iterator(get_piece_color(get_piece(to)), to);
}

void game_t::undo_castling_move(history_entry_t past_move)
{
    auto from = past_move._from, to = past_move._to;
    auto is_white = get_piece_color(get_piece(to)) == color_t::w;
    bool king_side = from._j > to._j;

    auto rook_from = from, king_from = to;
    auto rook_to = pos_t{rook_from._i, (uint8_t)(king_side ? 5 : 3)};
    auto king_to = pos_t{king_from._i, (uint8_t)(king_side ? 6 : 2)};

    update_iterator(rook_to, rook_from);
    update_iterator(king_to, king_from);

    set_piece(rook_from, is_white ? piece_t::wr : piece_t::br);
    set_piece(king_from, is_white ? piece_t::wk : piece_t::bk);

    set_piece(rook_to, piece_t::empt);
    set_piece(king_to, piece_t::empt);
}

void game_t::undo_promotion_move(history_entry_t past_move)
{
    auto from = past_move._from, to = past_move._to;
    auto is_white = get_piece_color(get_piece(to)) == color_t::w;

    update_iterator(to, from);

    auto previous_piece = past_move._state[0];
    set_piece(from, is_white ? piece_t::wp : piece_t::bp);
    set_piece(to, previous_piece);

    if (previous_piece != piece_t::empt)
        add_to_iterator(get_piece_color(previous_piece), to);
}

void game_t::undo_en_passant_move(history_entry_t past_move)
{
    auto from = past_move._from, to = past_move._to;
    auto is_white = get_piece_color(get_piece(to)) == color_t::w;

    set_piece(from, is_white ? piece_t::wp : piece_t::bp);
    set_piece(pos_t{from._i, to._j}, is_white ? piece_t::bp : piece_t::wp);
    set_piece(to, piece_t::empt);
}

void game_t::redo_simple_move(history_entry_t new_move)
{
    auto to = new_move._to, from = new_move._from;
    // remove_from_iterator(to);
    update_iterator(from, to);

    set_piece(to, new_move._state[1]);
    set_piece(from, piece_t::empt);
}

void game_t::redo_attack_move(history_entry_t new_move)
{
    auto to = new_move._to, from = new_move._from;
    remove_from_iterator(to);
    update_iterator(from, to);

    set_piece(to, new_move._state[1]);
    set_piece(from, piece_t::empt);
}

void game_t::redo_en_passant_move(history_entry_t new_move)
{
    auto from = new_move._from, to = new_move._to;

    remove_from_iterator({from._i, to._j});
    update_iterator(from, to);

    set_piece(to, get_piece(from));
    set_piece(pos_t{from._i, to._j}, piece_t::empt);
    set_piece(pos_t{from._i, from._j}, piece_t::empt);
}

void game_t::redo_castling_move(history_entry_t new_move)
{
    auto from = new_move._from, to = new_move._to;

    bool king_side = from._j > to._j;
    auto is_white = get_piece_color(get_piece(to)) == color_t::w;

    auto rook_from = from, king_from = to;
    auto rook_to = pos_t{rook_from._i, (uint8_t)(king_side ? 6 : 2)};
    auto king_to = pos_t{king_from._i, (uint8_t)(king_side ? 5 : 3)};

    update_iterator(rook_from, rook_to);
    update_iterator(king_from, king_to);

    set_piece(rook_to, is_white ? piece_t::wr : piece_t::br);
    set_piece(king_to, is_white ? piece_t::wk : piece_t::bk);
    set_piece(rook_from, piece_t::empt);
    set_piece(king_from, piece_t::empt);
}

void game_t::redo_promotion_move(history_entry_t new_move)
{
    auto from = new_move._from, to = new_move._to;

    remove_from_iterator(to);
    update_iterator(from, to);

    set_piece(to, new_move._state[1]);
    set_piece(from, piece_t::empt);
}

bool game_t::move_pawn(pos_t from, pos_t to, bool commit)
{
    auto piece = get_piece(from);
    auto is_white = get_piece_color(piece) == color_t::w;
    auto empty_to = is_empty(to);
    auto diff_color = empty_to ? false : !same_col(from, to);

    if (is_white) {
        auto empt_u = is_empty(from.u());
        auto empt_uu = empt_u && is_empty(from.u().u());

        if ((from.u() == to && empt_u) // simple move.
            || (from._i == 1 && from.u(2) == to && empt_uu)
            || (from.lu() == to  && diff_color) // attack.
            || (from.ru() == to && diff_color)) // attack.
        {
            if (commit) commit_move(from, to);
            return true;
        }
    } else {
        auto empt_d = is_empty(from.d());
        auto empt_dd = empt_d && is_empty(from.d().d());

        if ((from.d() == to && empt_d)
            || (from._i == 6 && from.d(2) == to && empt_dd)
            || (from.ld() == to && diff_color)
            || (from.rd() == to && diff_color))
        {
            if (commit) commit_move(from, to);
            return true;
        }
    }

    if (can_en_passant(from, to)) {
        if (commit) do_en_passant(from, to);
        return true;
    }

    return false;
}

bool game_t::move_king(pos_t from, pos_t to, bool commit)
{
    if (from.retg_distance(to) == 1) {
        if (commit) commit_move(from, to);
        return true;
    }

    return false;
}

bool game_t::move_queen(pos_t from, pos_t to, bool commit)
{
    bool is_diag = abs(to._i - from._i) == abs(to._j - from._j);
    bool is_paral =  (to._i == from._i) || (to._j == from._j);

    if (!(is_diag || is_paral)) return false;

    if ((is_diag && empty_diagonal(from, to)) || empty_parallel(from, to)) {
        if (commit) commit_move(from, to);
        return true;
    }

    return false;
}

bool game_t::move_knight(pos_t from, pos_t to, bool commit)
{
    if (from._i == to._i || from._j == to._j) return false;

    if (from.radial_distance(to) == 3) {
        if (commit)
            commit_move(from, to);

        return true;
    }
    return false;
}

bool game_t::move_bishop(pos_t from, pos_t to, bool commit)
{
    bool is_diag = abs(to._i - from._i) == abs(to._j - from._j);

    if (!is_diag) return false;

    if (empty_diagonal(from, to)) {
        if (commit) commit_move(from, to);
        return true;
    }

    return false;
}

bool game_t::move_rook(pos_t from, pos_t to, bool commit)
{
    bool is_paral = (from._i == to._i) || (from._j == to._j);

    if (!is_paral) return false;

    pos_t king_pos = find_king(get_piece_color(get_piece(from)));
    if (to == king_pos && can_castling(from, to)) {
        if (commit)
            do_castling(from, to);
        return true;
    }

    if (empty_parallel(from, to)) {
        if (commit) commit_move(from, to);
        return true;
    }
    return false;
}

void game_t::commit_move(pos_t from, pos_t to)
{
    auto new_p = _board[from._i][from._j];
    auto old_p = _board[to._i][to._j];

    if (is_empty(to)) { // simple move.
        auto bkp = history_entry_t::make_simple_move(from, to, new_p);
        push_history(bkp);
        update_iterator(from, to);
    } else { // attack move.
        auto bkp = history_entry_t::make_attack_move(from, to, old_p, new_p);
        push_history(bkp);
        remove_from_iterator(to);
        update_iterator(from, to);
    }

    _board[to._i][to._j] = new_p;
    _board[from._i][from._j] = piece_t::empt;
}

bool game_t::empty_diagonal(pos_t from, pos_t to) const
{
    int v_coef = (from._i < to._i) ? 1 : -1;
    int h_coef = (from._j < to._j) ? 1 : -1;

    pos_t v = from;
    do
        v = v.inc(v_coef, h_coef);
    while (v != to && is_empty(v));

    return v == to;
}

bool game_t::empty_parallel(pos_t from, pos_t to) const
{
    auto v_step = 0, h_step = 0;

    if (from._i == to._i) {
        v_step = 0;
        h_step = (from._j < to._j) ? 1 : -1;

    } else {
        v_step = (from._i < to._i) ? 1 : -1;
        h_step = 0;
    }

    pos_t v = from;
    do
        v = v.inc(v_step, h_step);
    while (v != to && is_empty(v));

    return v == to;
}

bool game_t::can_en_passant(pos_t from, pos_t to) const
{
    if (_history_head == 0) return false;
    auto last_move = top_history();

    if (last_move._type != history_entry_t::type_t::simple)
        return false;

    auto lfrom = last_move._from, lto = last_move._to;
    auto lpiece = _board[lto._i][lto._j];

    bool last_move_was_a_pawn = lpiece == piece_t::bp || lpiece == piece_t::wp;
    bool jumped_2_positions = abs(lfrom._i - lto._i) == 2;
    bool stay_same_line = lto._i == from._i;
    bool will_same_column = lto._j == to._j;
    bool will_mean_line = ((from._i + lto._i)/2) == to._i;

    return last_move_was_a_pawn && jumped_2_positions && stay_same_line
            && will_same_column && will_mean_line;
}

void game_t::do_en_passant(pos_t from, pos_t to)
{
    auto bkp = history_entry_t::make_en_passant_move(from, to);
    push_history(bkp);

    remove_from_iterator({from._i, to._j});
    update_iterator(from, to);

    _board[to._i][to._j] = get_piece(from);
    _board[from._i][from._j] = piece_t::empt;
    _board[from._i][to._j] = piece_t::empt;
}

bool game_t::can_castling(pos_t from, pos_t to) const
{
    for (std::size_t k = 0; k < _history_head; ++k) {
        if (_history[k]._from == from || _history[k]._from == to)
            return false;
    }

    if (!empty_parallel(from, to)) return false;

    bool ooo = from._j < to._j;
    color_t color = get_piece_color(get_piece(from));
    pos_t king_pos = to;

    for (int i = 0; i < (ooo ? 3 : 2); ++i) {
        for (auto enemy_p: (color == color_t::w) ? _b_pieces : _w_pieces) {
            if (can_move(enemy_p, king_pos))
                return false;
            king_pos = ooo ? king_pos.l() : king_pos.r();
        }
    }
}

void game_t::do_castling(pos_t from, pos_t to)
{
    auto c = get_piece_color(get_piece(from));
    auto ooo = from._j == 0; // king_side ?
    auto king_i = (c == color_t::w ? 0 : 7), king_j = (ooo ? 2 : 6);
    auto rook_i = (c == color_t::w ? 0 : 7), rook_j = (ooo ? 3 : 5);

    update_iterator(from, pos_t{(uint8_t)rook_i, (uint8_t)rook_j});
    update_iterator(to,   pos_t{(uint8_t)king_i, (uint8_t)king_j});

    _board[rook_i][rook_j]   = get_piece(from);
    _board[king_i][king_j]   = get_piece(to);
    _board[from._i][from._j] = piece_t::empt;
    _board[to._i][to._j]     = piece_t::empt;

    push_history(history_entry_t::make_castling_move(from, to));
}

void game_t::push_history(history_entry_t history_entry)
{

    if (_history_head < _history.size()) {
        auto b = _history.begin() + _history_head;
        auto e = _history.end();
        _history.erase(b, e);
    }

    _history.push_back(history_entry);
    _history_head++;
}

history_entry_t game_t::pop_history()
{
    // TODO update exception message.

    if (_history_head == 0) throw std::string{"pop exception"};

    return _history[--_history_head];
}

history_entry_t game_t::top_history() const
{
    // TODO update exception message.s
    if (_history_head == 0) throw std::string{"top exception"};
    auto result = _history[_history_head - 1];
    return result;
}

bool game_t::same_col(pos_t pos_p1, pos_t pos_p2) const
{
    auto c1 = get_piece_color(get_piece(pos_p1));
    auto c2 = get_piece_color(get_piece(pos_p2));

    return c1 == c2;
}

bool game_t::same_i(pos_t p1, pos_t p2) const
{
    return p1._i == p2._i;
}

bool game_t::same_j(pos_t p1, pos_t p2) const
{
    return p1._j == p2._j;
}

bool game_t::is_empty(pos_t p) const
{
    return _board[p._i][p._j] == piece_t::empt;
}

void game_t::add_to_iterator(color_t col, pos_t pos)
{
    auto is_white = col == color_t::w;
    auto& container = is_white ? _w_pieces : _b_pieces;
    auto b = container.begin(), e = container.end();

    if (std::find(b, e, pos) == e) container.push_back(pos);
}

void game_t::update_iterator(pos_t old_pos, pos_t new_pos)
{
    auto color = get_piece_color(get_piece(old_pos));
    auto is_white = color == color_t::w;
    auto& container = is_white ? _w_pieces : _b_pieces;
    auto b = container.begin(), e = container.end();

    std::replace(b, e, old_pos, new_pos);
}

void game_t::remove_from_iterator(pos_t pos)
{
    auto color = get_piece_color(get_piece(pos));
    auto is_white = color == color_t::w;
    auto& container = is_white ? _w_pieces : _b_pieces;
    auto b = container.begin();
    auto e = container.end();

    auto last = std::remove(b, e, pos);
    container.erase(last, container.end());
}

void game_t::reset_iterators()
{
    _w_pieces.clear();
    _b_pieces.clear();

    for (uint8_t i = 0; i < 2; ++i) {
        for (uint8_t j = 0; j < 8; ++j) {
            _w_pieces.push_back(pos_t{i, j});
            _b_pieces.push_back(pos_t{(uint8_t)(7 - i), j});
        }
    }
}

pos_t game_t::find_king(color_t color) const
{
    bool is_white = color == color_t::w;
    auto& pieces_pos = is_white ? _w_pieces : _b_pieces;
    auto king_piece = is_white ? piece_t::wk : piece_t::bk;

    for (auto pos: pieces_pos) {
        auto piece = _board[pos._i][pos._j];
        if (piece == king_piece)
            return pos;
    }

    throw std::string{"where is my king???"};
}

bool game_t::is_castling(pos_t from, pos_t to) const
{
    auto piece_from = get_piece(from);
    auto piece_to = get_piece(to);

    color_t col = get_piece_color(piece_from);
    piece_t rook = col == color_t::w ? piece_t::wr : piece_t::br;
    piece_t king = col == color_t::w ? piece_t::wk : piece_t::bk;

    return (piece_from == rook && piece_to == king);
}

}