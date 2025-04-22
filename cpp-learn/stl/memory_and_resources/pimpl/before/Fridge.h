#ifndef FRIDGE_H
#define FRIDGE_H

#include "Engine.h"

class Fridge {
public:
    void coolDown();

private:
    Engine engine{};
};

#endif // FRIDGE_H
