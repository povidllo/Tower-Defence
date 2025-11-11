#include "ProjectEditor.h"
#include "ui_ProjectEditor.h"


ProjectEditor::ProjectEditor(std::string path, std::string name, QWidget *parent) : QMainWindow(parent),
    ui(new Ui::ProjectEditor) {
    ui->setupUi(this);
    projectController = std::make_shared<ProjectController>(path, name);
    qDebug() << "created project";
    commonSetUp();
}

ProjectEditor::ProjectEditor(std::string path, QWidget *parent) : QMainWindow(parent), ui(new Ui::ProjectEditor) {
    ui->setupUi(this);
    projectController = std::make_shared<ProjectController>(path);
    qDebug() << "created project";
    commonSetUp();
}

ProjectEditor::~ProjectEditor() {
    delete ui;
}

void ProjectEditor::commonSetUp() {
    towerEditor = std::make_unique<TowerEditor>(projectController->getTowerController());

    connect(ui->actionTower_editor, &QAction::triggered, this, &ProjectEditor::openTowerEditor);
}

void ProjectEditor::openTowerEditor() {
    qDebug() << "opening tower editor";
    if (centralWidget()) {
        centralWidget()->deleteLater();
    }

    setCentralWidget(towerEditor.get());

    ui->actionTower_editor->setChecked(true);
}
