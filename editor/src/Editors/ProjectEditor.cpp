#include "ProjectEditor.h"

#include <complex>
#include <qstackedwidget.h>

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

ProjectEditor::~ProjectEditor() { delete ui; }

void ProjectEditor::commonSetUp() {
	towerEditor = std::make_unique<TowerEditor>(projectController->getTowerController());
	enemyEditor = std::make_unique<EnemyEditor>(projectController->getEnemyController());
	mapEditor = std::make_unique<MapEditor>(projectController->getMapController());

	ui->stackedWidget = new QStackedWidget(this);
	setCentralWidget(ui->stackedWidget);

	ui->stackedWidget->addWidget(towerEditor.get());
	ui->stackedWidget->addWidget(enemyEditor.get());
	ui->stackedWidget->addWidget(mapEditor.get());


	connect(ui->actionTower_editor, &QAction::triggered, this, &ProjectEditor::openTowerEditor);
	connect(ui->actionEnemy_editor, &QAction::triggered, this, &ProjectEditor::openEnemyEditor);
	connect(ui->actionMap_editor, &QAction::triggered, this, &ProjectEditor::openMapEditor);
	connect(ui->actionSave_project, &QAction::triggered, this, &ProjectEditor::onSaveProjectClicked);
}

void ProjectEditor::openTowerEditor() {
	qDebug() << "opening tower editor " << towerEditor->metaObject();
	ui->stackedWidget->setCurrentWidget(towerEditor.get());
}

void ProjectEditor::openEnemyEditor() {
	qDebug() << "opening enemy editor " << enemyEditor->metaObject();
	ui->stackedWidget->setCurrentWidget(enemyEditor.get());
}

void ProjectEditor::openMapEditor() {
	qDebug() << "opening map editor " << mapEditor->metaObject();
	ui->stackedWidget->setCurrentWidget(mapEditor.get());
}

void ProjectEditor::onSaveProjectClicked() {
	if (projectController->saveProject()) {
		qDebug() << "saving project";
		return;
	}
	qDebug() << "failed to save project";
}
