#include "../include/model/game.hpp"

int main(int argc, char **argv)
{
    try {
        ch::game_t game_engine{};
        game_engine.move({1, 1}, {2, 1});
        game_engine.move({0, 2}, {1, 1});
        game_engine.move({1, 0}, {3, 0});
        game_engine.move({0, 0}, {1, 0});
        game_engine.move({0, 1}, {2, 2});
        game_engine.move({0, 3}, {0, 0});
        game_engine.move({3, 0}, {4, 0});
        game_engine.move({4, 0}, {5, 0});
        game_engine.move({6, 1}, {5, 0});
        game_engine.undo();
        game_engine.redo();
        game_engine.print_board();
    } catch (std::string msg) {
        cout << msg << endl;
    }
    return 0;
}