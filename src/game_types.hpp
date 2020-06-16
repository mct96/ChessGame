#pragma once

#include <cstdint>

namespace ch {
enum class color_t: uint8_t {
    b, w
};

enum class piece_t: uint8_t {
    empt = 0,
    bp = 100, br, bh, bb, bk, bq,
    wp = 200, wr, wh, wb, wk, wq
};

color_t get_piece_color(piece_t piece);

struct pos_t {
    uint8_t _i, _j;

    bool operator==(const pos_t& p) const;
    bool operator!=(const pos_t& p) const;

    // distance.
    uint8_t radial_distance(const pos_t& p) const;
    uint8_t retg_distance(const pos_t& p) const;

    bool check_bounds(
        uint8_t li, uint8_t ui, uint8_t lj, uint8_t uj) const;

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




}