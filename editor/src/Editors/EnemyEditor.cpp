// You may need to build the project (run Qt uic code generator) to get "ui_EnemyEditor.h" resolved

#include "EnemyEditor.h"

#include <algorithm>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "TowerEditor.h"
#include "ui_EnemyEditor.h"

namespace {
	QStringList getAvailableNames(const std::vector<std::string> &allNames, const std::vector<std::string> &usedNames) {
		QStringList available;
		for (const auto &name: allNames) {
			if (std::find(usedNames.begin(), usedNames.end(), name) == usedNames.end()) {
				available << QString::fromStdString(name);
			}
		}
		return available;
	}
}

EnemyEditor::EnemyEditor(const std::shared_ptr<EnemyController> &EnemyController, QWidget *parent) : QWidget(parent),
	ui(new Ui::EnemyEditor), enemyController{EnemyController} {
	ui->setupUi(this);

	auto *effectCreatorGroup = new QGroupBox("Effect creators", this);
	auto *effectCreatorLayout = new QHBoxLayout(effectCreatorGroup);
	auto *baseLayout = new QVBoxLayout();
	auto *damageTakenLayout = new QVBoxLayout();
	auto *damageDealtLayout = new QVBoxLayout();
	baseEffectCreatorList = new QListWidget(effectCreatorGroup);
	damageTakenEffectCreatorList = new QListWidget(effectCreatorGroup);
	damageDealtEffectCreatorList = new QListWidget(effectCreatorGroup);
	auto *addBaseButton = new QPushButton("Add base", effectCreatorGroup);
	auto *removeBaseButton = new QPushButton("Remove base", effectCreatorGroup);
	auto *addDamageTakenButton = new QPushButton("Add taken", effectCreatorGroup);
	auto *removeDamageTakenButton = new QPushButton("Remove taken", effectCreatorGroup);
	auto *addDamageDealtButton = new QPushButton("Add dealt", effectCreatorGroup);
	auto *removeDamageDealtButton = new QPushButton("Remove dealt", effectCreatorGroup);
	baseLayout->addWidget(new QLabel("On spawn", effectCreatorGroup));
	baseLayout->addWidget(baseEffectCreatorList);
	baseLayout->addWidget(addBaseButton);
	baseLayout->addWidget(removeBaseButton);
	damageTakenLayout->addWidget(new QLabel("On damage taken", effectCreatorGroup));
	damageTakenLayout->addWidget(damageTakenEffectCreatorList);
	damageTakenLayout->addWidget(addDamageTakenButton);
	damageTakenLayout->addWidget(removeDamageTakenButton);
	damageDealtLayout->addWidget(new QLabel("On damage dealt", effectCreatorGroup));
	damageDealtLayout->addWidget(damageDealtEffectCreatorList);
	damageDealtLayout->addWidget(addDamageDealtButton);
	damageDealtLayout->addWidget(removeDamageDealtButton);
	effectCreatorLayout->addLayout(baseLayout);
	effectCreatorLayout->addLayout(damageTakenLayout);
	effectCreatorLayout->addLayout(damageDealtLayout);
	ui->editorLayout->addWidget(effectCreatorGroup);

	connect(ui->addEnemyButton, &QPushButton::clicked, this, &EnemyEditor::addEnemy);
	connect(ui->enemyList, &QListWidget::itemClicked, this, &EnemyEditor::onItemClicked);
	connect(ui->saveButton, &QPushButton::clicked, this, &EnemyEditor::onSaveButtonClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &EnemyEditor::onDeleteButtonClicked);
	connect(ui->chooseTextureButton, &QPushButton::clicked, this, &EnemyEditor::onChooseTextureButtonClicked);
	connect(addBaseButton, &QPushButton::clicked, this, &EnemyEditor::onAddBaseEffectCreatorButtonClicked);
	connect(removeBaseButton, &QPushButton::clicked, this, &EnemyEditor::onRemoveBaseEffectCreatorButtonClicked);
	connect(addDamageTakenButton, &QPushButton::clicked, this, &EnemyEditor::onAddDamageTakenEffectCreatorButtonClicked);
	connect(removeDamageTakenButton, &QPushButton::clicked, this,
			&EnemyEditor::onRemoveDamageTakenEffectCreatorButtonClicked);
	connect(addDamageDealtButton, &QPushButton::clicked, this, &EnemyEditor::onAddDamageDealtEffectCreatorButtonClicked);
	connect(removeDamageDealtButton, &QPushButton::clicked, this,
			&EnemyEditor::onRemoveDamageDealtEffectCreatorButtonClicked);

	updateEnemyList();
	ui->enemyPreview->setVisible(false);
	ui->chooseTextureButton->setVisible(false);
}

