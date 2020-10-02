#include <iostream>
#include "Game.hpp"

int main(int argc, char* argv[])
{
    Game game = Game();
    Game().Run();

    cout << "Game has terminated successfully, score: "
         << game.GetScore() << ", size: " << game.GetSize()
         << endl;

    return 0;
}
