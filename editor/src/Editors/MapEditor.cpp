#include "MapEditor.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <qimagereader.h>
#include <qinputdialog.h>
#include <qspinbox.h>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <algorithm>

#include "Map.h"
#include "ProjectController.h"
#include "Team.h"
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

	connect(ui->enableOnlineCheckBox, &QCheckBox::toggled, this, &MapEditor::onEnableOnlineCheckBoxToggled);
	connect(ui->maxPlayersSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MapEditor::onMaxPlayersSpinBoxChanged);
	connect(ui->saveOnlineSettingsButton, &QPushButton::clicked, this, &MapEditor::onSaveOnlineSettingsButtonClicked);
	connect(ui->addTeamButton, &QPushButton::clicked, this, &MapEditor::onAddTeamButtonClicked);

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
	qDebug() << "map name: " << QString::fromStdString(mapName);
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
	qDebug() << "mode: " << QString::fromStdString(mode);
	if (mode == "Tiles") {
		ui->editorStack->setCurrentIndex(0);
	} else if (mode == "Waves & Paths") {
		ui->editorStack->setCurrentIndex(1);
	} else if (mode == "Spots") {
		ui->editorStack->setCurrentIndex(2);
	} else if (mode == "Settings") {
		ui->editorStack->setCurrentIndex(3);
		fillPropertiesForm();
	} else if (mode == "Online options") {
		ui->editorStack->setCurrentIndex(4);
		
		auto currentMap = mapController->getCurrentMap();
		if (currentMap) {
			bool onlineEnabled = currentMap->isOnlineEnabled();
			int maxPlayers = currentMap->getMaxPlayers();
			
			ui->enableOnlineCheckBox->blockSignals(true);
			ui->enableOnlineCheckBox->setChecked(onlineEnabled);
			ui->enableOnlineCheckBox->blockSignals(false);
			
			ui->maxPlayersSpinBox->blockSignals(true);
			ui->maxPlayersSpinBox->setValue(maxPlayers);
			ui->maxPlayersSpinBox->blockSignals(false);

			ui->maxPlayersSpinBox->setEnabled(onlineEnabled);
			ui->teamsLabel->setEnabled(onlineEnabled);
			ui->teamAssignmentContainer->setEnabled(onlineEnabled);
			updateAddTeamButtonState();
			ui->playerSpotsLabel->setEnabled(onlineEnabled);
			ui->playerSpotsScrollArea->setEnabled(onlineEnabled);
			
			if (onlineEnabled) {
				onMaxPlayersSpinBoxChanged(maxPlayers);
				
				const auto &playerSpots = currentMap->getPlayerSpots();
				for (int playerIdx = 0; playerIdx < playerSpots.size() && playerIdx < playerSpotCheckboxes.size(); ++playerIdx) {
					for (int spotIdx = 0; spotIdx < playerSpotCheckboxes[playerIdx].size(); ++spotIdx) {
						if (spotIdx < currentMap->getSpots().size()) {
							const auto &spotName = currentMap->getSpots()[spotIdx]->getName();
							bool isSelected = std::find(playerSpots[playerIdx].begin(), 
													   playerSpots[playerIdx].end(), 
													   spotName) != playerSpots[playerIdx].end();
							playerSpotCheckboxes[playerIdx][spotIdx]->setChecked(isSelected);
						}
					}
				}
			} else {
				rebuildPlayerTeamAssignmentRows();
			}
		}
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
	qDebug() << "deleting wave " << QString::fromStdString(currentWaveName);
	if (currentWaveName == "") {
		return;
	}
	mapController->removeWave(currentWaveName);

	updateWaveList();
}

