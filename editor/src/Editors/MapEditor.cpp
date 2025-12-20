#include "MapEditor.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <qimagereader.h>
#include <qinputdialog.h>
#include <qspinbox.h>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

#include "ProjectController.h"
#include "TextureManager.h"
#include "TowerController.h"
#include "WaveEditor.h"
#include "ui_MapEditor.h"

MapEditor::MapEditor(const std::shared_ptr<MapController> &mapController, QWidget *parent) : QWidget(parent),
	ui(new Ui::MapEditor), mapController(mapController) {
	ui->setupUi(this);

	connect(ui->addMapButton, &QPushButton::clicked, this, &MapEditor::onAddMapButtonClicked);
	connect(ui->mapComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MapEditor::onMapItemClicked);
	connect(ui->addTextureButton, &QPushButton::clicked, this, &MapEditor::onAddTextureButtonClicked);
	connect(ui->textureList, &QListWidget::itemClicked, this, &MapEditor::onTextureItemClicked);
	connect(ui->deleteMapButton, &QPushButton::clicked, this, &MapEditor::onDeleteMapButtonClicked);
	connect(ui->modeList, &QListWidget::itemClicked, this, &MapEditor::onModeItemClicked);

	connect(ui->addWaveButton, &QPushButton::clicked, this, &MapEditor::onAddWaveButtonClicked);
	connect(ui->deleteWaveButton, &QPushButton::clicked, this, &MapEditor::onDeleteWaveButtonClicked);
	connect(ui->wavesList, &QListWidget::itemClicked, this, &MapEditor::onWaveItemClicked);
	connect(ui->editWaveButton, &QPushButton::clicked, this, &MapEditor::onEditWaveButtonClicked);
	connect(ui->saveSettingsButton, &QPushButton::clicked, this, &MapEditor::onSaveMapSettingsButtonClicked);

	connect(ui->addSpotButton, &QPushButton::clicked, this, &MapEditor::onAddSpotButtonClicked);
	connect(ui->removeSpotButton, &QPushButton::clicked, this, &MapEditor::onRemoveSpotButtonClicked);
	connect(ui->spotsList, &QListWidget::itemClicked, this, &MapEditor::onSpotItemClicked);
	connect(ui->editSpotButton, &QPushButton::clicked, this, &MapEditor::onEditSpotButtonClicked);

	connect(ui->editCurrentWavePathButton, &QPushButton::clicked, this, &MapEditor::onEditPathButtonClicked);

	ui->editorStack->setCurrentIndex(0);
	ui->modeList->setCurrentRow(0);

	updateMapList();
	updateTextureList();
	updateWaveList();
	updateSpotList();
}

MapEditor::~MapEditor() { delete ui; }

void MapEditor::onAddMapButtonClicked() {
	QDialog dialog(this);
	dialog.setWindowTitle("Add New Map");

	QVBoxLayout *mainLayout = new QVBoxLayout(&dialog);

	QHBoxLayout *nameLayout = new QHBoxLayout();
	QLabel *nameLabel = new QLabel("Map Name:");
	QLineEdit *nameEdit = new QLineEdit();
	nameLayout->addWidget(nameLabel);
	nameLayout->addWidget(nameEdit);
	mainLayout->addLayout(nameLayout);

	QHBoxLayout *xLayout = new QHBoxLayout();
	QLabel *xLabel = new QLabel("Width (X) (max 40):");
	QSpinBox *xSpin = new QSpinBox();
	xSpin->setRange(10, 40);
	xLayout->addWidget(xLabel);
	xLayout->addWidget(xSpin);
	mainLayout->addLayout(xLayout);

	QHBoxLayout *yLayout = new QHBoxLayout();
	QLabel *yLabel = new QLabel("Height (Y) (max 20):");
	QSpinBox *ySpin = new QSpinBox();
	ySpin->setRange(10, 20);
	yLayout->addWidget(yLabel);
	yLayout->addWidget(ySpin);
	mainLayout->addLayout(yLayout);

	QHBoxLayout *buttonsLayout = new QHBoxLayout();
	QPushButton *okButton = new QPushButton("OK");
	QPushButton *cancelButton = new QPushButton("Cancel");
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(okButton);
	buttonsLayout->addWidget(cancelButton);
	mainLayout->addLayout(buttonsLayout);

	connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
	connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

	if (dialog.exec() == QDialog::Accepted) {
		QString name = nameEdit->text();
		int width = xSpin->value();
		int height = ySpin->value();

		if (name.isEmpty()) {
			QMessageBox::warning(this, "Error", "Map name cannot be empty");
			return;
		}

		if (mapController->mapExists(name.toStdString())) {
			QMessageBox::warning(this, "Error", ("Map with name " + name.toStdString() + " already exists").data());
		}
		mapController->addMap(name.toStdString(), height, width);

		updateMapList();
	}
}

