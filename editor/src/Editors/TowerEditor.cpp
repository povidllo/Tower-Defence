#include "TowerEditor.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>

#include "ui_TowerEditor.h"

TowerEditor::TowerEditor(const std::shared_ptr<TowerController> &towerController, QWidget *parent) : QWidget(parent),
	ui(new Ui::TowerEditor), towerController(towerController) {
	ui->setupUi(this);

	connect(ui->addTowerButton, &QPushButton::clicked, this, &TowerEditor::addTower);
	connect(ui->towerList, &QListWidget::itemClicked, this, &TowerEditor::onItemClicked);
	connect(ui->saveButton, &QPushButton::clicked, this, &TowerEditor::onSaveButtonClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &TowerEditor::onDeleteButtonClicked);
	connect(ui->chooseTextureButton, &QPushButton::clicked, this, &TowerEditor::onChooseTextureButtonClicked);

	connect(ui->chooseTextureButton, &QPushButton::clicked, this, &TowerEditor::onChooseTextureButtonClicked);
	connect(ui->addNextTowerButton, &QPushButton::clicked, this, &TowerEditor::onAddNextUpgradeButtonClicked);
	connect(ui->removeNextTowerButton, &QPushButton::clicked, this, &TowerEditor::onRemoveNextUpgradeButtonClicked);

	updateTowerList();
	ui->towerPreview->setVisible(false);
	ui->chooseTextureButton->setVisible(false);

	ui->nextTowerList->setVisible(false);
	ui->addNextTowerButton->setVisible(false);
	ui->removeNextTowerButton->setVisible(false);
}

TowerEditor::~TowerEditor() { delete ui; }

void TowerEditor::addTower() const {
	qDebug() << "opening tower editor";
	std::string baseName = "tower";
	int counter = 1;
	while (towerController->towerExists(baseName)) {
		baseName = "tower_" + std::to_string(counter);
		counter++;
	}
	towerController->addTower(baseName);
	updateTowerList();
	updateUpgradeList();
}

void TowerEditor::onItemClicked(const QListWidgetItem *item) {
	const std::string towerName = item->text().toStdString();
	qDebug() << "tower name: " << towerName;
	qDebug() << "item clicked";
	towerController->setCurrentTower(towerName);

	auto currentTower = towerController->getCurrentTower();

	BaseEditor::clearPropertiesForm(ui->propertiesForm, m_propertyEditors);

	ui->towerPreview->setVisible(true);
	ui->chooseTextureButton->setVisible(true);

	ui->nextTowerList->setVisible(true);
	ui->addNextTowerButton->setVisible(true);
	ui->removeNextTowerButton->setVisible(true);

	fillPropertiesForm(currentTower);
}

void TowerEditor::onSaveButtonClicked() {
	const auto tower = towerController->getCurrentTower();
	if (!tower)
		return;

	nlohmann::json j = BaseEditor::collectPropertiesToJson(m_propertyEditors);

	if (tower->getName() != j["name"].get<std::string>()) {
		if (towerController->towerExists(j["name"].get<std::string>())) {
			QMessageBox::warning(this, "Warning", "Tower with this name has already exist", QMessageBox::Ok);
			return;
		}
	}

	tower->fromJson(j);

	BaseEditor::flashSaveButton(ui->saveButton);

	auto towers = towerController->getTowerNames();
	for (const auto &name: towers) {
		qDebug() << "   tower name: " << name;
	}

	updateTowerList();
	updateUpgradeList();
}

void TowerEditor::onDeleteButtonClicked() const {
	const auto tower = towerController->getCurrentTower();
	if (!tower) {
		return;
	}

	towerController->removeTower(tower->getName());

	updateTowerList();
	updateUpgradeList();
}

void TowerEditor::onChooseTextureButtonClicked() {
	QString filePath = QFileDialog::getOpenFileName(this, "Choose Texture", QDir::currentPath(), ".png");

	if (filePath.isEmpty()) {
		return;
	}
	try {
		towerController->setTowerTexture(filePath.toStdString());
		for (auto it = m_propertyEditors.constBegin(); it != m_propertyEditors.constEnd(); it++) {
			const QString &key = it.key();
			if (key == "towerTexturePath") {
				if (auto *edit = qobject_cast<QLineEdit *>(it.value())) {
					edit->setText(filePath);
				}
				break;
			}
		}
		showTowerTexturePreview(filePath.toStdString());
	} catch (const std::exception &e) {
		qDebug() << "Error: " << e.what();
		return;
	}
}

void TowerEditor::onAddNextUpgradeButtonClicked() {
}

void TowerEditor::onRemoveNextUpgradeButtonClicked() {
}

void TowerEditor::updateTowerList() const {
	qDebug() << "Tower list updated";

	const auto towerNames = towerController->getTowerNames();

	BaseEditor::fillListWidget(ui->towerList, towerNames);
}

void TowerEditor::updateUpgradeList() const {
	BaseEditor::fillListWidget(ui->nextTowerList, towerController->getNextUpgradeNames());
}

void TowerEditor::fillPropertiesForm(const std::shared_ptr<TowerSample> &tower) {
	auto j = tower->toJson();

	for (auto &[key, value]: j.items()) {
		bool flag = false;
		for (auto &elem: doNotShowThisFields) {
			if (key == elem) {
				flag = true;
				break;
			}
		}
		if (flag) {
			continue;
		}

		if (key == "towerTexturePath") {
			if (!value.empty()) {
				showTowerTexturePreview(value);
			} else {
				ui->towerPreview->setText("No preview");
			}
		}

		QString fieldName = QString::fromStdString(key);
		QString label = fieldName;
		label[0] = label[0].toUpper();

		QWidget *editor = nullptr;

		if (key == "nextUpgrade") {
			// auto *edit =
		} else {
			if (value.is_string()) {
				auto *edit = new QLineEdit(QString::fromStdString(value.get<std::string>()), this);
				editor = edit;
			} else if (value.is_number_float() || value.is_number()) {
				auto *spin = new QDoubleSpinBox(this);
				spin->setRange(-1e6, 1e6);
				spin->setDecimals(3);
				spin->setValue(value.get<double>());
				editor = spin;
			} else if (value.is_boolean()) {
				auto *check = new QCheckBox(this);
				check->setChecked(value.get<bool>());
				editor = check;
			}
		}
		if (editor) {
			editor->setObjectName(fieldName);
			ui->propertiesForm->addRow(label + ":", editor);
			m_propertyEditors[fieldName] = editor;
		}
	}
}

void TowerEditor::showTowerTexturePreview(const std::string &path) const {
	QPixmap pixmap(QString::fromStdString(path));
	ui->towerPreview->setPixmap(pixmap.scaled(ui->towerPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
