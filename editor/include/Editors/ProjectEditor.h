#ifndef ProjectEditor_H
#define ProjectEditor_H

#include <QMainWindow>
#include "Project.h"
#include "ProjectController.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class ProjectEditor;
}

QT_END_NAMESPACE

class ProjectEditor : public QMainWindow {
    Q_OBJECT

public:
    explicit ProjectEditor(std::string path, std::string name, QWidget *parent = nullptr);
    explicit ProjectEditor(std::string path, QWidget *parent = nullptr);

    ~ProjectEditor() override;

private:
    // std::shared_ptr<Project> currentProject;

    std::shared_ptr<ProjectController> projectController;
    // CampaignEditor* campaignEditor;
    // AbilityEditor* abilityEditor;

    Ui::ProjectEditor *ui;
};


#endif