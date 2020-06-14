#include "../include/model/game.hpp"

void test_undo_redo()
{
    ch::game_t game_engine{};
    auto initial_hash = game_engine.hash();
    std::list<std::pair<ch::pos_t, ch::pos_t>> moves{
        {{{1, 1}, {2, 1}},
         {{0, 2}, {1, 1}},
         {{1, 0}, {3, 0}},
         {{0, 0}, {1, 0}},
         {{0, 1}, {2, 2}},
         {{0, 3}, {0, 0}},
         {{3, 0}, {4, 0}},
         {{4, 0}, {5, 0}},
         {{6, 1}, {5, 0}}}};

        for (auto move: moves)
            if (!game_engine.move(move.first, move.second)) {
                cout << "unexpected invalid move.\n";
            }

        auto final_hash = game_engine.hash();
        game_engine.print_board();

        for (int i = 0; i < moves.size(); ++i)
            game_engine.undo();

        if (initial_hash == game_engine.hash()) {
            cout << "undo working properly." << endl;
        } else {
            cout << "undo not working properly." << endl;
        }

        for (int i = 0; i < moves.size(); ++i) {
            game_engine.redo();
        }

        if (final_hash == game_engine.hash()) {
            cout << "redo working properly." << endl;
        } else {
            cout << "redo not working properly." << endl;
        }

        game_engine.print_board();
}

void test_check()
{
    ch::game_t game_engine{};
    game_engine.move({1, 4}, {3, 4}); // white
    game_engine.move({6, 0}, {5, 0}); // black
    game_engine.move({0, 3}, {4, 7}); // white
    game_engine.move({5, 0}, {4, 0}); // black
    game_engine.move({0, 5}, {3, 2}); // white
    game_engine.move({4, 0}, {3, 0}); // black
    game_engine.move({4, 7}, {6, 5}); // white
    game_engine.move({6, 3}, {5, 3}); // white
    game_engine.print_board();

    if (game_engine.test_check(ch::color_t::b))
        cout << "check: true \n";
    else
        cout << "check: false \n";

    if (game_engine.test_check_mate(ch::color_t::b))
        cout << "check mate: true \n";
    else
        cout << "check mate: false \n";
    game_engine.print_board();
}

int main(int argc, char **argv)
{
    try {
        test_undo_redo();

        test_check();


    } catch (std::string msg) {
        cout << msg << endl;
    }
    return 0;
}