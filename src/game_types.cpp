#include "game_types.hpp"

#include <string>

namespace ch {

bool pos_t::operator==(const pos_t& p) const
{
    return _i == p._i && _j == p._j;
}

bool pos_t::operator!=(const pos_t& p) const
{
    return !(*this == p);
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
    return pos_t{(uint8_t)(_i + offset), _j};
}

pos_t pos_t::d(uint8_t offset) const
{
    return pos_t{(uint8_t)(_i - offset), _j};
}

pos_t pos_t::l(uint8_t offset) const
{
    return pos_t{_i, (uint8_t)(_j - offset)};
}

pos_t pos_t::r(uint8_t offset) const
{
    return pos_t{_i, (uint8_t)(_j + offset)};
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
    return pos_t{(uint8_t)(_i + v_offset), (uint8_t)(_j + h_offset)};
}

color_t get_piece_color(piece_t piece) {
    auto upiece = static_cast<uint8_t>(piece);
    if (upiece >= 200) return color_t::w;
    if (upiece >= 100) return color_t::b;
    throw std::string{"there isn't any piece here, so there isn't any color."};
}

}