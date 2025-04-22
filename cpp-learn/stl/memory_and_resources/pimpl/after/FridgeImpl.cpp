#include "Engine.h"
#include "Fridge.h"

class Fridge::FridgeImpl {
public:
    void coolDown() {
        engine.stop();
    }

private:
    Engine engine;
};

Fridge::Fridge() : impl(new FridgeImpl()) {}

Fridge::~Fridge() = default;

void Fridge::coolDown() {
    impl->coolDown();
}
