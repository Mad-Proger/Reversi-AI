#pragma once

#include <mutex>
#include <utility>
#include <condition_variable>

#include "common/Desk.h"
#include "Player.h"

class WindowPlayer :
    public Player {
public:
    WindowPlayer();

    void findMove(const Desk& d, int& xOpt, int& yOpt);
    void passClick(int x, int y);

private:
    std::mutex mutex;
    std::condition_variable cond;
    int moveX;
    int moveY;
};
