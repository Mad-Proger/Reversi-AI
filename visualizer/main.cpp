#include "Game.h"
#include "AIPlayer.h"
#include "Evaluator.h"
#include "GameWindow.h"
#include "WindowPlayer.h"

#include <filesystem>

int main(int argc, char* argv[]) {
    std::filesystem::path executable = std::filesystem::absolute(argv[0]);
    std::filesystem::path resourceDir = executable.parent_path() / "res";
    std::filesystem::path blackModelFile = resourceDir / "black.model";
    std::filesystem::path whiteModelFile = resourceDir / "white.model";

    Evaluator nnEvaluator(blackModelFile, whiteModelFile);
    AIPlayer p1(nnEvaluator, 8);
    WindowPlayer p2;
    Game game(p1, p2);
    std::filesystem::path fontFile = resourceDir / "font.ttf";
    GameWindow window(game, fontFile);

    window.open();

    bool moveMade;
    do {
        moveMade = game.makeMove();
    } while (window.isOpen() && moveMade);

    return 0;
}