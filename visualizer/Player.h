#pragma once

#include <utility>

#include "common/Desk.h"

class Player {
public:
    virtual void findMove(const Desk& d, int& xOpt, int& yOpt) = 0;
};
