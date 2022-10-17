#include "Game.h"
#include "AIPlayer.h"
#include "Evaluator.h"
#include "GameWindow.h"
#include "WindowPlayer.h"

int main(int argc, char* argv[]) {
    torch::nn::Sequential model(
        torch::nn::Linear(64, 256),
        torch::nn::Tanh(),
        torch::nn::Linear(256, 128),
        torch::nn::Tanh(),
        torch::nn::Linear(128, 1),
        torch::nn::Tanh()
    );
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
