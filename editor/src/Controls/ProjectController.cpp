#include "ProjectController.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>

#include "EnemyEditor.h"

namespace fs = std::filesystem;


ProjectController::ProjectController(const std::string &path) {
	fs::path p(path);
	fs::path jsonFile = p / "Project.json";
	if (!fs::exists(jsonFile)) {
		throw fs::filesystem_error("Path or JSON file doesn't exist", p,
								   std::make_error_code(std::errc::no_such_file_or_directory));
	}

	std::ifstream inFile(jsonFile.string());
	if (!inFile) {
		throw fs::filesystem_error("Can't open Project JSON file", jsonFile, std::make_error_code(std::errc::io_error));
	}

	nlohmann::json j;
	try {
		inFile >> j;
	} catch (const nlohmann::json::parse_error &e) {
		throw std::runtime_error(std::string("JSON parse error: ") + e.what());
	}

	currentProject = std::make_shared<Project>(j);
	loadControls();
}

ProjectController::ProjectController(const std::string &path, const std::string &name) {
	fs::path p(path);
	if (!fs::exists(p)) {
		throw fs::filesystem_error("Path doesn't exist", p, std::make_error_code(std::errc::no_such_file_or_directory));
	}

	fs::path fullPath = p / name;

	try {
		fs::create_directory(fullPath);

		fs::path jsonProjectFilePath = fullPath / "Project.json";

		auto now = std::chrono::system_clock::now();
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
		currentProject = std::make_shared<Project>(name, fullPath.string(), ms);

		std::ofstream outFile(jsonProjectFilePath.string());
		if (!outFile) {
			throw fs::filesystem_error("Can't open file", jsonProjectFilePath,
									   std::make_error_code(std::errc::io_error));
		}

		outFile << currentProject->toJson().dump(4);
	} catch (...) {
		if (fs::exists(fullPath)) {
			fs::remove_all(fullPath);
		}
		throw;
	}

	loadControls();
}

std::shared_ptr<TowerController> ProjectController::getTowerController() { return towerController; }
std::shared_ptr<EnemyController> ProjectController::getEnemyController() { return enemyController; }

void ProjectController::loadControls() {
	towerController = std::make_shared<TowerController>(this);
	enemyController = std::make_shared<EnemyController>(this);
}
