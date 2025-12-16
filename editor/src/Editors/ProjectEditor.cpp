#include "ProjectEditor.h"

#include <complex>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <qstackedwidget.h>
#include <qjsonarray.h>
#include <qpainter.h>

#include "TextureManager.h"
#include "ui_ProjectEditor.h"

ProjectEditor::ProjectEditor(std::string path, std::string name, QWidget *parent) : QMainWindow(parent),
	ui(new Ui::ProjectEditor) {
	ui->setupUi(this);
	projectController = std::make_shared<ProjectController>(path, name);
	qDebug() << "created project";
	commonSetUp();
	connectQtElements();
}

ProjectEditor::ProjectEditor(std::string path, QWidget *parent) : QMainWindow(parent), ui(new Ui::ProjectEditor) {
	ui->setupUi(this);
	projectController = std::make_shared<ProjectController>(path);
	qDebug() << "created project";
	commonSetUp();
	connectQtElements();
}

ProjectEditor::~ProjectEditor() { delete ui; }

void ProjectEditor::connectQtElements() {
	connect(ui->actionTower_editor, &QAction::triggered, this, &ProjectEditor::openTowerEditor);
	connect(ui->actionEnemy_editor, &QAction::triggered, this, &ProjectEditor::openEnemyEditor);
	connect(ui->actionMap_editor, &QAction::triggered, this, &ProjectEditor::openMapEditor);
	connect(ui->actionSave_project, &QAction::triggered, this, &ProjectEditor::onSaveProjectClicked);
	connect(ui->actionCompile_project, &QAction::triggered, this, &ProjectEditor::compileProjectClicked);
	connect(ui->actionOpen_project, &QAction::triggered, this, &ProjectEditor::openProjectClicked);
}

void ProjectEditor::commonSetUp() {
	towerEditor = std::make_unique<TowerEditor>(projectController->getTowerController());
	enemyEditor = std::make_unique<EnemyEditor>(projectController->getEnemyController());
	mapEditor = std::make_unique<MapEditor>(projectController->getMapController());

	ui->stackedWidget = new QStackedWidget(this);
	setCentralWidget(ui->stackedWidget);

	ui->stackedWidget->addWidget(towerEditor.get());
	ui->stackedWidget->addWidget(enemyEditor.get());
	ui->stackedWidget->addWidget(mapEditor.get());

	QDir::setCurrent(projectController->getCurrentProject()->getPath().data());
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

void ProjectEditor::compileProject() {
	QString exportDir = QFileDialog::getExistingDirectory(
		this, "Select Export Folder", QDir::homePath()
	);
	if (exportDir.isEmpty()) {
		return;
	}

	QDir dir(exportDir);
	if (!dir.mkpath("resources")) {
		QMessageBox::critical(this, "Error", "Failed to create resources folder!");
		return;
	}

	QSet<QString> usedTextures;

	for (const auto &tower: projectController->getTowers()) {
		if (!tower->getTowerTexturePath().empty())
			usedTextures.insert(QString::fromStdString(tower->getTowerTexturePath()));
		if (!tower->getProjectileTexturePath().empty())
			usedTextures.insert(QString::fromStdString(tower->getProjectileTexturePath()));
	}

	for (const auto &enemy: projectController->getEnemies()) {
		if (!enemy->getEnemyTexturePath().empty()) {
			usedTextures.insert(QString::fromStdString(enemy->getEnemyTexturePath()));
		}
	}

	for (const QString &srcPath: usedTextures) {
		QFileInfo info(srcPath);
		QString destPath = dir.filePath("resources/" + info.fileName());
		if (QFile::exists(destPath)) QFile::remove(destPath);
		if (!QFile::copy(srcPath, destPath)) {
			qWarning() << "Failed to copy:" << srcPath;
		}
	}

	json compiledMaps = json::array();
	//
	for (const auto &map: projectController->getMaps()) {
		auto mapObj = map->toJson();
		const auto &tiles = map->getTiles();
		int tileSize = TextureManager::instance().getImageSize();
		int width = map->getWidth();
		int height = map->getHeight();

		QImage atlas(width * tileSize, height * tileSize, QImage::Format_ARGB32);
		atlas.fill(Qt::transparent);

		QPainter painter(&atlas);
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				int tileId = tiles[y][x];
				if (QPixmap tile = TextureManager::instance().get(tileId); !tile.isNull()) {
					painter.drawPixmap(x * tileSize, y * tileSize, tile);
				}
			}
		}
		painter.end();

		QString atlasName = QString("map_%1.png").arg(QString::fromStdString(map->getName()));
		QString atlasPath = dir.filePath(atlasName);
		if (!atlas.save(atlasPath, "PNG")) {
			QMessageBox::critical(this, "Error", "Failed to save map atlas: " + atlasName);
			return;
		}

		mapObj.erase("tiles");
		mapObj["finalMapImagePath"] = atlasName.toStdString();

		compiledMaps.push_back(std::move(mapObj));
	}
	json compiledTowers = json::array();
	for (const auto &tower: projectController->getTowers()) {
		json towerObj = tower->toJson();

		if (towerObj.contains("towerTexturePath")) {
			QString oldPath = QString::fromStdString(tower->getTowerTexturePath());
			QFileInfo info(oldPath);
			towerObj["towerTexturePath"] = ("resources/" + info.fileName()).toStdString();
		}
		if (towerObj.contains("projectileTexturePath")) {
			QString oldPath = QString::fromStdString(tower->getProjectileTexturePath());
			QFileInfo info(oldPath);
			towerObj["projectileTexturePath"] = ("resources/" + info.fileName()).toStdString();
		}

		compiledTowers.push_back(std::move(towerObj));
	}

	json compiledEnemies = json::array();
	for (const auto &enemy: projectController->getEnemies()) {
		json enemyObj = enemy->toJson();

		if (enemyObj.contains("enemyTexturePath")) {
			QString oldPath = QString::fromStdString(enemy->getEnemyTexturePath());
			QFileInfo info(oldPath);
			enemyObj["enemyTexturePath"] = ("resources/" + info.fileName()).toStdString();
		}

		compiledEnemies.push_back(std::move(enemyObj));
	}

	json finalProject = projectController->getCurrentProject()->toJson();
	finalProject["maps"] = compiledMaps;
	finalProject["towers"] = compiledTowers;
	finalProject["enemies"] = compiledEnemies;
	finalProject.erase("textures");

	QString jsonPath = dir.filePath("project.json");
	std::ofstream file(jsonPath.toStdString());
	if (file.is_open()) {
		file << finalProject.dump(4);
		file.close();

		QMessageBox::information(this, "Success",
								QString("Project compiled!\n%1 textures + %2 maps → %3")
								.arg(usedTextures.size())
								.arg(projectController->getMaps().size())
								.arg(exportDir));
	} else {
		QMessageBox::critical(this, "Error", "Failed to save project.json");
	}
}

void ProjectEditor::compileProjectClicked() {
	compileProject();
}

void ProjectEditor::openProjectClicked() {
	QString path = QFileDialog::getExistingDirectory(
		this,
		"Select Project Folder",
		"",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
	);
	try {
		projectController = std::make_shared<ProjectController>(path.toStdString());
	} catch (const std::filesystem::filesystem_error &e) {
		std::cerr << e.what() << std::endl;
		qDebug() << "Ooops, some mistakes with file system " << path.toStdString();
	}
	qDebug() << "created project";
	commonSetUp();
}
