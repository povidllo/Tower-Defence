#include <iostream>

#include "src/inner/core/Engine.h"
#include "../editor/include/Entity/Project.h"

int main(int argc, char *argv[]) {
    std::cout << "Amogus\n";
    Project proj = Project("testname", "testpath", 123);
    TDEngine::Inner::Engine engine = TDEngine::Inner::Engine(std::make_shared<Project>(proj));
    engine.startGame("asd");
}
