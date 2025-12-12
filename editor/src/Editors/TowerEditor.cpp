#include "TowerEditor.h"
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>
#include <qinputdialog.h>

#include "ui_TowerEditor.h"

TowerEditor::TowerEditor(const std::shared_ptr<TowerController> &towerController, QWidget *parent) : QWidget(parent),
	ui(new Ui::TowerEditor), towerController(towerController) {
	ui->setupUi(this);

	connect(ui->addTowerButton, &QPushButton::clicked, this, &TowerEditor::onAddTowerButtonClicked);
	connect(ui->towerList, &QListWidget::itemClicked, this, &TowerEditor::onItemClicked);
	connect(ui->saveButton, &QPushButton::clicked, this, &TowerEditor::onSaveButtonClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &TowerEditor::onDeleteButtonClicked);
	connect(ui->chooseTextureButton, &QPushButton::clicked, this, &TowerEditor::onChooseTextureButtonClicked);

	connect(ui->chooseTextureButton, &QPushButton::clicked, this, &TowerEditor::onChooseTextureButtonClicked);
	connect(ui->addNextTowerButton, &QPushButton::clicked, this, &TowerEditor::onAddNextUpgradeButtonClicked);
	connect(ui->removeNextTowerButton, &QPushButton::clicked, this, &TowerEditor::onRemoveNextUpgradeButtonClicked);

	connect(ui->projectileSettings, &QPushButton::clicked, this, &TowerEditor::onProjectileSettingsButtonClicked);

	updateTowerList();
	rightPanelView(false);
}

void TowerEditor::rightPanelView(bool what) {
	// ui->towerPreview->setVisible(what);
	// ui->chooseTextureButton->setVisible(what);
	// ui->nextTowerList->setVisible(what);
	// ui->addNextTowerButton->setVisible(what);
	// ui->removeNextTowerButton->setVisible(what);
	ui->rightPanel->setVisible(what);

	ui->towerPreview->setText("No preview");

	BaseEditor::clearPropertiesForm(ui->propertiesForm, m_propertyEditors);
	if (what) {
		fillPropertiesForm(towerController->getCurrentTower());
	}
}

TowerEditor::~TowerEditor() { delete ui; }

void TowerEditor::onAddTowerButtonClicked() {
	qDebug() << "Add tower";
	std::string baseName = "tower";
	int counter = 1;
	while (towerController->towerExists(baseName)) {
		baseName = "tower_" + std::to_string(counter);
		counter++;
	}
	towerController->addTower(baseName);
	updateTowerList();
	updateUpgradeList();
	rightPanelView(false);
}

