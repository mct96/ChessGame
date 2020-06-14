#include "../../include/model/parser.hpp"

namespace ch {

match_t::operator std::string() const
{
    std::string str{};
    str += "event: " + _event + "\n";
    str += "site: " + _site + "\n";
    str += "date: " + _date + "\n";
    str += "event date: " + _event_date + "\n";
    str += "round: " + to_string(_round) + "\n";
    str += "result: " + _result + "\n";
    str += "white player: " + _white_player + "\n";
    str += "black player: " + _black_player + "\n";
    str += "ECO: " + _ECO + "\n";
    str += "white ELO: " + to_string(_white_ELO) + "\n";
    str += "black ELO: " + to_string(_black_ELO) + "\n";
    str += "ply count: " + to_string(_ply_count) + "\n";
    str += "\n\nmoves (total = "+ to_string(_moves.size()) + "):\n";

    for (auto move: _moves)
        str += "   " + move.first + " " +move.second + "\n";

    return str;
}

parser_t::parser_t(std::string filename)
    :
    _ifs{filename}
{
    if (_ifs)
        read_file();
    else
        throw std::string{"Couldn't read the file."};
}

void parser_t::read_file()
{
    constexpr uint buffer_sz = 120;
    char buffer[buffer_sz];

    while (!_ifs.eof()) {
        _ifs.getline(buffer, buffer_sz);
        _buffer.append(buffer);
        _buffer.append("\n");
    }
}

match_t parser_t::match() const
{
    match_t result{};
    std::regex ptn{R"-(\[\w+? "([[:print:]]+?)"\])-"};

    auto b = _buffer.begin(), e = _buffer.end();
    std::sregex_iterator iter{b, e, ptn};

    result._event = (*iter++)[1].str();
    result._site = (*iter++)[1].str();
    result._date = (*iter++)[1].str();
    result._event_date = (*iter++)[1].str();
    result._round = stoi((*iter++)[1].str());
    result._result = (*iter++)[1].str();
    result._white_player = (*iter++)[1].str();
    result._black_player = (*iter++)[1].str();
    result._ECO = (*iter++)[1].str();
    result._white_ELO = stoi((*iter++)[1].str());
    result._black_ELO = stoi((*iter++)[1].str());
    result._ply_count = stoi((*iter++)[1].str());
    result._moves = read_moves();

    return result;
}

parser_t::~parser_t()
{

}

vector_png_t parser_t::read_moves() const
{
    vector_png_t moves{};

    auto b = _buffer.begin(), e = _buffer.end();
    std::regex ptn{R"-(\d+\.\s([[:graph:]]+)(?:\s|\n)([[:graph:]]+)(?:\s|\n))-"};
    std::sregex_iterator iter{b, e, ptn, std::regex_constants::format_no_copy};

    decltype(iter) end{};
    for (;iter != end; ++iter)
        moves.push_back(std::make_pair((*iter)[1], (*iter)[2]));

    return moves;
}

std::ostream& operator<<(std::ostream &os, const match_t& m)
{
    os << static_cast<std::string>(m);
    return os;
}

std::vector<move_t> convert(vector_png_t moves_in_pgn) {
    // TODO extract all regex to a struct with static constant members.
    // TODO work directly with move struct instead of pgn notation.
    const std::string piece_ptn = "R|N|B|K|Q";
    const std::string movement_ptn = "(?:O-O)|(?:O-O-O)|(?:[a-h][1-8])";
    const std::string ptn_str = "^(" + piece_ptn + ")?([a-h]?[1-8]?)?(x)?(" +
                            movement_ptn + ")(=[R|N|B|Q])?([\+|#])?$";
    // ^(R|N|B|K|Q)?([a-h]?[1-8]?)?(x)?((?:O-O)|(?:O-O-O)|(?:[a-h][1-8]))
    // (=[R|N|B|Q])?([\+|#])?$
    // 1 group: piece
    // 2 group: disambiguating
    // 3 group: attack
    // 4 group: rock x-side, destination
    // 5 group: promotion
    // 6 group: check / check-mate
    std::regex ptn{ptn_str};

    std::vector<move_t> result;
    for (auto move_in_pgn : moves_in_pgn) {
        move_t move;
    }
}

}