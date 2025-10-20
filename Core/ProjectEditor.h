#ifndef EDITOR_PROJECTEDITRO_H
#define EDITOR_PROJECTEDITRO_H
#include <string>

class ProjectEditor {
    public:
    std::string projectName;
    std::string currentProjectPath;

    ProjectEditor(const std::string &name, const std::string &path): projectName(name), currentProjectPath(path) {};
};


#endif //EDITOR_PROJECTEDITRO_H