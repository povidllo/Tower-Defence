#ifndef TOWERDEFENCE_ENEMYCONTROLLER_H
#define TOWERDEFENCE_ENEMYCONTROLLER_H
#include <fstream>
#include <memory>
#include "EnemySample.h"


class ProjectController;

class EnemyController {
public:
	EnemyController(ProjectController *projectController) :
		projectController(projectController), currentEnemy(nullptr) {}

	void setCurrentEnemy(const std::string &name);

	std::shared_ptr<EnemySample> getCurrentEnemy();

	void addEnemy(std::string name);

	bool removeEnemy(const std::string &name) const;

	std::vector<std::string> getEnemyNames() const;

	bool enemyExists(const std::string &name) const;

	void setEnemyTexture(const std::string &path) {
		if (!isPngBySignature(path)) {
			throw std::invalid_argument("Enemy texture does not have .png format");
		}
		currentEnemy->setEnemyTexturePath(path);
	}

private:
	ProjectController *projectController;
	std::shared_ptr<EnemySample> currentEnemy;

	bool isPngBySignature(const std::string &path) {
		std::ifstream file(path, std::ios::binary);
		if (!file)
			return false;

		unsigned char pngSignature[8] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
		unsigned char header[8] = {};

		file.read(reinterpret_cast<char *>(header), 8);

		return std::equal(std::begin(pngSignature), std::end(pngSignature), header);
	}
};


#endif // TOWERDEFENCE_ENEMYCONTROLLER_H
