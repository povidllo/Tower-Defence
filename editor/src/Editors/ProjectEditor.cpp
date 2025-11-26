#include "ProjectEditor.h"

#include <QPushButton>

#include "ui_ProjectEditor.h"


ProjectEditor::ProjectEditor(std::string path, std::string name, QWidget *parent) :
	QMainWindow(parent), ui(new Ui::ProjectEditor) {
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
	// towerEditor = new TowerEditor(projectController->getTowerController());
	// enemyEditor = new EnemyEditor(projectController->getEnemyController());
	container = new QWidget(this);
	QVBoxLayout *layout = new QVBoxLayout(container);
	container->setLayout(layout);
	layout->addWidget(towerEditor.get());
	layout->addWidget(enemyEditor.get());

	towerEditor->hide();
	enemyEditor->hide();

	setCentralWidget(container);


	connect(ui->actionTower_editor, &QAction::triggered, this, &ProjectEditor::openTowerEditor);
	connect(ui->actionEnemy_editor, &QAction::triggered, this, &ProjectEditor::openEnemyEditor);
}

void ProjectEditor::openTowerEditor() {
	qDebug() << "opening tower editor " << towerEditor->metaObject();
	// if (centralWidget()) {
	// 	centralWidget()->deleteLater();
	// }

	// setCentralWidget(towerEditor.get());
	// setCentralWidget(towerEditor);


	// ui->actionTower_editor->setChecked(true);

	towerEditor->show();
	enemyEditor->hide();
}

void ProjectEditor::openEnemyEditor() {
	qDebug() << "opening enemy editor " << enemyEditor->metaObject();

	// if (centralWidget()) {
	// 	centralWidget()->deleteLater();
	// }

	// setCentralWidget(enemyEditor.get());
	// setCentralWidget(enemyEditor);

	// ui->actionEnemy_editor->setChecked(true);
	enemyEditor->show();
	towerEditor->hide();
}
