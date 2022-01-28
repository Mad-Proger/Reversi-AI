#include "Game.h"
#include "AIPlayer.h"
#include "Evaluator.h"
#include "GameWindow.h"
#include "WindowPlayer.h"
#include "src/model.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::invalid_argument("No model provided");
    }
    keras2cpp::Model model = keras2cpp::Model::load(argv[1]);
    Evaluator nnEvaluator(model);
    AIPlayer p1(nnEvaluator, 8);
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
