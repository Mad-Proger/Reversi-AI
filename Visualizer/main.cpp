#include "Game.h"
#include "Model.h"
#include "AIPlayer.h"
#include "GameWindow.h"
#include "WindowPlayer.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::invalid_argument("No model provided");
    }
    Model model;
    model.load(argv[1]);
    AIPlayer p1(model, 8);
    WindowPlayer p2;
    Game game(p1, p2);
    GameWindow window(game);

    window.open();

    bool moveMade;
    do {
        moveMade = game.makeMove();
    } while (window.isOpen() && moveMade);

    return 0;
}