void MapEditor::onWaveItemClicked(QListWidgetItem *item) {
	auto waveName = item->text().toStdString();
	qDebug() << "wave " << QString::fromStdString(waveName);

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
	const auto towerNames = mapController->getAvailableTowers();
	if (towerNames.empty()) {
		QMessageBox::warning(this, tr("Error"),
							tr("Create at least one tower in the Tower editor before placing map spots."));
		return;
	}

	std::string baseName = "spot";
	int counter = 1;
	while (mapController->spotExist(baseName)) {
		baseName = "spot_" + std::to_string(counter);
		counter++;
	}

	QDialog dialog(this);
	dialog.setWindowTitle(tr("Add spot"));

	auto *layout = new QVBoxLayout(&dialog);

	auto *towerLabel = new QLabel(tr("Tower (from project):"), &dialog);
	auto *towerCombo = new QComboBox(&dialog);
	for (const auto &tn : towerNames) {
		towerCombo->addItem(QString::fromStdString(tn));
	}
	layout->addWidget(towerLabel);
	layout->addWidget(towerCombo);

	auto *xLayout = new QHBoxLayout();
	auto *xLabel = new QLabel(tr("X (tile):"), &dialog);
	auto *xSpin = new QSpinBox(&dialog);
	xSpin->setRange(0, currentMap->getWidth() - 1);
	xLayout->addWidget(xLabel);
	xLayout->addWidget(xSpin);
	layout->addLayout(xLayout);

	auto *yLayout = new QHBoxLayout();
	auto *yLabel = new QLabel(tr("Y (tile):"), &dialog);
	auto *ySpin = new QSpinBox(&dialog);
	ySpin->setRange(0, currentMap->getHeight() - 1);
	yLayout->addWidget(yLabel);
	yLayout->addWidget(ySpin);
	layout->addLayout(yLayout);

	auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
	connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
	layout->addWidget(buttonBox);

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	const int x = xSpin->value();
	const int y = ySpin->value();
	const std::string templateName = towerCombo->currentText().toStdString();

	if (y < 0 || y >= currentMap->getHeight() || x < 0 || x >= currentMap->getWidth()) {
		return;
	}
	const auto &spots = currentMap->getSpots();
	const auto it = std::find_if(spots.begin(), spots.end(), [x, y](const std::shared_ptr<TowerSample> &s) {
		return s->getX() == x && s->getY() == y;
	});
	if (it != spots.end()) {
		QMessageBox::warning(this, "Error", "Spot already exists at this position!");
		return;
	}

	if (!mapController->addSpot(baseName, templateName, x, y)) {
		QMessageBox::warning(this, tr("Error"), tr("Tower template not found."));
		return;
	}
	refreshMapView();
	updateSpotList();
	qDebug() << "Added spot at (" << x << "," << y << ") instance" << QString::fromStdString(baseName)
			<< "tower" << QString::fromStdString(templateName);
}

