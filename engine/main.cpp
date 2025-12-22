#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>
#include "../editor/include/Entity/Project.h"
#include "src/display/MainManager.h"
#include "src/inner/core/Engine.h"

int main(int argc, char *argv[]) {
	std::cout << "Amogus\n";
	std::ifstream pFile("project.json");
	if (!pFile.is_open()) {
		std::cerr << "[ERROR] Failed to open file: " << "project.json" << std::endl;
		std::cerr << "Make sure the file is in the working directory!" << std::endl;
		// Полезно вывести текущую директорию (через std::filesystem), чтобы понять, где ищет программа
		return 1;
	}
	nlohmann::json pJson;
	pFile >> pJson;
	Project proj(pJson);

	TDEngine::Inner::MainManager mainManager(proj, 800, 600);
	mainManager.mainLoop("map_1");
}
