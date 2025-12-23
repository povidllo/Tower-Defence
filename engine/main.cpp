#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../editor/include/Entity/Project.h"
#include "src/display/MainManager.h"


int main(int argc, char *argv[]) {
	// Используем std::filesystem для проверки путей (C++17)
	namespace fs = std::filesystem;
	const std::string projectFile = "project.json";

	if (!fs::exists(projectFile)) {
		std::cerr << "[ERROR] File not found: " << projectFile << std::endl;
		std::cerr << "Current working directory: " << fs::current_path() << std::endl;
		return 1;
	}

	std::ifstream pFile(projectFile);
	if (!pFile.is_open()) {
		std::cerr << "[ERROR] Failed to open file." << std::endl;
		return 1;
	}

	nlohmann::json pJson;
	try {
		pFile >> pJson;
	} catch (const nlohmann::json::parse_error &e) {
		std::cerr << "[ERROR] JSON parse error: " << e.what() << std::endl;
		return 1;
	}

	// Project может выбрасывать исключения при инициализации
	try {
		Project proj(pJson);
		TDEngine::Inner::MainManager mainManager(proj, 800, 600);
		mainManager.run("map_1");
	} catch (const std::exception &e) {
		std::cerr << "[CRITICAL ERROR] " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
