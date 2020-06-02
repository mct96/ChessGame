#include "../../include/model/game.hpp"

namespace ch {

bool pos_t::operator==(const pos_t& p) const
{
    return _i == p._i && _j == p._j;
}

uint8_t pos_t::radial_distance(const pos_t& p) const
{
    return abs(_i - p._i) + abs(_j - p._j);
}

uint8_t pos_t::retg_distance(const pos_t& p) const
{
    return std::max(abs(_i - p._i), abs(_j - p._j));
}

bool pos_t::check_bounds(uint8_t li, uint8_t ui, uint8_t lj, uint8_t uj) const
{
    return (_i >= li && _i <= ui) && (_j >= lj && _j <= uj);
}

pos_t pos_t::u(uint8_t offset) const
{
    return pos_t{_i + offset, _j};
}
pos_t pos_t::d(uint8_t offset) const
{
    return pos_t{_i - offset, _j};
}
pos_t pos_t::l(uint8_t offset) const
{
    return pos_t{_i, _j - offset};
}
pos_t pos_t::r(uint8_t offset) const
{
    return pos_t{_i, _j + offset};
}
pos_t pos_t::lu(uint8_t offset) const
{
    return inc(offset, -offset);
}
pos_t pos_t::ld(uint8_t offset) const
{
    return inc(-offset, -offset);
}
pos_t pos_t::ru(uint8_t offset) const
{
    return inc(offset, offset);
}
pos_t pos_t::rd(uint8_t offset) const
{
    return inc(-offset, offset);
}

pos_t pos_t::inc(uint8_t v_offset, uint8_t h_offset) const
{
    return pos_t{_i + v_offset, _j + h_offset};
}

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

bool game_t::test_check_mate(color_t color)
{
    // TODO implements a method to list all possibles moves of a piece.
}

bool game_t::test_check(color_t color)
{
    auto king_pos = find_king(color);
    auto pieces = color == color_t::w ? _b_pieces : _w_pieces;
    auto b = pieces.begin(), e = pieces.end();

    return std::any_of(b, e,
        [=, this](pos_t p) { return move(p, king_pos, false); });
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
    auto from = past_move._from;
    auto to = past_move._to;

    switch (past_move._type) {
    case history_entry_t::type_t::simple:
    case history_entry_t::type_t::attack:
        _board[from._i][from._j] = past_move._state[1];
        _board[to._i][to._j] = past_move._state[0];
        // TODO update iterators.
        // TODO restore update after attack.
        break;

    case history_entry_t::type_t::en_passant:
        if (to._i == 2) { // black.
            _board[from._i][from._j] = piece_t::bp;
            _board[from._i][to._j] = piece_t::wp;
            // TODO update iterators.
        } else { // white.
            _board[from._i][from._j] = piece_t::wp;
            _board[from._i][to._j] = piece_t::bp;
            // TODO update iterators.
        }
        break;

    case history_entry_t::type_t::castling:
        bool king_side = from._j > to._j;
        auto is_white = get_piece_color(_board[to._i][to._j]) == color_t::w;

        _board[from._i][from._j] = is_white ? piece_t::wr : piece_t::br;
        _board[to._i][to._j] = is_white ? piece_t::wk : piece_t::bk;
        _board[from._i][king_side ? 6 : 2] = piece_t::empt;
        _board[from._i][king_side ? 5 : 3] = piece_t::empt;

        break;

    case history_entry_t::type_t::promotion:
        _board[from._i][from._j] = (to._i == 7) ? piece_t::wp : piece_t::bp;
        _board[to._i][to._j] = past_move._state[0];
        break;
    }
}

void game_t::redo()
{
    if (_history_head >= _history.size()) return;

    history_entry_t new_move = _history[_history_head];
    auto to = new_move._to, from = new_move._from;

    switch (new_move._type) {
    case history_entry_t::type_t::simple:
    case history_entry_t::type_t::attack:
        _board[to._i][to._j] = new_move._state[1];
        _board[from._i][from._j] = piece_t::empt;
        // TODO restore iterator.
        break;

    case history_entry_t::type_t::en_passant:
        _board[to._i][to._j] = to._i == 2 ? piece_t::bp : piece_t::wp;
        _board[from._i][to._j] = piece_t::empt;
        _board[from._i][from._j] = piece_t::empt;
        break;

    case history_entry_t::type_t::castling:
        bool king_side = from._j > to._j;
        auto is_white = get_piece_color(_board[to._i][to._j]) == color_t::w;

        _board[to._i][king_side ? 6 : 2] = is_white ? piece_t::wr : piece_t::br;
        _board[to._i][king_side ? 5 : 3] = is_white ? piece_t::wk : piece_t::bk;
        _board[from._i][from._j] = piece_t::empt;
        _board[to._i][to._j] = piece_t::empt;

        break;

    case history_entry_t::type_t::promotion:
        _board[to._i][to._j] = new_move._state[1];
        _board[from._j][from._i] = piece_t::empt;
        break;

    }

    ++_history_head;
}

bool game_t::move(pos_t from, pos_t to, bool commit)
{
    if (from == to || same_col(from, to)
        || !from.check_bounds() || !to.check_bounds())
        return false;

    auto piece = _board[from._i][from._j];

    switch (piece) {
    case piece_t::bp: case piece_t::wp:
        move_pawn(from, to, commit);
        break;
    case piece_t::br: case piece_t::wr:
        move_rook(from, to, commit);
        break;
    case piece_t::bh: case piece_t::wh:
        move_knight(from, to, commit);
        break;
    case piece_t::bb: case piece_t::wb:
        move_bishop(from, to, commit);
        break;
    case piece_t::bq: case piece_t::wq:
        move_queen(from, to, commit);
        break;
    case piece_t::bk: case piece_t::wk:
        move_king(from, to, commit);
        break;
    }

}

bool game_t::move_pawn(pos_t from, pos_t to, bool commit)
{
    auto piece = _board[from._i][from._j];

    auto diff_color = !same_col(from, to);
    if (piece == piece_t::wp) {
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

    // handle en passant move.
    if (can_en_passant(from, to)) {
        if (!commit) return true;

        auto bkp = history_entry_t::make_en_passant_move(from, to);
        push_history(bkp);

        _board[to._i][to._j] = piece;
        _board[from._i][from._j] = piece_t::empt;
        _board[from._i][to._j] = piece_t::empt;
        return true;
    }

    return false;
}

bool game_t::move_king(pos_t from, pos_t to, bool commit)
{
    if (from.retg_distance(to) == 1) {
        if (commit) commit_move(from, to);
    }

    return false;
}

bool game_t::move_queen(pos_t from, pos_t to, bool commit)
{
    bool is_diag = (to._i - from._i) == (to._j - from._j);
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

    if (from.radial_distance(to) == 2) {
        if (commit) commit_move(from, to);
        return true;
    }

    return false;
}

bool game_t::move_bishop(pos_t from, pos_t to, bool commit)
{
    bool is_diag = (to._i - from._i) == (to._j - from._j);

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
    } else { // attack move.
        auto bkp = history_entry_t::make_attack_move(from, to, old_p, new_p);
        push_history(bkp);
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
    bool will_middle_line = ((from._i + lto._i)/2) == to._i;

    return last_move_was_a_pawn && jumped_2_positions && stay_same_line
            && will_same_column && will_middle_line;
}

bool game_t::can_castling(pos_t from, pos_t to)
{
    // TODO to should be the king, otherwise will generate a ambiguous move.
    for (std::size_t k = 0; k < _history_head; ++k) {
        if (_history[k]._from == from || _history[k]._from == to)
            return false;
    }

    if (!empty_parallel(from, to)) return false;

    bool ooo = from._j < to._j;
    color_t color = get_piece_color(_board[from._i][from._j]);
    pos_t king_pos = to;

    for (int i = 0; i < ooo ? 3 : 2; ++i) {
        for (auto enemy_p: (color == color_t::w) ? _b_pieces : _w_pieces) {
            if (move(enemy_p, king_pos, false))
                return false;

            king_pos = ooo ? king_pos.l() : king_pos.r();
        }
    }
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
    if (_history_head == 0) throw std::exception{};

    return _history[--_history_head];
}

history_entry_t game_t::top_history() const
{
    // TODO update exception message.
    if (_history_head == 0) throw std::exception{};

    return _history[_history_head - 1];
}

bool game_t::same_col(pos_t p1, pos_t p2) const
{
    auto c1 = get_piece_color(_board[p1._i][p1._j]);
    auto c2 = get_piece_color(_board[p2._i][p2._j]);
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

void game_t::reset_iterators()
{
    _w_pieces.clear();
    _b_pieces.clear();

    for (auto i = 0; i < 2; ++i) {
        for (auto j = 0; i < 8; ++j) {
            _w_pieces.push_back(pos_t{i, j});
            _b_pieces.push_back(pos_t{7 - i, j});
        }
    }
}

pos_t game_t::find_king(color_t color) const
{
    auto& pieces = color == color_t::w ? _w_pieces : _b_pieces;
    auto b = pieces.begin(), e = pieces.end();
    auto result = std::find(b, e, color == color_t::w ?
                                  piece_t::wk : piece_t::bk);

    return *result;
}

}