void MapEditor::onMapItemClicked(const int index) {
	if (index < 0) {
		return;
	}
	const std::string mapName = ui->mapComboBox->currentText().toStdString();
	qDebug() << "map name: " << mapName;
	qDebug() << "item clicked";
	mapController->setCurrentMap(mapName);

	refreshMapView();
	updateTextureList();
	updateWaveList();
	updateSpotList();
	currentWaveName = "";
}

void MapEditor::onModeItemClicked(const QListWidgetItem *item) {
	const std::string mode = item->text().toStdString();
	qDebug() << "mode: " << mode;
	if (mode == "Tiles") {
		ui->editorStack->setCurrentIndex(0);
	} else if (mode == "Waves & Paths") {
		ui->editorStack->setCurrentIndex(1);
	} else if (mode == "Spots") {
		ui->editorStack->setCurrentIndex(2);
	} else if (mode == "Settings") {
		ui->editorStack->setCurrentIndex(3);
		fillPropertiesForm();
	}
}

void MapEditor::onAddTextureButtonClicked() {
	QString filePath = QFileDialog::getOpenFileName(this, tr("Add Texture"), QDir::currentPath(), ".png");

	if (filePath.isEmpty()) {
		return;
	}

	filePath = TextureUtils::returnRelativeOrAbsolutePath(filePath);

	QImageReader reader(filePath);
	if (!reader.canRead()) {
		QMessageBox::warning(this, tr("Error"), tr("Cannot read file:\n%1").arg(filePath));
		return;
	}

	QSize size = reader.size();
	if (!size.isValid()) {
		QMessageBox::warning(this, tr("Error"), tr("Invalid or corrupted PNG file:\n%1").arg(filePath));
		return;
	}

	if (const int REQUIRED_SIZE = TextureManager::instance().getImageSize();
		size.width() != REQUIRED_SIZE || size.height() != REQUIRED_SIZE) {
		QMessageBox::warning(this, tr("Wrong Size"),
							tr("Texture must be exactly %4×%4 pixels!\n"
								"Current size: %1×%2\n"
								"File: %3")
							.arg(size.width())
							.arg(size.height())
							.arg(QFileInfo(filePath).fileName())
							.arg(REQUIRED_SIZE));
		return;
	}

	QPixmap pixmap(filePath);
	if (pixmap.isNull()) {
		QMessageBox::warning(this, tr("Error"), tr("Failed to load image (corrupted or not PNG)"));
		return;
	}

	int newId = 1;
	const auto &allTextures = TextureManager::instance().getAllTextures();
	if (!allTextures.empty()) {
		auto maxIt = std::max_element(allTextures.begin(), allTextures.end(),
									[](const auto &a, const auto &b) { return a.first < b.first; });
		newId = maxIt->first + 1;
	}

	TextureManager::instance().registerTexture(newId, filePath.toStdString());

	updateTextureList();

	QMessageBox::information(this, tr("Success"), tr("Texture added successfully!\nID: %1\nSize: 32×32").arg(newId));
}

void MapEditor::onDeleteMapButtonClicked() {
	const auto map = mapController->getCurrentMap();
	if (!map) {
		return;
	}

	mapController->removeMap(map->getName());

	updateMapList();
}

void MapEditor::onAddWaveButtonClicked() {
	std::string baseName = "wave";
	int counter = 1;
	while (mapController->waveExists(baseName)) {
		baseName = "wave_" + std::to_string(counter);
		counter++;
	}
	mapController->addWave(baseName);
	updateWaveList();
}

void MapEditor::onDeleteWaveButtonClicked() {
	qDebug() << "deleting wave " << currentWaveName;
	if (currentWaveName == "") {
		return;
	}
	mapController->removeWave(currentWaveName);

	updateWaveList();
}

