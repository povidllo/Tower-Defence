#include "ProjectController.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <qpainter.h>

#include "TextureManager.h"

namespace fs = std::filesystem;

Project::json ProjectController::loadFromFile(const std::string &path) {
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

	return j;
}

bool ProjectController::saveProject() {
	try {
		fs::path path = currentProject->getPath();
		json j = toJson();
		fs::path jsonFile = path / "Project.json";


		std::ofstream file(jsonFile);
		file << std::setw(4) << j << std::endl;

		setProjectLastSaveDate(std::time(nullptr));

		return true;
	} catch (const std::exception &e) {
		return false;
	}
}

ProjectController::ProjectController(const std::string &path) {
	auto j = loadFromFile(path);

	currentProject = std::make_shared<Project>(j);
	loadControls();
	setEmptyTile();
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
	setEmptyTile();
}

std::shared_ptr<TowerController> ProjectController::getTowerController() { return towerController; }

std::shared_ptr<EnemyController> ProjectController::getEnemyController() { return enemyController; }

std::shared_ptr<MapController> ProjectController::getMapController() { return mapController; }

json ProjectController::toJson() const { return currentProject->toJson(); }

void ProjectController::fromJson(const json &j) { currentProject->fromJson(j); }

std::string ProjectController::getProjectName() const { return currentProject->getName(); }

void ProjectController::setProjectName(const std::string &name) const { currentProject->setName(name); }

std::string ProjectController::getProjectPath() const { return currentProject->getPath(); }

void ProjectController::setProjectPath(const std::string &path) const { currentProject->setPath(path); }

std::time_t ProjectController::getProjectLastSaveDate() const { return currentProject->getLastSaveDate(); }

void ProjectController::setProjectLastSaveDate(const std::time_t &date) const { currentProject->setLastSaveDate(date); }

std::vector<std::shared_ptr<TowerSample> > &ProjectController::getTowers() const { return currentProject->getTowers(); }

std::vector<std::shared_ptr<EnemySample> > &ProjectController::getEnemies() const {
	return currentProject->getEnemies();
}

void ProjectController::removeEnemiesFromWaves(std::string enemyName) const {
	auto maps = currentProject->getMaps();

	for (auto &map: maps) {
		for (auto &wave: map->getWaves()) {
			wave->getEnemies().erase(std::remove_if(wave->getEnemies().begin(), wave->getEnemies().end(),
													[enemyName](const auto &pair) { return pair.first == enemyName; }),
									wave->getEnemies().end());
		}
	}
}

void ProjectController::removeTowersFromSpots(const std::string &towerName) {
	for (auto &map: currentProject->getMaps()) {
		for (auto &spot: map->getSpots()) {
			spot->removeNextUpgrade(towerName);
		}
	}
}

void ProjectController::loadControls() {
	towerController = std::make_shared<TowerController>(this);
	enemyController = std::make_shared<EnemyController>(this);
	mapController = std::make_shared<MapController>(this);
}

void ProjectController::setEmptyTile() {
	int imageSize = TextureManager::instance().getImageSize();

	QPixmap empty(imageSize, imageSize);
	empty.fill(Qt::transparent);

	QPainter p(&empty);
	p.setPen(Qt::NoPen);
	p.setBrush(QColor(50, 50, 50, 100));
	p.drawRect(0, 0, imageSize, imageSize);
	p.setPen(QColor(100, 100, 100));
	p.drawText(0, 0, imageSize, imageSize, Qt::AlignCenter, "∅");
	p.setPen(QPen(Qt::darkGray, 2));
	p.drawLine(0, 0, imageSize, imageSize);
	p.drawLine(0, imageSize, imageSize, 0);

	TextureManager::instance().registerTexture(0, "");
	TextureManager::instance().pixmaps[0] = empty;
}
