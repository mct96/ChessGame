#include "../src/parser.hpp"

int main()
{
    try {
        ch::pgn_parser_t parser{"games/game_1977000.txt", "../src/assets/regex.txt"};
        parser.match();
    } catch (std::string str) {
        cout << str << endl;
    }
    return 0;
}