void MapEditor::onWaveItemClicked(QListWidgetItem *item) {
	auto waveName = item->text().toStdString();
	qDebug() << "wave " << waveName;

	if (waveName != "") {
		currentWaveName = waveName;
	}

	refreshMapView();

	if (!pathEditingMode) {
		drawCurrentWavePath();
	}
}

void MapEditor::onEditWaveButtonClicked() {
	if (currentWaveName.empty()) {
		QMessageBox::information(this, "No Wave Selected", "Please select a wave from the list first!");
		return;
	}

	WaveEditor dialog(mapController, currentWaveName, this);

	if (dialog.exec() == QDialog::Accepted) {
		updateWaveList();
		refreshMapView();
		qDebug() << "Wave" << QString::fromStdString(currentWaveName) << "was successfully edited";
	}
}

void MapEditor::onAddSpotButtonClicked() {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) {
		QMessageBox::warning(this, "Error", "No map selected!");
		return;
	}
	std::string baseName = "spot";
	int counter = 1;

	while (mapController->spotExist(baseName)) {
		baseName = "spot_" + std::to_string(counter);
		counter++;
	}

	QDialog dialog(this);
	dialog.setWindowTitle("Add Spot");

	QVBoxLayout *layout = new QVBoxLayout(&dialog);

	QHBoxLayout *xLayout = new QHBoxLayout();
	QLabel *xLabel = new QLabel("X (tile):");
	QSpinBox *xSpin = new QSpinBox();
	xSpin->setRange(0, currentMap->getWidth() - 1);
	xLayout->addWidget(xLabel);
	xLayout->addWidget(xSpin);
	layout->addLayout(xLayout);

	QHBoxLayout *yLayout = new QHBoxLayout();
	QLabel *yLabel = new QLabel("Y (tile):");
	QSpinBox *ySpin = new QSpinBox();
	ySpin->setRange(0, currentMap->getHeight() - 1);
	yLayout->addWidget(yLabel);
	yLayout->addWidget(ySpin);
	layout->addLayout(yLayout);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	layout->addWidget(buttonBox);

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	int x = xSpin->value();
	int y = ySpin->value();

	if (y < 0 || y >= currentMap->getHeight() || x < 0 || x >= currentMap->getWidth()) {
		return;
	}
	const auto &spots = currentMap->getSpots();
	auto it = std::find_if(spots.begin(), spots.end(),
							[x, y, this](const std::shared_ptr<TowerSample> &s) {
								return s->getX() == x && s->getY() == y && s->getName() != currentWaveName;
							});

	if (it != spots.end()) {
		QMessageBox::warning(this, "Error", "Spot already exists at this position!");
		return;
	}

	mapController->addSpot(baseName, x, y);
	refreshMapView();
	updateSpotList();
	qDebug() << "Added spot at (" << x << "," << y << ") with tower:" << QString::fromStdString(baseName);
}

void MapEditor::updateSpotList() {
	ui->spotsList->clear();

	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	for (const auto &spot: currentMap->getSpots()) {
		QString text = QString::fromStdString(spot->getName());
		ui->spotsList->addItem(text);
	}
}

void MapEditor::onRemoveSpotButtonClicked() {
	if (currentSpot.empty()) {
		return;
	}
	mapController->removeSpot(currentSpot);
	refreshMapView();
	updateSpotList();
}