void TowerEditor::onItemClicked(const QListWidgetItem *item) {
	const std::string towerName = item->text().toStdString();
	qDebug() << "tower name: " << towerName;
	qDebug() << "item clicked";
	towerController->setCurrentTower(towerName);

	auto currentTower = towerController->getCurrentTower();

	rightPanelView(true);
	updateUpgradeList();
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
		towerController->updateUpgradesNameAfterRename(tower->getName(), j["name"].get<std::string>());
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

void TowerEditor::onDeleteButtonClicked() {
	const auto tower = towerController->getCurrentTower();
	if (!tower) {
		return;
	}
	towerController->updateUpgradesAfterRemoving(tower->getName());
	towerController->removeTower(tower->getName());

	updateTowerList();
	updateUpgradeList();
	rightPanelView(false);
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
	auto currentTower = towerController->getCurrentTower();
	if (!currentTower) {
		QMessageBox::warning(this, "Error", "No tower selected!");
		return;
	}

	// Получаем все башни кроме текущей и уже добавленных
	QStringList availableTowers;
	const auto &allNames = towerController->getTowerNames();
	const auto &currentUpgrades = towerController->getNextUpgradeNames();

	for (const auto &nameStr: allNames) {
		std::string name = nameStr;
		if (name == currentTower->getName()) {
			continue;
		}
		if (std::find(currentUpgrades.begin(), currentUpgrades.end(), name) != currentUpgrades.end()) {
			continue;
		}
		availableTowers << QString::fromStdString(name);
	}

	if (availableTowers.isEmpty()) {
		QMessageBox::information(this, "No Upgrades", "No available towers to add as upgrade.");
		return;
	}

	bool ok;
	QString selected = QInputDialog::getItem(
		this,
		tr("Add Upgrade"),
		tr("Choose tower to upgrade into:"),
		availableTowers,
		0,
		false,
		&ok
	);

	if (ok && !selected.isEmpty()) {
		currentTower->addNextUpgrade(selected.toStdString());
		updateUpgradeList();
		qDebug() << "Added upgrade:" << selected;
	}
}

void TowerEditor::onRemoveNextUpgradeButtonClicked() {
	auto *item = ui->nextTowerList->currentItem();
	if (!item) {
		QMessageBox::information(this, "No Selection", "Select an upgrade to remove.");
		return;
	}

	auto currentTower = towerController->getCurrentTower();
	if (!currentTower) return;

	std::string upgradeName = item->text().toStdString();
	currentTower->removeNextUpgrade(upgradeName);

	updateUpgradeList();
	qDebug() << "Removed upgrade:" << QString::fromStdString(upgradeName);
}

void TowerEditor::onProjectileSettingsButtonClicked() {
	auto currentTower = towerController->getCurrentTower();
	if (!currentTower) {
		QMessageBox::warning(this, "Error", "No tower selected!");
		return;
	}

	std::string currentPath = currentTower->getProjectileTexturePath();
	double currentSpeed = currentTower->getProjectileSpeed();

	QDialog dialog(this);
	dialog.setWindowTitle("Projectile Settings");
	dialog.resize(400, 400);

	QVBoxLayout *layout = new QVBoxLayout(&dialog);

	QLabel *previewLabel = new QLabel("No preview");
	previewLabel->setAlignment(Qt::AlignCenter);
	previewLabel->setMinimumSize(128, 128);
	previewLabel->setStyleSheet("border: 1px solid gray; background: white;");
	layout->addWidget(previewLabel);

	QPushButton *chooseBtn = new QPushButton("Choose Projectile Texture");
	layout->addWidget(chooseBtn);

	QHBoxLayout *speedLayout = new QHBoxLayout();
	QLabel *speedLabel = new QLabel("Projectile Speed:");
	QDoubleSpinBox *speedSpin = new QDoubleSpinBox();
	speedSpin->setRange(0.1, 1000.0);
	speedSpin->setDecimals(2);
	speedSpin->setValue(currentSpeed);
	speedLayout->addWidget(speedLabel);
	speedLayout->addWidget(speedSpin);
	layout->addLayout(speedLayout);

	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	layout->addWidget(buttonBox);

	connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

	connect(chooseBtn, &QPushButton::clicked, this, [&]() {
		QString filePath = QFileDialog::getOpenFileName(&dialog, "Choose Projectile Texture", QDir::currentPath(),
														"*.png");
		if (filePath.isEmpty()) return;

		currentPath = filePath.toStdString();

		QPixmap pixmap(filePath);
		if (pixmap.isNull()) {
			previewLabel->setText("Invalid image");
		} else {
			previewLabel->setPixmap(pixmap.scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	});

	if (!currentPath.empty()) {
		QPixmap pixmap(QString::fromStdString(currentPath));
		if (!pixmap.isNull()) {
			previewLabel->setPixmap(pixmap.scaled(previewLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		}
	}

	if (dialog.exec() != QDialog::Accepted) {
		return;
	}

	currentTower->setProjectileTexturePath(currentPath);
	currentTower->setProjectileSpeed(speedSpin->value());

	if (m_propertyEditors.contains("projectileTexturePath")) {
		auto *edit = qobject_cast<QLineEdit *>(m_propertyEditors["projectileTexturePath"]);
		if (edit) edit->setText(QString::fromStdString(currentPath));
	}
	if (m_propertyEditors.contains("projectileSpeed")) {
		auto *spin = qobject_cast<QDoubleSpinBox *>(m_propertyEditors["projectileSpeed"]);
		if (spin) spin->setValue(currentTower->getProjectileSpeed());
	}

	qDebug() << "Projectile settings updated for tower:" << QString::fromStdString(currentTower->getName());
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
