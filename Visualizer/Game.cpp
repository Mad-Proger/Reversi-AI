#include "Game.h"

Game::Game(Player& black, Player& white) :
    d(), mutex(),
    black(black), white(white) {
}

bool Game::makeMove() {
    mutex.lock();
    Desk d1(d);
    mutex.unlock();

    if (!d1.checkAnyMove(-1) && !d1.checkAnyMove(1)) {
        return false;
    }

    int x, y;
    if (d1.getCurrentColor() == 1) {
        black.findMove(d1, x, y);
    } else {
        white.findMove(d1, x, y);
    }

    mutex.lock();
    bool status = d.makeMove(x, y);
    mutex.unlock();

    return status;
}

void Game::passClick(int x, int y) {
    mutex.lock();

    WindowPlayer* clickReceiver;
    if (d.getCurrentColor() == 1) {
        clickReceiver = dynamic_cast<WindowPlayer*>(&black);
    } else {
        clickReceiver = dynamic_cast<WindowPlayer*>(&white);
    }

    if (clickReceiver != nullptr) {
        clickReceiver->passClick(x, y);
    }

    mutex.unlock();
}

std::array<std::array<int, 8>, 8> Game::getCurrentDeskState() {
    mutex.lock();

    std::array<std::array<int, 8>, 8> res;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            res[i][j] = d(i, j);
        }
    }

    mutex.unlock();

    return res;
}

std::pair<int, int> Game::getScore() {
    mutex.lock();
    std::pair<int, int> res = d.getScore();
    mutex.unlock();
    return res;
}
