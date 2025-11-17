#include "Project.h"

std::string Project::getName() const {
    return name;
}

void Project::setName(const std::string &name) {
    this->name = name;
}

std::string Project::getPath() const {
    return path;
}

void Project::setPath(const std::string &path) {
    this->path = path;
}

std::time_t Project::getLastSaveDate() const {
    return lastSaveDate;
}

void Project::setLastSaveDate(std::time_t last_save_date) {
    lastSaveDate = last_save_date;
}

std::map<std::string, std::shared_ptr<TowerSample> > Project::getTowers() const {
    return towers;
}

std::shared_ptr<TowerSample> Project::getTower(std::string &name) const {
    return towers.at(name);
}

void Project::addTower(const std::shared_ptr<TowerSample> &tower) {
    this->towers[tower->getName()] = tower;
}

bool Project::existsTower(const std::string &name) const {
    const auto it = towers.find(name);
    return it != towers.end();
}
