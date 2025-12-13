#include <fstream>
#include <iostream>
#include <filesystem>

#include "src/inner/core/Engine.h"
#include <nlohmann/json.hpp>
#include "../editor/include/Entity/Project.h"

int main(int argc, char *argv[]) {
    std::cout << "Amogus\n";
	std::ifstream pFile("../../engine/project.json");
	if (!pFile.is_open()) {
		std::cerr << "[ERROR] Failed to open file: " << "project.json" << std::endl;
		std::cerr << "Make sure the file is in the working directory!" << std::endl;
		// Полезно вывести текущую директорию (через std::filesystem), чтобы понять, где ищет программа
		return 1;
	}
	nlohmann::json pJson;
	pFile >> pJson;
    Project proj(pJson);
    TDEngine::Inner::Engine engine = TDEngine::Inner::Engine(std::make_shared<Project>(proj));
    engine.startGame("map_1");
}
