#include "MapEditor.h"
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QMessageBox>
#include <qimagereader.h>
#include <qspinbox.h>
#include "TextureManager.h"
#include "ui_MapEditor.h"


MapEditor::MapEditor(const std::shared_ptr<MapController> &mapController, QWidget *parent) :
	QWidget(parent), ui(new Ui::MapEditor), mapController(mapController) {
	ui->setupUi(this);

	connect(ui->addMapButton, &QPushButton::clicked, this, &MapEditor::onAddMapButtonClicked);
	connect(ui->mapList, &QListWidget::itemClicked, this, &MapEditor::onMapItemClicked);
	connect(ui->addTextureButton, &QPushButton::clicked, this, &MapEditor::onAddTextureButtonClicked);
	connect(ui->textureList, &QListWidget::itemClicked, this, &MapEditor::onTextureItemClicked);

	updateMapList();
	updateTextureList();
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
void MapEditor::onMapItemClicked(const QListWidgetItem *item) {
	const std::string mapName = item->text().toStdString();
	qDebug() << "map name: " << mapName;
	qDebug() << "item clicked";
	mapController->setCurrentMap(mapName);

	auto currentMap = mapController->getCurrentMap();

	const auto &tiles = currentMap->getTiles();

	QGraphicsScene *scene = ui->mapView->scene();
	if (!scene) {
		scene = new QGraphicsScene(this);
		ui->mapView->setScene(scene);

		scene->installEventFilter(this);
	} else {
		scene->clear();
	}

	int imageSize = TextureManager::instance().getImageSize();

	for (int y = 0; y < tiles.size(); y++) {
		for (int x = 0; x < tiles[y].size(); x++) {
			int tileId = tiles[y][x];
			QPixmap pix = TextureManager::instance().get(tileId);
			if (!pix.isNull()) {
				QGraphicsPixmapItem *pixItem = scene->addPixmap(pix);
				pixItem->setPos(x * imageSize, y * imageSize);

				QGraphicsRectItem *rectItem =
						scene->addRect(x * imageSize, y * imageSize, imageSize, imageSize, QPen(Qt::black));
				rectItem->setZValue(1);
			}
		}
	}


	ui->mapView->setSceneRect(scene->itemsBoundingRect());
}

void MapEditor::onAddTextureButtonClicked() {
	QString filePath = QFileDialog::getOpenFileName(this, tr("Add Texture"), QDir::currentPath(), ".png");

	if (filePath.isEmpty()) {
		return;
	}

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

void MapEditor::updateMapList() const {
	qDebug() << "Map list updated";

	const auto mapNames = mapController->getMapNames();

	BaseEditor::fillListWidget(ui->mapList, mapNames);
}

bool MapEditor::eventFilter(QObject *obj, QEvent *event) {
	if (obj == ui->mapView->scene() && event->type() == QEvent::GraphicsSceneMousePress) {
		QGraphicsSceneMouseEvent *mouseEvent = static_cast<QGraphicsSceneMouseEvent *>(event);

		int tileSize = TextureManager::instance().getImageSize();
		int tileX = static_cast<int>(mouseEvent->scenePos().x()) / tileSize;
		int tileY = static_cast<int>(mouseEvent->scenePos().y()) / tileSize;

		auto currentMap = mapController->getCurrentMap();
		if (!currentMap)
			return true;

		const auto &tiles = currentMap->getTiles();
		if (tileY < 0 || tileY >= tiles.size() || tileX < 0 || tileX >= tiles[tileY].size()) {
			return true;
		}

		int currentTextureId = TextureManager::instance().getCurrentTexture();
		if (currentTextureId == 0) {
			QMessageBox::information(this, "Warning", "No texture selected!");
			return true;
		}

		currentMap->setTile(tileX, tileY, currentTextureId);

		onMapItemClicked(ui->mapList->currentItem());

		return true;
	}

	return QWidget::eventFilter(obj, event);
}

void MapEditor::onTextureItemClicked(QListWidgetItem *item) {
	if (!item)
		return;

	int textureId = item->data(Qt::UserRole).toInt();
	TextureManager::instance().setCurrentTexture(textureId);

	qDebug() << "Selected texture:" << textureId;
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
