#include "parser.hpp"

#include <map>

namespace ch {

std::ostream& operator<<(std::ostream& os, const move_t& move)
{
    os << "check: " << move._state_left._check << "\n";
    os << "check mate: " << move._state_left._check_mate << "\n";
    os << "attack: " << move._attack << "\n";
    os << "promotion: " << move._promotion << "\n";
    os << "from: " << (int)move._from._i << ", " << (int)move._from._j << "\n";
    os << "to: " << (int)move._to._i << ", " << (int)move._to._j << "\n";
    os << "piece: " << (int)move._piece << "\n";
    return os;
}

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

    return str;
}

pgn_parser_t::pgn_parser_t(std::string filename, std::string regex_ptn_file)
    :
    _pgn_header_rgx{}, _pgn_move_code_rgx{},
    _file_buffer{},
    _game{}
{
    buffer_file(filename);
    load_regex_patterns(regex_ptn_file);
}

void pgn_parser_t::update(std::string filename)
{
    _file_buffer.clear();
    buffer_file(filename);
}

void pgn_parser_t::buffer_file(std::string filename)
{
    std::ifstream file{filename};
    if (!file) throw std::string{"The file could not be loaded."};

    constexpr uint buffer_sz = 120;
    char buffer[buffer_sz];

    while (!file.eof()) {
        file.getline(buffer, buffer_sz);
        _file_buffer.append(buffer);
        _file_buffer.append("\n");
    }
}

void pgn_parser_t::load_regex_patterns(std::string filename)
{
    std::ifstream file{filename};
    if (!file) throw std::string{"The file could not be loaded."};

    char local_buffer[255];

    file.ignore(255, '\n');
    file.getline(local_buffer, 255);
    _pgn_header_rgx.assign(std::string{local_buffer});
    cout << local_buffer << endl;

    file.ignore(255, '\n'); file.ignore(255, '\n');
    file.getline(local_buffer, 255);
    _pgn_move_code_rgx.assign(std::string{local_buffer});

    cout << local_buffer << endl;
}

match_t pgn_parser_t::match() const
{
    match_t result{};

    auto b = _file_buffer.begin(), e = _file_buffer.end();
    std::sregex_iterator iter{b, e, _pgn_header_rgx};

    result._event        = (*iter++)[1].str();
    result._site         = (*iter++)[1].str();
    result._date         = (*iter++)[1].str();
    result._event_date   = (*iter++)[1].str();
    result._round        = stoi((*iter++)[1].str());
    result._result       = (*iter++)[1].str();
    result._white_player = (*iter++)[1].str();
    result._black_player = (*iter++)[1].str();
    result._ECO          = (*iter++)[1].str();
    result._white_ELO    = stoi((*iter++)[1].str());
    result._black_ELO    = stoi((*iter++)[1].str());
    result._ply_count    = stoi((*iter++)[1].str());
    result._moves        = parse_moves();

    return result;
}

pgn_parser_t::~pgn_parser_t()
{

}

pgn_parser_t::decode_result_t pgn_parser_t::
    extract_move_code(std::string move_code) const
{
    std::smatch matched{};
    std::regex_match(move_code, matched, _pgn_move_code_rgx);

    decode_result_t result {
        matched[1].str(), matched[2].str(), matched[3].str(), matched[4].str(),
        matched[5].str(), matched[6].str()};

    cout << "1: "<< matched[1].str() << endl;
    cout << "2: "<< matched[2].str() << endl;
    cout << "3: "<< matched[3].str() << endl;
    cout << "4: "<< matched[4].str() << endl;
    cout << "5: "<< matched[5].str() << endl;
    cout << "6: "<< matched[6].str() << endl;
    _game.print_board();

    return result;
}

move_t pgn_parser_t::decode(std::string pgn_move, color_t color) const
{
    decode_result_t decoded_values = extract_move_code(pgn_move);

    move_t move{};
    move._attack = was_attack(decoded_values.attack_str);

    if (was_promotion(decoded_values.promotion_str)) {
        move._promotion = true;
        move._promotion_type =
            selected_piece_promotion(decoded_values.promotion_str, color);
    } else {
        move._promotion = false;
        move._promotion_type = piece_t::empt;
    }

    move._state_left = decode_state(decoded_values.check_or_check_mate_str);

    npiece_t piece_type = get_piece_type(decoded_values.piece_str);
    piece_t piece = pgn_to_piece(piece_type, color);
    move._piece = piece;

    move._to = decode_to(decoded_values.to_str, color);
    move._from = decode_from(move._to, decoded_values.to_str, piece, color,
                             decoded_values.disambiguation_str);

    if (!_game.move(move._from, move._to))
        throw std::string{"Error at parsing: "} + pgn_move + ".";

    return move;
}


bool pgn_parser_t::was_attack(std::string attack_code) const
{
    if (attack_code.empty()) return false;

    return tolower(attack_code[0]) == 'x';
}

bool pgn_parser_t::was_promotion(std::string promotion_code) const
{
    return !promotion_code.empty();
}

piece_t pgn_parser_t::
    selected_piece_promotion(std::string promotion_code, color_t c) const
{
    npiece_t selected_piece = decode_promotion(promotion_code);
    return pgn_to_piece(selected_piece, c);
}


