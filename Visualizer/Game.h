#pragma once

#include <mutex>
#include <array>
#include <utility>

#include "Desk.h"
#include "Player.h"
#include "WindowPlayer.h"

class Game {
public:
    Game(Player& black, Player& white);

    bool makeMove();
    void passClick(int x, int y);

    std::array<std::array<int, 8>, 8> getCurrentDeskState();
    std::pair<int, int> getScore();

private:
    Desk d;
    Player& black;
    Player& white;
    std::mutex mutex;
};

