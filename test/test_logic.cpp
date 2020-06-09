#include "../include/model/game.hpp"

int main(int argc, char **argv)
{
    try {
        ch::game_t game_engine{};
        if (game_engine.move({1, 1}, {2, 2}) || game_engine.move({1, 1}, {2, 0}))
            cout << "an error" << endl;
        auto initial_hash = game_engine.hash();
        game_engine.move({1, 1}, {2, 1}); // 1
        game_engine.move({0, 2}, {1, 1}); // 2
        game_engine.move({1, 0}, {3, 0}); // 3
        game_engine.move({0, 0}, {1, 0}); // 4
        game_engine.move({0, 1}, {2, 2}); // 5
        game_engine.move({0, 3}, {0, 0}); // 6
        game_engine.move({3, 0}, {4, 0}); // 7
        game_engine.move({4, 0}, {5, 0}); // 8
        game_engine.move({6, 1}, {5, 0}); // 9

        auto final_hash = game_engine.hash();
        game_engine.print_board();

        for (int i = 0; i < 9; ++i)
            game_engine.undo();

        if (initial_hash == game_engine.hash()) {
            cout << "undo working properly." << endl;
        } else {
            cout << "undo not working properly." << endl;
        }

        game_engine.move({1, 2}, {2, 2});
        game_engine.move({1, 4}, {2, 4});
        game_engine.move({1, 3}, {3, 3});
        auto moves = game_engine.list_moves({0, 3});
        for (auto move: moves) {
            cout << "[" << (int)move._i << ", " << (int)move._j << "]" << endl;
        }
        // game_engine.move({0, 1}, {1, 3});
        // for (int i = 0; i < 9; ++i)
        //     game_engine.redo();

        // if (final_hash == game_engine.hash()) {
        //     cout << "redo working properly." << endl;
        // } else {
        //     cout << "redo not working properly." << endl;
        // }

        game_engine.print_board();
    } catch (std::string msg) {
        cout << msg << endl;
    }
    return 0;
}