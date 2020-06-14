#pragma once

#include "game.hpp"

#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <regex>
#include <iostream>

using namespace std;

namespace ch {
    using vector_png_t = std::vector<std::pair<std::string, std::string>>;

struct move_t {
    bool _check;
    bool _check_mate;
    bool _attack;

    pos_t _destination;
    piece_t _piece;
};

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

    vector_png_t _moves;

    operator std::string() const;
};

std::ostream& operator<<(std::ostream &os, const match_t& m);

class parser_t {
public:
    parser_t(std::string filename);

    void read_file();

    match_t match() const;

    ~parser_t();

private:
    vector_png_t read_moves() const;

    std::ifstream _ifs;
    std::string _buffer;
};

std::vector<move_t> convert(vector_png_t move_in_pgn);

}

