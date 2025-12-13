#include <fstream>
#include <iostream>

#include "src/inner/core/Engine.h"
#include "../editor/include/Entity/Project.h"

int main(int argc, char *argv[]) {
	std::cout << "hello user\n";
	std::cout << "to continue, enter yes\n";
	// std::string ans;
	// std::cin >> ans;
	// if (ans != "yes") {
	// 	return 0;
	// }
    std::cout << "Amogus\n";
	std::ifstream pFile("project.json");
	nlohmann::json pJson;
    Project proj(pJson);
    // TDEngine::Inner::Engine engine = TDEngine::Inner::Engine(std::make_shared<Project>(proj));
    // engine.startGame("asd");
}