EnemyEditor::~EnemyEditor() { delete ui; }

void EnemyEditor::addEnemy() const {
	std::string baseName = "enemy";
	int counter = 1;
	while (enemyController->enemyExists(baseName)) {
		baseName = "enemy_" + std::to_string(counter);
		counter++;
	}
	enemyController->addEnemy(baseName);
	updateEnemyList();
}

void EnemyEditor::onItemClicked(const QListWidgetItem *item) {
	const std::string enemyName = item->text().toStdString();
	qDebug() << "enemy name: " << QString::fromStdString(enemyName);
	qDebug() << "item clicked";
	enemyController->setCurrentEnemy(enemyName);

	auto currentEnemy = enemyController->getCurrentEnemy();

	BaseEditor::clearPropertiesForm(ui->propertiesForm, m_propertyEditors);

	if (!currentEnemy) {
		ui->editorTitle->setText("Enemy Properties");
		return;
	}
	ui->enemyPreview->setVisible(true);
	ui->chooseTextureButton->setVisible(true);

	ui->editorTitle->setText("Editing: " + QString::fromStdString(currentEnemy->getName()));

	fillPropertiesForm(currentEnemy);
	updateEffectCreatorLists();
}

void EnemyEditor::onSaveButtonClicked() {
	const auto enemy = enemyController->getCurrentEnemy();
	if (!enemy) {
		return;
	}

	nlohmann::json j = BaseEditor::collectPropertiesToJson(m_propertyEditors);

	if (enemy->getName() != j["name"].get<std::string>()) {
		if (enemyController->enemyExists(j["name"].get<std::string>())) {
			QMessageBox::warning(this, "Warning", "Enemy with this name has already exist", QMessageBox::Ok);
			return;
		}
	}

	enemy->fromJson(j);

	BaseEditor::flashSaveButton(ui->saveButton);

	auto enemies = enemyController->getEnemyNames();
	for (const auto &name: enemies) {
		qDebug() << "   enemy name: " << QString::fromStdString(name);
	}

	updateEnemyList();
}

void EnemyEditor::onDeleteButtonClicked() {
	const auto enemy = enemyController->getCurrentEnemy();
	if (!enemy) {
		return;
	}

	enemyController->removeEnemy(enemy->getName());

	updateEnemyList();
}

void EnemyEditor::onChooseTextureButtonClicked() {
	QString filePath = QFileDialog::getOpenFileName(this, "Choose Texture", QDir::currentPath(), ".png");

	if (filePath.isEmpty()) {
		return;
	}

	filePath = TextureUtils::returnRelativeOrAbsolutePath(filePath);


	try {
		enemyController->setEnemyTexture(filePath.toStdString());
		for (auto it = m_propertyEditors.constBegin(); it != m_propertyEditors.constEnd(); it++) {
			const QString &key = it.key();
			if (key == "enemyTexturePath") {
				if (auto *edit = qobject_cast<QLineEdit *>(it.value())) {
					edit->setText(filePath);
				}
				break;
			}
		}
		showEnemyTexturePreview(filePath.toStdString());
	} catch (const std::exception &e) {
		qDebug() << "Error: " << e.what();
		return;
	}
}

void EnemyEditor::onAddBaseEffectCreatorButtonClicked() {
	auto currentEnemy = enemyController->getCurrentEnemy();
	if (!currentEnemy) {
		return;
	}

	const auto availableNames = getAvailableNames(
		enemyController->getEffectCreatorNames(),
		currentEnemy->getBaseEffectCreatorNames()
	);
	if (availableNames.isEmpty()) {
		QMessageBox::information(this, "No Effect Creators", "No available effect creators to add.");
		return;
	}

	bool ok = false;
	const auto selected = QInputDialog::getItem(this, tr("Add Effect Creator"), tr("Choose effect creator:"),
												availableNames, 0, false, &ok);
	if (ok && !selected.isEmpty()) {
		currentEnemy->addBaseEffectCreator(selected.toStdString());
		updateEffectCreatorLists();
	}
}

void EnemyEditor::onRemoveBaseEffectCreatorButtonClicked() {
	auto currentEnemy = enemyController->getCurrentEnemy();
	auto *item = baseEffectCreatorList->currentItem();
	if (!currentEnemy || !item) {
		return;
	}
	currentEnemy->removeBaseEffectCreator(item->text().toStdString());
	updateEffectCreatorLists();
}

