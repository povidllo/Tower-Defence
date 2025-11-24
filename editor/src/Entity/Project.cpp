#include "Project.h"

Project::Project(const std::string &name, const std::string &path, const std::time_t &lastSaveDate) {
    this->name = name;
    this->lastSaveDate = lastSaveDate;
    this->path = path;
}

Project::Project(const json &j) {
    Project::fromJson(j);
}

Project::json Project::toJson() const {
    return {
        {"name", name},
        {"lastSaveDate", lastSaveDate},
        {"path", path}
    };
}

void Project::fromJson(const json &j) {
    name = j.at("name").get<std::string>();
    lastSaveDate = j.at("lastSaveDate").get<std::time_t>();
}

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

std::vector<std::shared_ptr<TowerSample> > &Project::getTowers() {
    return towers;
}