void MapEditor::updateSpotList() {
	ui->spotsList->clear();

	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	for (const auto &spot: currentMap->getSpots()) {
		QString text = QString::fromStdString(spot->getName());
		if (spot->isMapSpotReference()) {
			text += QStringLiteral(" [") + QString::fromStdString(spot->getTowerTemplateName()) + QStringLiteral("]");
		}
		auto *listItem = new QListWidgetItem(text);
		listItem->setData(Qt::UserRole, QString::fromStdString(spot->getName()));
		ui->spotsList->addItem(listItem);
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
	if (!spot) {
		QMessageBox::warning(this, tr("Error"), tr("Spot not found (internal id). Reselect the spot in the list."));
		return;
	}
	int currentX = spot->getX();
	int currentY = spot->getY();

	if (spot->isMapSpotReference()) {
		QDialog refDialog(this);
		refDialog.setWindowTitle(tr("Edit map spot"));
		refDialog.resize(420, 320);
		auto *layout = new QVBoxLayout(&refDialog);

		auto *info = new QLabel(
			tr("This spot references a tower from the project. Stats come from the Tower editor; here you can change position and which tower is used."),
			&refDialog);
		info->setWordWrap(true);
		layout->addWidget(info);

		auto *xLayout = new QHBoxLayout();
		auto *xLabel = new QLabel(tr("X (tile):"), &refDialog);
		auto *xSpin = new QSpinBox(&refDialog);
		xSpin->setRange(0, mapController->getCurrentMap()->getWidth() - 1);
		xSpin->setValue(currentX);
		xLayout->addWidget(xLabel);
		xLayout->addWidget(xSpin);
		layout->addLayout(xLayout);

		auto *yLayout = new QHBoxLayout();
		auto *yLabel = new QLabel(tr("Y (tile):"), &refDialog);
		auto *ySpin = new QSpinBox(&refDialog);
		ySpin->setRange(0, mapController->getCurrentMap()->getHeight() - 1);
		ySpin->setValue(currentY);
		yLayout->addWidget(yLabel);
		yLayout->addWidget(ySpin);
		layout->addLayout(yLayout);

		auto *towerLabel = new QLabel(tr("Tower:"), &refDialog);
		auto *towerCombo = new QComboBox(&refDialog);
		const auto names = mapController->getAvailableTowers();
		for (size_t i = 0; i < names.size(); ++i) {
			towerCombo->addItem(QString::fromStdString(names[i]));
			if (names[i] == spot->getTowerTemplateName()) {
				towerCombo->setCurrentIndex(static_cast<int>(i));
			}
		}
		layout->addWidget(towerLabel);
		layout->addWidget(towerCombo);

		auto *bb = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &refDialog);
		connect(bb, &QDialogButtonBox::accepted, &refDialog, &QDialog::accept);
		connect(bb, &QDialogButtonBox::rejected, &refDialog, &QDialog::reject);
		layout->addWidget(bb);

		if (refDialog.exec() != QDialog::Accepted) {
			return;
		}

		const int newX = xSpin->value();
		const int newY = ySpin->value();
		const auto &spotsCheck = mapController->getCurrentMap()->getSpots();
		const auto clash = std::find_if(spotsCheck.begin(), spotsCheck.end(),
										[newX, newY, this](const std::shared_ptr<TowerSample> &s) {
											return s->getX() == newX && s->getY() == newY && s->getName() != currentSpot;
										});
		if (clash != spotsCheck.end()) {
			QMessageBox::warning(this, "Error", "Spot already exists at this position!");
			return;
		}

		spot->setX(newX);
		spot->setY(newY);
		const std::string newTpl = towerCombo->currentText().toStdString();
		spot->setTowerTemplateName(newTpl);
		for (const auto &t : mapController->getProjectController()->getTowers()) {
			if (t->getName() == newTpl) {
				spot->applyTemplate(*t);
				break;
			}
		}

		refreshMapView();
		updateSpotList();
		return;
	}

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

		QString spotLabel = QString::fromStdString(spot->getName());
		if (spot->isMapSpotReference()) {
			spotLabel += QStringLiteral("\n[") + QString::fromStdString(spot->getTowerTemplateName()) + QStringLiteral("]");
		}
		QGraphicsTextItem *text = scene->addText(spotLabel);
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
	const QString id = item->data(Qt::UserRole).toString();
	currentSpot = id.isEmpty() ? item->text().toStdString() : id.toStdString();
	qDebug() << "spot id: " << QString::fromStdString(currentSpot);
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

	if (currentMap->isOnlineEnabled()) {
		for (auto &team : currentMap->getTeams()) {
			for (auto &player : team->getPlayers()) {
				{
					QString key = QString("%1_hp").arg(QString::fromStdString(player.getPlayerName()));
					QString label = QString("%1 HP:").arg(QString::fromStdString(player.getPlayerName()));

					auto *spin = new QDoubleSpinBox(this);
					spin->setRange(1.0, 10000.0);
					spin->setDecimals(1);
					spin->setValue(player.getHp());

					ui->propertiesForm->addRow(label, spin);
					m_propertyEditors[key] = spin;
				}

				{
					QString key = QString("%1_gold").arg(QString::fromStdString(player.getPlayerName()));
					QString label = QString("%1 Gold:")
						.arg(QString::fromStdString(player.getPlayerName()));
					auto *spin = new QDoubleSpinBox(this);
					spin->setRange(0.0, 100000.0);
					spin->setDecimals(0);
					spin->setValue(player.getStartCurrency());

					ui->propertiesForm->addRow(label, spin);
					m_propertyEditors[key] = spin;
				}

			}

		}
	} else {
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
}

void MapEditor::onSaveMapSettingsButtonClicked() {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	if (currentMap->isOnlineEnabled()) {

		for (auto &team : currentMap->getTeams()) {
			for (auto &player : team->getPlayers()) {

				QString name = QString::fromStdString(player.getPlayerName());

				// HP
				QString hpKey = name + "_hp";
				if (m_propertyEditors.contains(hpKey)) {
					if (auto *spin = qobject_cast<QDoubleSpinBox*>(m_propertyEditors[hpKey])) {
						player.setHp(spin->value());
					}
				}

				// GOLD
				QString goldKey = name + "_gold";
				if (m_propertyEditors.contains(goldKey)) {
					if (auto *spin = qobject_cast<QDoubleSpinBox*>(m_propertyEditors[goldKey])) {
						player.setStartCurrency(spin->value());
					}
				}
			}
		}
	} else {
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
}

void MapEditor::onEnableOnlineCheckBoxToggled(bool checked) {
	ui->maxPlayersSpinBox->setEnabled(checked);
	ui->teamsLabel->setEnabled(checked);
	ui->teamAssignmentContainer->setEnabled(checked);
	ui->playerSpotsLabel->setEnabled(checked);
	ui->playerSpotsScrollArea->setEnabled(checked);

	if (auto m = mapController->getCurrentMap()) {
		m->setOnlineEnabled(checked);
		if (checked) {
			m->syncOnlineTeamsWithPlayerCount(ui->maxPlayersSpinBox->value(), false);
		}
	}

	if (checked) {
		onMaxPlayersSpinBoxChanged(ui->maxPlayersSpinBox->value());
	} else {
		playerSpotCheckboxes.clear();
		QLayoutItem *item;
		while ((item = ui->playerSpotsContainer->layout()->takeAt(0)) != nullptr) {
			delete item->widget();
			delete item;
		}
		rebuildPlayerTeamAssignmentRows();
	}
	updateAddTeamButtonState();
}

void MapEditor::onMaxPlayersSpinBoxChanged(int value) {
	if (!ui->enableOnlineCheckBox->isChecked()) return;

	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	currentMap->syncOnlineTeamsWithPlayerCount(value, false);

	const auto &spots = currentMap->getSpots();
	
	playerSpotCheckboxes.clear();
	QLayoutItem *item;
	while ((item = ui->playerSpotsContainer->layout()->takeAt(0)) != nullptr) {
		delete item->widget();
		delete item;
	}

	playerSpotCheckboxes.resize(value);

	for (int playerIdx = 0; playerIdx < value; ++playerIdx) {
		QGroupBox *playerGroup = new QGroupBox(QString("Player %1 Spots").arg(playerIdx + 1));
		QVBoxLayout *playerLayout = new QVBoxLayout(playerGroup);

		playerSpotCheckboxes[playerIdx].resize(spots.size());

		for (int spotIdx = 0; spotIdx < spots.size(); ++spotIdx) {
			const auto &spot = spots[spotIdx];
			QString spotName = QString::fromStdString(spot->getName());

			QCheckBox *checkbox = new QCheckBox(spotName);
			playerLayout->addWidget(checkbox);
			playerSpotCheckboxes[playerIdx][spotIdx] = checkbox;
		}

		ui->playerSpotsContainer->layout()->addWidget(playerGroup);
	}

	auto layout = qobject_cast<QVBoxLayout*>(ui->playerSpotsContainer->layout());
	if (layout) {
		layout->addStretch();
	}

	rebuildPlayerTeamAssignmentRows();
}

void MapEditor::onSaveOnlineSettingsButtonClicked() {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap) return;

	bool onlineEnabled = ui->enableOnlineCheckBox->isChecked();
	int maxPlayers = ui->maxPlayersSpinBox->value();

	currentMap->setOnlineEnabled(onlineEnabled);
	currentMap->setMaxPlayers(maxPlayers);

	if (onlineEnabled) {
		currentMap->syncOnlineTeamsWithPlayerCount(maxPlayers, true);
		if (static_cast<int>(playerTeamCombos.size()) == maxPlayers) {
			std::vector<int> teamPick;
			teamPick.reserve(maxPlayers);
			for (int i = 0; i < maxPlayers; ++i) {
				teamPick.push_back(playerTeamCombos[static_cast<size_t>(i)]->currentIndex());
			}
			currentMap->applyPlayerTeamAssignments(teamPick);
		}
	}

	std::vector<std::vector<std::string>> playerSpots;
	playerSpots.resize(maxPlayers);

	for (int playerIdx = 0; playerIdx < maxPlayers; ++playerIdx) {
		for (int spotIdx = 0; spotIdx < playerSpotCheckboxes[playerIdx].size(); ++spotIdx) {
			if (playerSpotCheckboxes[playerIdx][spotIdx]->isChecked()) {
				const auto &allSpots = currentMap->getSpots();
				if (spotIdx < allSpots.size()) {
					playerSpots[playerIdx].push_back(allSpots[spotIdx]->getName());
				}
			}
		}
	}

	currentMap->setPlayerSpots(playerSpots);

	QMessageBox::information(this, "Success", 
		QString("Online settings saved!\nOnline: %1\nMax Players: %2")
			.arg(onlineEnabled ? "Enabled" : "Disabled")
			.arg(maxPlayers));

	qDebug() << "Online settings saved for map:" << QString::fromStdString(currentMap->getName());
}

void MapEditor::onAddTeamButtonClicked() {
	auto m = mapController->getCurrentMap();
	if (!m || !ui->enableOnlineCheckBox->isChecked()) {
		return;
	}
	if (static_cast<int>(m->getTeams().size()) >= Map::getMaxOnlineTeams()) {
		return;
	}
	const int next = static_cast<int>(m->getTeams().size()) + 1;
	m->getTeams().push_back(std::make_shared<Team>("Team " + std::to_string(next)));
	rebuildPlayerTeamAssignmentRows();
}

void MapEditor::rebuildPlayerTeamAssignmentRows() {
	QLayoutItem *it;
	while ((it = ui->teamAssignmentLayout->takeAt(0)) != nullptr) {
		if (it->widget() != nullptr) {
			delete it->widget();
		}
		delete it;
	}
	playerTeamCombos.clear();

	auto currentMap = mapController->getCurrentMap();
	if (!currentMap || !ui->enableOnlineCheckBox->isChecked()) {
		return;
	}

	const int n = ui->maxPlayersSpinBox->value();
	const auto &tlist = currentMap->getTeams();
	for (int i = 0; i < n; ++i) {
		auto *row = new QWidget(ui->teamAssignmentContainer);
		auto *hl = new QHBoxLayout(row);
		hl->setContentsMargins(0, 0, 0, 0);
		hl->addWidget(new QLabel(QStringLiteral("Player %1").arg(i + 1), row));
		auto *combo = new QComboBox(row);
		for (const auto &team : tlist) {
			combo->addItem(QString::fromStdString(team->getTeamName()));
		}
		if (combo->count() > 0) {
			const std::string pid = "player_" + std::to_string(i + 1);
			const int ti = currentMap->findTeamIndexForPlayer(pid);
			const int maxIdx = combo->count() - 1;
			combo->setCurrentIndex(std::clamp(ti, 0, maxIdx));
		}
		hl->addWidget(combo, 1);
		ui->teamAssignmentLayout->addWidget(row);
		playerTeamCombos.push_back(combo);
	}

	updateAddTeamButtonState();
}

void MapEditor::updateAddTeamButtonState() {
	auto *m = mapController->getCurrentMap().get();
	const bool online = ui->enableOnlineCheckBox->isChecked();
	const bool canAdd = online && m != nullptr && static_cast<int>(m->getTeams().size()) < Map::getMaxOnlineTeams();
	ui->addTeamButton->setEnabled(canAdd);
}
