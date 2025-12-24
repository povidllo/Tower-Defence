#include <filesystem>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../editor/include/Entity/Project.h"
#include "src/display/MainManager.h"

int main(int argc, char *argv[]) {
	namespace fs = std::filesystem;
	const std::string projectFile = "project.json";

	if (!fs::exists(projectFile)) {
		std::cerr << "[ERROR] File not found: " << projectFile << std::endl;
		return 1;
	}

	std::ifstream pFile(projectFile);
	if (!pFile.is_open())
		return 1;

	nlohmann::json pJson;
	try {
		pFile >> pJson;
	} catch (const nlohmann::json::parse_error &e) {
		std::cerr << "[ERROR] JSON parse: " << e.what() << std::endl;
		return 1;
	}

	try {
		Project proj(pJson);

		TDEngine::Inner::MainManager mainManager(proj, 850, 600);
		mainManager.run();
	} catch (const std::exception &e) {
		std::cerr << "[CRITICAL ERROR] " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
