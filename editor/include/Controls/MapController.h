#ifndef TOWERDEFENCE_MAPCONTROLLER_H
#define TOWERDEFENCE_MAPCONTROLLER_H
#include "Map.h"

class ProjectController;


class MapController {

public:
	explicit MapController(ProjectController *projectController) :
		projectController(projectController), currentMap(nullptr) {}

	void setCurrentMap(const std::string &name);
	std::shared_ptr<Map> getCurrentMap() const;

	void addMap(const std::string &name, int height, int width);
	bool removeMap(const std::string &name) const;
	std::vector<std::string> getMapNames() const;
	bool mapExists(const std::string &name) const;

private:
	ProjectController *projectController;
	std::shared_ptr<Map> currentMap;
};


#endif // TOWERDEFENCE_MAPCONTROLLER_H
