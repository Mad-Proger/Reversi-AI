#include "WindowPlayer.h"

WindowPlayer::WindowPlayer(): mutex(), cond() {
    moveX = 8;
    moveY = 8;
}

void WindowPlayer::findMove(const Desk& d, int& xOpt, int& yOpt) {
    std::unique_lock<std::mutex> lock(mutex);
    cond.wait(lock, [&d, this]() {
        return d.checkMove(moveX, moveY, d.getCurrentColor()) || (moveX == -1 && moveY == -1);
    });
    lock.unlock();

    xOpt = moveX;
    yOpt = moveY;
}

void WindowPlayer::passClick(int x, int y) {
    std::lock_guard<std::mutex> lock(mutex);
    moveX = x;
    moveY = y;
    cond.notify_one();
}