void EnemyEditor::onAddDamageTakenEffectCreatorButtonClicked() {
	auto currentEnemy = enemyController->getCurrentEnemy();
	if (!currentEnemy) {
		return;
	}

	const auto availableNames = getAvailableNames(
		enemyController->getEffectCreatorNames(),
		currentEnemy->getDamageTakenEffectCreatorNames()
	);
	if (availableNames.isEmpty()) {
		QMessageBox::information(this, "No Effect Creators", "No available effect creators to add.");
		return;
	}

	bool ok = false;
	const auto selected = QInputDialog::getItem(this, tr("Add Effect Creator"), tr("Choose effect creator:"),
												availableNames, 0, false, &ok);
	if (ok && !selected.isEmpty()) {
		currentEnemy->addDamageTakenEffectCreator(selected.toStdString());
		updateEffectCreatorLists();
	}
}

void EnemyEditor::onRemoveDamageTakenEffectCreatorButtonClicked() {
	auto currentEnemy = enemyController->getCurrentEnemy();
	auto *item = damageTakenEffectCreatorList->currentItem();
	if (!currentEnemy || !item) {
		return;
	}
	currentEnemy->removeDamageTakenEffectCreator(item->text().toStdString());
	updateEffectCreatorLists();
}

void EnemyEditor::onAddDamageDealtEffectCreatorButtonClicked() {
	auto currentEnemy = enemyController->getCurrentEnemy();
	if (!currentEnemy) {
		return;
	}

	const auto availableNames = getAvailableNames(
		enemyController->getEffectCreatorNames(),
		currentEnemy->getDamageDealtEffectCreatorNames()
	);
	if (availableNames.isEmpty()) {
		QMessageBox::information(this, "No Effect Creators", "No available effect creators to add.");
		return;
	}

	bool ok = false;
	const auto selected = QInputDialog::getItem(this, tr("Add Effect Creator"), tr("Choose effect creator:"),
												availableNames, 0, false, &ok);
	if (ok && !selected.isEmpty()) {
		currentEnemy->addDamageDealtEffectCreator(selected.toStdString());
		updateEffectCreatorLists();
	}
}

void EnemyEditor::onRemoveDamageDealtEffectCreatorButtonClicked() {
	auto currentEnemy = enemyController->getCurrentEnemy();
	auto *item = damageDealtEffectCreatorList->currentItem();
	if (!currentEnemy || !item) {
		return;
	}
	currentEnemy->removeDamageDealtEffectCreator(item->text().toStdString());
	updateEffectCreatorLists();
}

void EnemyEditor::updateEnemyList() const {
	qDebug() << "Enemy list updated";

	const auto enemyNames = enemyController->getEnemyNames();

	BaseEditor::fillListWidget(ui->enemyList, enemyNames);
}

void EnemyEditor::updateEffectCreatorLists() const {
	const auto currentEnemy = enemyController->getCurrentEnemy();
	if (!currentEnemy) {
		baseEffectCreatorList->clear();
		damageTakenEffectCreatorList->clear();
		damageDealtEffectCreatorList->clear();
		return;
	}
	BaseEditor::fillListWidget(baseEffectCreatorList, currentEnemy->getBaseEffectCreatorNames());
	BaseEditor::fillListWidget(damageTakenEffectCreatorList, currentEnemy->getDamageTakenEffectCreatorNames());
	BaseEditor::fillListWidget(damageDealtEffectCreatorList, currentEnemy->getDamageDealtEffectCreatorNames());
}

void EnemyEditor::fillPropertiesForm(const std::shared_ptr<EnemySample> &enemy) {
	auto j = enemy->toJson();

	for (auto &[key, value]: j.items()) {
		bool skip = false;
		for (auto &elem: doNotShowThisFields) {
			if (key == elem) {
				skip = true;
				break;
			}
		}
		if (skip) {
			continue;
		}

		if (key == "enemyTexturePath") {
			if (!value.empty()) {
				showEnemyTexturePreview(value);
			} else {
				ui->enemyPreview->setText("No preview");
			}
		}

		QString fieldName = QString::fromStdString(key);
		QString label = fieldName;
		label[0] = label[0].toUpper();

		QWidget *editor = nullptr;

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

		if (editor) {
			editor->setObjectName(fieldName);
			ui->propertiesForm->addRow(label + ":", editor);
			m_propertyEditors[fieldName] = editor;
		}
	}
}

void EnemyEditor::showEnemyTexturePreview(const std::string &path) const {
	QPixmap pixmap(QString::fromStdString(path));
	ui->enemyPreview->setPixmap(pixmap.scaled(ui->enemyPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
