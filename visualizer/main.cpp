#include "Game.h"
#include "AIPlayer.h"
#include "Evaluator.h"
#include "GameWindow.h"
#include "WindowPlayer.h"

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::filesystem::path executable = std::filesystem::absolute(argv[0]);
    std::filesystem::path resourceDir = executable.parent_path() / "res";
    std::filesystem::path blackModelFile = resourceDir / "black.model";
    std::filesystem::path whiteModelFile = resourceDir / "white.model";

    Evaluator nnEvaluator(blackModelFile, whiteModelFile);
    AIPlayer aiPlayer(nnEvaluator, 6);
    WindowPlayer windowPlayer;

    std::cout << "Enter color to play for: ";
    std::string color;
    std::cin >> color;

    Player& blackPlayer = color == "black" ? static_cast<Player&>(windowPlayer) : static_cast<Player&>(aiPlayer);
    Player& whitePlayer = color == "black" ? static_cast<Player&>(aiPlayer) : static_cast<Player&>(windowPlayer);

    Game game(blackPlayer, whitePlayer);
    std::filesystem::path fontFile = resourceDir / "font.ttf";
    GameWindow window(game, fontFile);

    window.open();

    bool moveMade;
    do {
        moveMade = game.makeMove();
    } while (window.isOpen() && moveMade);

    return 0;
}