void MapEditor::onEditSpotButtonClicked() {
	if (currentSpot.empty()) {
		QMessageBox::warning(this, "Error", "No spot selected!");
		return;
	}

	auto spot = mapController->getSpot(currentSpot);
	int currentX = spot->getX();
	int currentY = spot->getY();

	QDialog dialog(this);
	dialog.setWindowTitle("Edit Spot");
	dialog.resize(500, 600);

	QVBoxLayout *layout = new QVBoxLayout(&dialog);

	// X
	QHBoxLayout *xLayout = new QHBoxLayout();
	QLabel *xLabel = new QLabel("X (tile):");
	QSpinBox *xSpin = new QSpinBox();
	xSpin->setRange(0, mapController->getCurrentMap()->getWidth() - 1);
	xSpin->setValue(currentX);
	xLayout->addWidget(xLabel);
	xLayout->addWidget(xSpin);
	layout->addLayout(xLayout);

	// Y
	QHBoxLayout *yLayout = new QHBoxLayout();
	QLabel *yLabel = new QLabel("Y (tile):");
	QSpinBox *ySpin = new QSpinBox();
	ySpin->setRange(0, mapController->getCurrentMap()->getHeight() - 1);
	ySpin->setValue(currentY);
	yLayout->addWidget(yLabel);
	yLayout->addWidget(ySpin);
	layout->addLayout(yLayout);

	QGroupBox *textureGroup = new QGroupBox("Tower Texture");
	QVBoxLayout *textureLayout = new QVBoxLayout(textureGroup);

	QLabel *texturePreview = new QLabel("No preview");
	texturePreview->setAlignment(Qt::AlignCenter);
	texturePreview->setMinimumSize(TextureManager::instance().getImageSize(),
									TextureManager::instance().getImageSize());
	texturePreview->setStyleSheet("border: 1px solid gray; background: white;");
	textureLayout->addWidget(texturePreview);

	QPushButton *chooseTextureBtn = new QPushButton("Choose Tower Texture");
	textureLayout->addWidget(chooseTextureBtn);

	layout->addWidget(textureGroup);

	std::string currentTexturePath = spot->getTowerTexturePath();
	if (!currentTexturePath.empty()) {
		QPixmap pix(QString::fromStdString(currentTexturePath));
		if (!pix.isNull()) {
			texturePreview->
					setPixmap(pix.scaled(texturePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		} else {
			texturePreview->setText("Invalid image");
		}
	}

	const auto allTowerNames = mapController->getAvailableTowers();;


	QGroupBox *upgradesGroup = new QGroupBox("Upgrade Paths");
	QVBoxLayout *upgradesLayout = new QVBoxLayout(upgradesGroup);

	auto currentUpgrades = mapController->getSpot(currentSpot)->getUpgradeNames();
	QListWidget *upgradesList = new QListWidget();
	for (const auto &name: currentUpgrades) {
		upgradesList->addItem(QString::fromStdString(name));
	}
	upgradesLayout->addWidget(upgradesList);

	QHBoxLayout *upgradeButtons = new QHBoxLayout();
	QPushButton *addUpgradeBtn = new QPushButton("Add Upgrade");
	QPushButton *removeUpgradeBtn = new QPushButton("Remove");
	upgradeButtons->addWidget(addUpgradeBtn);
	upgradeButtons->addWidget(removeUpgradeBtn);
	upgradesLayout->addLayout(upgradeButtons);

	layout->addWidget(upgradesGroup);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	layout->addWidget(buttonBox);

	connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	connect(addUpgradeBtn, &QPushButton::clicked, this, [allTowerNames, upgradesList, &dialog]() {
		QStringList available;
		for (const auto &name: allTowerNames) {
			std::string n = name;
			bool alreadyAdded = false;
			for (int i = 0; i < upgradesList->count(); ++i) {
				if (upgradesList->item(i)->text().toStdString() == n) {
					alreadyAdded = true;
					break;
				}
			}
			if (!alreadyAdded) {
				available << QString::fromStdString(n);
			}
		}

		if (available.isEmpty()) {
			QMessageBox::information(&dialog, "No Upgrades", "No available towers.");
			return;
		}

		bool ok;
		QString selected = QInputDialog::getItem(&dialog, "Add Upgrade", "Choose tower:", available, 0, false, &ok);
		if (ok && !selected.isEmpty()) {
			upgradesList->addItem(selected);
		}
	});

	connect(chooseTextureBtn, &QPushButton::clicked, this, [&currentTexturePath, &texturePreview, &dialog]() {
		QString filePath = QFileDialog::getOpenFileName(&dialog, "Choose Tower Texture", QDir::currentPath(), "*.png");
		if (filePath.isEmpty()) {
			return;
		}

		QPixmap pix(filePath);
		if (pix.isNull()) {
			QMessageBox::warning(&dialog, "Error", "Invalid image file!");
			return;
		}

		texturePreview->setPixmap(pix.scaled(texturePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

		currentTexturePath = filePath.toStdString();
	});

	connect(removeUpgradeBtn, &QPushButton::clicked, this, [=]() {
		auto *item = upgradesList->currentItem();
		if (item) {
			delete item;
		}
	});


	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	int newX = xSpin->value();
	int newY = ySpin->value();
	auto list = upgradesList->selectedItems();

	const auto &spots = mapController->getCurrentMap()->getSpots();
	auto it = std::find_if(spots.begin(), spots.end(),
							[newX, newY, this](const std::shared_ptr<TowerSample> &s) {
								return s->getX() == newX && s->getY() == newY && s->getName() != currentSpot;
							});

	if (it != spots.end()) {
		QMessageBox::warning(this, "Error", "Spot already exists at this position!");
		return;
	}

	spot->setX(newX);
	spot->setY(newY);

	spot->setTowerTexturePath(currentTexturePath);

	json upgradeArray = json::array();

	for (int i = 0; i < upgradesList->count(); ++i) {
		QListWidgetItem *item = upgradesList->item(i);
		upgradeArray.push_back(item->text().toStdString());
	}

	json updateJson = {{"nextUpgrade", upgradeArray}};

	mapController->getSpot(currentSpot)->fromJson(updateJson);

	refreshMapView();
	updateSpotList();

	qDebug() << "Spot edited: (" << newX << "," << newY << ") ";
}

void MapEditor::updateMapList() const {
	qDebug() << "Map list updated";

	const auto mapNames = mapController->getMapNames();

	ui->mapComboBox->clear();
	for (const auto &name: mapNames) {
		ui->mapComboBox->addItem(QString::fromStdString(name));
	}
}

void MapEditor::updateWaveList() const {
	const auto waves = mapController->getWavesNames();

	fillListWidget(ui->wavesList, waves);
}

bool MapEditor::eventFilter(QObject *obj, QEvent *event) {
	if (obj == ui->mapView->scene() && event->type() == QEvent::GraphicsSceneMousePress) {
		auto *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);
		QPointF pos = mouseEvent->scenePos();

		int tileSize = TextureManager::instance().getImageSize();
		int tileX = static_cast<int>(pos.x()) / tileSize;
		int tileY = static_cast<int>(pos.y()) / tileSize;

		auto currentMap = mapController->getCurrentMap();
		if (!currentMap)
			return true;

		if (pathEditingMode) {
			if (mouseEvent->button() == Qt::LeftButton) {
				bool result = mapController->addPathPoint(currentWaveName, tileX, tileY);
				if (result) {
					for (auto *item: pathGraphicsItems) {
						ui->mapView->scene()->removeItem(item);
						delete item;
					}
					pathGraphicsItems.clear();
					drawCurrentWavePath();
				}
			} else if (mouseEvent->button() == Qt::RightButton) {
				bool result = mapController->deletePathPoint(currentWaveName);
				if (result) {
					for (auto *item: pathGraphicsItems) {
						ui->mapView->scene()->removeItem(item);
						delete item;
					}
					pathGraphicsItems.clear();
					drawCurrentWavePath();
				}
			}
			return true;
		}

		if (tileY < 0 || tileY >= currentMap->getHeight() || tileX < 0 || tileX >= currentMap->getWidth()) {
			return true;
		}

		int currentTextureId = TextureManager::instance().getCurrentTexture();
		if (currentTextureId == 0) {
			QMessageBox::information(this, "Warning", "No texture selected!");
			return true;
		}

		currentMap->setTile(tileX, tileY, currentTextureId);
		refreshMapView();
		return true;
	}

	return QWidget::eventFilter(obj, event);
}

void MapEditor::refreshMapView() {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	QGraphicsScene *scene = ui->mapView->scene();
	if (!scene) {
		scene = new QGraphicsScene(this);
		ui->mapView->setScene(scene);
		scene->installEventFilter(this);
	} else {
		scene->clear();
	}

	pathGraphicsItems.clear();

	const auto &tiles = currentMap->getTiles();
	int tileSize = TextureManager::instance().getImageSize();

	for (int y = 0; y < tiles.size(); ++y) {
		for (int x = 0; x < tiles[y].size(); ++x) {
			int tileId = tiles[y][x];
			QPixmap pix = TextureManager::instance().get(tileId);
			if (!pix.isNull()) {
				scene->addPixmap(pix)->setPos(x * tileSize, y * tileSize);
				scene->addRect(x * tileSize, y * tileSize, tileSize, tileSize,
								QPen(QColor(0, 0, 0, 50)));
			}
		}
	}
	for (const auto &spot: currentMap->getSpots()) {
		int x = spot->getX() * tileSize;
		int y = spot->getY() * tileSize;

		scene->addEllipse(x + tileSize / 6, y + tileSize / 6,
						tileSize * 2 / 3, tileSize * 2 / 3,
						QPen(Qt::blue, 4), QBrush(QColor(0, 0, 255, 80)));

		QGraphicsTextItem *text = scene->addText(QString::fromStdString(spot->getName()));
		text->setDefaultTextColor(Qt::white);
		QFont font = text->font();
		font.setBold(true);
		text->setFont(font);
		text->setPos(x + tileSize / 4, y + tileSize / 4);
		text->setZValue(101);
	}


	ui->mapView->setSceneRect(scene->itemsBoundingRect());
}

void MapEditor::onTextureItemClicked(QListWidgetItem *item) {
	if (!item) {
		return;
	}

	int textureId = item->data(Qt::UserRole).toInt();
	TextureManager::instance().setCurrentTexture(textureId);

	qDebug() << "Selected texture:" << textureId;
}

void MapEditor::onSpotItemClicked(QListWidgetItem *item) {
	if (!item) {
		return;
	}
	const std::string spotName = item->text().toStdString();
	qDebug() << "spot name: " << spotName;
	qDebug() << "item clicked";

	currentSpot = spotName;
}

void MapEditor::updateTextureList() {
	qDebug() << "Texture list updated";

	ui->textureList->clear();

	ui->textureList->setViewMode(QListWidget::IconMode);
	ui->textureList->setIconSize(QSize(48, 48));
	ui->textureList->setResizeMode(QListWidget::Adjust);
	ui->textureList->setMovement(QListWidget::Static);
	ui->textureList->setSpacing(8);
	ui->textureList->setFlow(QListWidget::LeftToRight);
	ui->textureList->setWrapping(true);

	ui->textureList->setStyleSheet(R"(
        QListWidget::item {
            text-align: center;
        }
        QListWidget::item:selected {
            background: rgba(100, 150, 255, 100);
            border: 2px solid #3377ff;
        }
    )");

	const auto &textures = TextureManager::instance().getAllTextures();

	for (const auto &[id, pathStr]: textures) {
		QString path = QString::fromStdString(pathStr);
		QString fileName = QFileInfo(path).fileName();

		QPixmap original = TextureManager::instance().get(id);
		if (original.isNull())
			continue;

		QPixmap scaled = original.scaled(48, 48, Qt::KeepAspectRatio, Qt::SmoothTransformation);

		QIcon icon(scaled);
		QString displayText = QString("%1\n%2").arg(id).arg(fileName);

		QListWidgetItem *item = new QListWidgetItem(icon, "");
		item->setData(Qt::UserRole, id);
		item->setToolTip(QString("ID: %1\n%2").arg(id).arg(QFileInfo(path).fileName()));

		ui->textureList->addItem(item);
	}

	if (ui->textureList->count() > 0 && ui->textureList->currentItem() == nullptr) {
		ui->textureList->setCurrentRow(0);
	}
}

// void MapEditor::onEditPathButtonClicked() {
// 	if (currentWaveName.empty()) {
// 		QMessageBox::information(this, "Error", "Select a wave first!");
// 		return;
// 	}
//
// 	pathEditingMode = !pathEditingMode;
// 	ui->editCurrentWavePathButton->setChecked(pathEditingMode);
// 	ui->editCurrentWavePathButton->setText(pathEditingMode ? "Finish Editing Path" : "Edit Path");
//
// 	// if (pathEditingMode) {
// 	// 	for (auto *item: pathGraphicsItems) {
// 	// 		ui->mapView->scene()->removeItem(item);
// 	// 		delete item;
// 	// 	}
// 	// 	pathGraphicsItems.clear();
// 	//
// 	// 	drawCurrentWavePath();
// 	// }
// 	if (pathEditingMode) {
// 		refreshMapView();
// 		drawCurrentWavePath();
// 	} else {
// 		refreshMapView();
// 	}
// }
void MapEditor::onEditPathButtonClicked() {
	if (currentWaveName.empty()) {
		QMessageBox::information(this, "Error", "Select a wave first!");
		return;
	}

	pathEditingMode = !pathEditingMode;
	ui->editCurrentWavePathButton->setChecked(pathEditingMode);
	ui->editCurrentWavePathButton->setText(pathEditingMode ? "Finish Editing Path" : "Edit Path");

	refreshMapView();

	// if (pathEditingMode) {
	drawCurrentWavePath();
	// }
}

// void MapEditor::drawCurrentWavePath() {
// 	auto currentMap = mapController->getCurrentMap();
// 	if (!currentMap) {
// 		return;
// 	}
//
// 	auto it = mapController->getWave(currentWaveName);
// 	if (it == nullptr) {
// 		return;
// 	}
//
// 	const auto &path = it->getPath();
// 	if (path.empty()) {
// 		return;
// 	}
//
// 	int tileSize = TextureManager::instance().getImageSize();
// 	QGraphicsScene *scene = ui->mapView->scene();
// 	if (!scene) {
// 		return;
// 	}
//
// 	for (size_t i = 0; i < path.size(); ++i) {
// 		int tx = path[i].first;
// 		int ty = path[i].second;
//
// 		QGraphicsEllipseItem *point = scene->addEllipse(tx * tileSize + tileSize / 4, ty * tileSize + tileSize / 4,
// 														tileSize / 2, tileSize / 2, QPen(Qt::red, 3), QBrush(Qt::red));
// 		point->setZValue(100);
// 		pathGraphicsItems.push_back(point);
//
// 		if (i < path.size() - 1) {
// 			int nextX = path[i + 1].first;
// 			int nextY = path[i + 1].second;
// 			QGraphicsLineItem *line =
// 					scene->addLine(tx * tileSize + tileSize / 2, ty * tileSize + tileSize / 2,
// 									nextX * tileSize + tileSize / 2, nextY * tileSize + tileSize / 2, QPen(Qt::red, 4));
// 			line->setZValue(99);
// 			pathGraphicsItems.push_back(line);
// 		}
// 	}
// }
void MapEditor::drawCurrentWavePath() {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	auto wave = mapController->getWave(currentWaveName);
	if (!wave) return;

	const auto &path = wave->getPath();
	if (path.empty()) return;

	int tileSize = TextureManager::instance().getImageSize();
	QGraphicsScene *scene = ui->mapView->scene();
	if (!scene) return;

	for (size_t i = 0; i < path.size(); ++i) {
		int tx = path[i].first;
		int ty = path[i].second;

		auto *point = scene->addEllipse(
			tx * tileSize + tileSize / 4, ty * tileSize + tileSize / 4,
			tileSize / 2, tileSize / 2,
			QPen(Qt::red, 3), QBrush(Qt::red));
		point->setZValue(100);
		pathGraphicsItems.push_back(point);

		if (i < path.size() - 1) {
			int nx = path[i + 1].first;
			int ny = path[i + 1].second;
			auto *line = scene->addLine(
				tx * tileSize + tileSize / 2, ty * tileSize + tileSize / 2,
				nx * tileSize + tileSize / 2, ny * tileSize + tileSize / 2,
				QPen(Qt::red, 4));
			line->setZValue(99);
			pathGraphicsItems.push_back(line);
		}
	}
}

void MapEditor::fillPropertiesForm() {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	BaseEditor::clearPropertiesForm(ui->propertiesForm, m_propertyEditors);

	{
		QString key = "hp";
		QString label = "HP:";

		QDoubleSpinBox *spin = new QDoubleSpinBox(this);
		spin->setRange(1.0, 10000.0);
		spin->setDecimals(1);
		spin->setValue(currentMap->getHp());

		ui->propertiesForm->addRow(label, spin);
		m_propertyEditors[key] = spin;
	}

	{
		QString key = "startCurrency";
		QString label = "Start Currency:";

		QDoubleSpinBox *spin = new QDoubleSpinBox(this);
		spin->setRange(0.0, 100000.0);
		spin->setDecimals(0);
		spin->setValue(currentMap->getStartCurrency());

		ui->propertiesForm->addRow(label, spin);
		m_propertyEditors[key] = spin;
	}
}

void MapEditor::onSaveMapSettingsButtonClicked() {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	if (m_propertyEditors.contains("hp")) {
		if (const auto *spin = qobject_cast<QDoubleSpinBox *>(m_propertyEditors["hp"])) {
			currentMap->setHp(spin->value());
		}
	}

	if (m_propertyEditors.contains("startCurrency")) {
		if (const auto *spin = qobject_cast<QDoubleSpinBox *>(m_propertyEditors["startCurrency"])) {
			currentMap->setStartCurrency(spin->value());
		}
	}

	qDebug() << "Map settings saved: HP =" << currentMap->getHp()
			<< "Start Currency =" << currentMap->getStartCurrency();
}
