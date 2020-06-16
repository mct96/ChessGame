#pragma once

#include "game.hpp"
#include "game_types.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <regex>
#include <iostream>

using namespace std;

namespace ch {

struct move_t {
    struct game_state_t {
        bool _check;
        bool _check_mate;
    } _state_left;

    bool _attack;
    bool _promotion;
    piece_t _promotion_type;

    pos_t _from;
    pos_t _to;
    piece_t _piece;
};

std::ostream& operator<<(std::ostream& os, const move_t& move);

struct match_t {
    std::string _event;
    std::string _site;
    std::string _date;
    std::string _event_date;
    unsigned int _round;
    std::string _result;
    std::string _white_player;
    std::string _black_player;
    std::string _ECO;
    unsigned int _white_ELO;
    unsigned int _black_ELO;
    unsigned int _ply_count;

    std::vector<move_t> _moves;

    operator std::string() const;
};

std::ostream& operator<<(std::ostream &os, const match_t& m);

class pgn_parser_t {
public:
    pgn_parser_t(std::string filename, std::string regex_ptn_file);

    void update(std::string filename);

    match_t match() const;

    ~pgn_parser_t();

private:
    void buffer_file(std::string filename);
    void load_regex_patterns(std::string filename);
    // types to decode from pgn.
    enum class npiece_t {P, R, N, B, K, Q};
    struct decode_result_t {
        std::string piece_str;
        std::string disambiguation_str;
        std::string attack_str;
        std::string to_str;
        std::string promotion_str;
        std::string check_or_check_mate_str;
        std::string game_result_str;
    };

    decode_result_t extract_move_code(std::string move_code) const;
    move_t decode(std::string pgn_move, color_t color) const;

    bool was_attack(std::string attack_code) const;

    bool was_promotion(std::string promotion_code) const;
    piece_t selected_piece_promotion(
        std::string promotion_code, color_t c) const;

    pos_t decode_to(std::string move_code, color_t color) const;
    pos_t decode_from(pos_t to, std::string to_move_code,
                      piece_t selected_piece, color_t piece_color,
                      std::string disambiguation) const;

    npiece_t get_piece_type(std::string piece_code) const;

    move_t::game_state_t decode_state(std::string pgn_move) const;

    npiece_t decode_promotion(std::string pgn_move) const;

    piece_t pgn_to_piece(npiece_t type, color_t color) const;

    uint8_t file_to_number(char file) const;
    uint8_t rank_to_number(char rank) const;

    std::vector<std::pair<std::string, std::string>> read_moves() const;
    std::vector<move_t> parse_moves() const;

    std::regex _pgn_header_rgx, _pgn_move_code_rgx;
    std::string _file_buffer;
    mutable game_t _game; // to get from.
};


}

