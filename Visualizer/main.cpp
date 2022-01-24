#include "Game.h"
#include "Model.h"
#include "AIPlayer.h"
#include "GameWindow.h"
#include "WindowPlayer.h"

int main() {
    Model model;
    model.load("reversi.model");
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
