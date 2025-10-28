// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "ProjectEditor.h"
#include "ui_ProjectEditor.h"


ProjectEditor::ProjectEditor(std::string path, std::string name, QWidget *parent) : QMainWindow(parent), ui(new Ui::ProjectEditor) {
    ui->setupUi(this);
    projectController = std::make_shared<ProjectController>(path, name);
    qDebug() << "created project";
}

ProjectEditor::ProjectEditor(std::string path, QWidget *parent) : QMainWindow(parent), ui(new Ui::ProjectEditor) {
    ui->setupUi(this);
    projectController = std::make_shared<ProjectController>(path);
    qDebug() << "created project";
}

ProjectEditor::~ProjectEditor() {
    delete ui;

}