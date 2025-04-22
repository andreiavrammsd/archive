#ifndef FRIDGE_H
#define FRIDGE_H

#include <memory>

class Fridge {
public:
    Fridge();

    ~Fridge();

    void coolDown();

private:
    class FridgeImpl;

    std::unique_ptr<FridgeImpl> impl;
};

#endif // FRIDGE_H
