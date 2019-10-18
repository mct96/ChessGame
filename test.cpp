
#include "include/puzzle.hpp"

int main()
{
    using namespace ch;
    ch::CPuzzle puzzle{};
    puzzle.print();
    auto res = puzzle.getPieceAt(CPosition{5, 1});
    res.lock()->print();
    return 0;
}