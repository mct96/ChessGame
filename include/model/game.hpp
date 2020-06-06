#pragma once

#include <vector>
#include <cstdint>
#include <algorithm>
#include <exception>
#include <initializer_list>
#include <string>
#include <list>

#ifndef NDEBUB
#include <iostream>

using namespace std;

#endif

namespace ch {

class game_t;

struct pos_t {
    uint8_t _i, _j;

    bool operator==(const pos_t& p) const;
    // distance.
    uint8_t radial_distance(const pos_t& p) const;
    uint8_t retg_distance(const pos_t& p) const;

    bool check_bounds(
        uint8_t li = 0, uint8_t ui = 7, uint8_t lj = 0, uint8_t uj = 7) const;

    pos_t u(uint8_t offset = 1) const;
    pos_t d(uint8_t offset = 1) const;
    pos_t l(uint8_t offset = 1) const;
    pos_t r(uint8_t offset = 1) const;
    pos_t lu(uint8_t offset = 1) const;
    pos_t ld(uint8_t offset = 1) const;
    pos_t ru(uint8_t offset = 1) const;
    pos_t rd(uint8_t offset = 1) const;
    inline pos_t inc(uint8_t v_offset = 1, uint8_t h_offset = 1) const;
};

enum class color_t: uint8_t {b, w};
enum class piece_t: uint8_t {
    empt = 0,
    bp = 100, br, bh, bb, bk, bq,
    wp = 200, wr, wh, wb, wk, wq
};

color_t get_piece_color(piece_t piece);

class history_entry_t {
    friend class game_t;

public:

    // ordinary move.
    static history_entry_t make_simple_move(
        pos_t from, pos_t to, piece_t piece);

    // ordinary attack move.
    static history_entry_t make_attack_move(
        pos_t from, pos_t to, piece_t piece_before, piece_t piece_after);

    // en-passant move.
    // the color, position are implicit.
    static history_entry_t make_en_passant_move(pos_t from, pos_t to);

    // castling move.
    // the envolved pieces are implicit.
    static history_entry_t make_castling_move(pos_t from, pos_t to);

    // promotion.
    // piece before, piece after: selected promotion piece.
    static history_entry_t make_promotion_move(
        pos_t from, pos_t to, piece_t piece_before, piece_t piece_after);


    history_entry_t() = default;
    history_entry_t(const history_entry_t&) = default;
    ~history_entry_t();
private:
    enum class type_t: uint8_t {
        simple, attack, en_passant, castling, promotion };

    explicit history_entry_t(
        type_t type, pos_t from, pos_t to, piece_t state_before = piece_t::empt,
        piece_t state_after = piece_t::empt);

    type_t _type;
    piece_t _state[2];
    pos_t _from, _to;
};

class game_t {
public:
    using positions_list_t = std::list<pos_t>;

    explicit game_t();
    ~game_t();

#ifndef NDEBUG
    char piece_to_char(piece_t) const;
    void print_board() const;
#endif
    bool test_check_mate(color_t color); // TODO should be constant.
    bool test_check(color_t color) const;
    void reset();

    void undo();
    void redo();

    template <typename Callable>
    // must return a ch::piece_t an take 0 (any) parameter.
    void promotion_handler(Callable callabe);

    bool move(pos_t from, pos_t to, bool commit = true);
    bool can_move(pos_t from, pos_t to) const;

    positions_list_t list_moves(pos_t pos) const;

protected:
    bool move_pawn(pos_t from, pos_t to, bool commit = true);
    bool move_king(pos_t from, pos_t to, bool commit = true);
    bool move_queen(pos_t from, pos_t to, bool commit = true);
    bool move_knight(pos_t from, pos_t to, bool commit = true);
    bool move_bishop(pos_t from, pos_t to, bool commit = true);
    bool move_rook(pos_t from, pos_t to, bool commit = true);

    void commit_move(pos_t from, pos_t to);

    bool empty_diagonal(pos_t from, pos_t to) const;
    bool empty_parallel(pos_t from, pos_t to) const;

    bool can_en_passant(pos_t from, pos_t to) const;
    bool can_castling(pos_t from, pos_t to) const;

    void push_history(history_entry_t history_entry);
    history_entry_t pop_history();
    history_entry_t top_history() const;

    bool same_col(pos_t p1, pos_t p2) const;
    inline bool same_j(pos_t p1, pos_t p2) const;
    inline bool same_i(pos_t p1, pos_t p2) const;
    inline bool is_empty(pos_t p) const;

    void add_to_iterator(color_t col, pos_t pos);
    void update_iterator(pos_t old_pos, pos_t new_pos);
    void remove_from_iterator(pos_t pos);
    void reset_iterators();

    pos_t find_king(color_t color) const;

private:
    std::vector<history_entry_t> _history;
    uint16_t _history_head;
    piece_t _board[8][8];

    // just iterators to enhance algorithms performance.
    std::list<pos_t> _b_pieces;
    std::list<pos_t> _w_pieces;
};

}