pgn_parser_t::npiece_t pgn_parser_t::get_piece_type(std::string piece_code) const
{
    const static std::map<std::string, npiece_t> code_to_piece{
        {"K", npiece_t::K}, {"Q", npiece_t::Q}, {"B", npiece_t::B},
        {"N", npiece_t::N}, {"R", npiece_t::R}, {"", npiece_t::P}
    };
    piece_code[0] = toupper(piece_code[0]);
    return code_to_piece.at(piece_code);
}

move_t::game_state_t pgn_parser_t::decode_state(std::string pgn_move) const
{
    return move_t::game_state_t{pgn_move == "+", pgn_move == "#"};;
}

pgn_parser_t::npiece_t pgn_parser_t::
    decode_promotion(std::string pgn_move) const
{
    const static std::map<std::string, npiece_t> code_to_piece{
        {"Q", npiece_t::Q}, {"B", npiece_t::B},
        {"N", npiece_t::N}, {"R", npiece_t::R}
    };

    pgn_move = toupper(pgn_move[1]);
    return code_to_piece.at(pgn_move);
}

piece_t pgn_parser_t::pgn_to_piece(npiece_t type, color_t color) const
{
    uint8_t base_value = color == color_t::w ? 200 : 100;
    uint8_t offset = 0;

    switch (type) {
        case npiece_t::P: offset = 0; break;
        case npiece_t::R: offset = 1; break;
        case npiece_t::N: offset = 2; break;
        case npiece_t::B: offset = 3; break;
        case npiece_t::K: offset = 4; break;
        case npiece_t::Q: offset = 5; break;
    }

    return static_cast<piece_t>(base_value + offset);
}

pos_t pgn_parser_t::decode_from(pos_t to, std::string to_move_code,
                                piece_t selected_piece, color_t piece_color,
                                std::string disambiguation) const
{
    // Castling case.
    if (to_move_code == "O-O-O" || to_move_code == "O-O")
        return pos_t{
            (uint8_t)(piece_color == color_t::w ? 0 : 7),
            (uint8_t)(to_move_code == "O-O" ? 7 : 0)
        };

    auto pieces_pos = _game.can_move_to(to, selected_piece);
    if (pieces_pos.size() == 1)
        return pieces_pos.front();

    auto b = pieces_pos.begin(), e = pieces_pos.end();
    int8_t j = file_to_number(disambiguation[0]);
    auto m = std::remove_if(b, e, [=](pos_t pos) { return  j != pos._j; });
    pieces_pos.erase(m, pieces_pos.end());

    // Use file to disambiguate a move.
    if (pieces_pos.size() == 1)
        return pieces_pos.front();

    int8_t i = rank_to_number(disambiguation[1]);
    m = std::remove_if(b, e, [=](pos_t pos) { return  i != pos._i; });
    pieces_pos.erase(m, pieces_pos.end());

    // Use rank to disambiguate a move.
    if (pieces_pos.size() == 1)
        return pieces_pos.front();

    throw std::string{"Couldn't identify \"from\" position."};
}

pos_t pgn_parser_t::decode_to(std::string pgn_move, color_t color) const
{
    uint8_t i, j;

    if (pgn_move == "O-O-O" || pgn_move == "O-O") {
        i = color == color_t::w ? 0 : 7;
        j = 4;
    } else {
        i = rank_to_number(pgn_move[1]);
        j = file_to_number(pgn_move[0]);
    }

    return pos_t{i, j};
}

uint8_t pgn_parser_t::file_to_number(char file) const
{
    file = toupper(file);
    if (file == 'A') return 0;
    if (file == 'B') return 1;
    if (file == 'C') return 2;
    if (file == 'D') return 3;
    if (file == 'E') return 4;
    if (file == 'F') return 5;
    if (file == 'G') return 6;
    if (file == 'H') return 7;
}

uint8_t pgn_parser_t::rank_to_number(char rank) const
{
    return stoi(std::string{rank}) - 1;
}

std::vector<std::pair<std::string, std::string>> pgn_parser_t::read_moves() const
{
    std::vector<std::pair<std::string, std::string>> moves{};

    auto b = _file_buffer.begin(), e = _file_buffer.end();
    std::regex ptn{
        R"-(\d+\.\s([[:graph:]]+)(?:\s|\n)([[:graph:]]+)(?:\s|\n))-"};
    std::sregex_iterator iter{b, e, ptn, std::regex_constants::format_no_copy};

    decltype(iter) end{};
    for (;iter != end; ++iter)
        moves.push_back(std::make_pair((*iter)[1], (*iter)[2]));

    return moves;
}

std::vector<move_t> pgn_parser_t::parse_moves() const
{
    std::vector<move_t> parsed_moves{};

    auto moves_in_pgn = read_moves();
    for (auto move_in_pgn: moves_in_pgn) {
        move_t w_move = decode(move_in_pgn.first, color_t::w);
        parsed_moves.push_back(w_move);

        move_t b_move = decode(move_in_pgn.second, color_t::b);
        parsed_moves.push_back(b_move);
    }

    return parsed_moves;
}

std::ostream& operator<<(std::ostream &os, const match_t& m)
{
    os << static_cast<std::string>(m);
    return os;
}

}