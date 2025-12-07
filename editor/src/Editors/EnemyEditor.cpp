// You may need to build the project (run Qt uic code generator) to get "ui_EnemyEditor.h" resolved

#include "EnemyEditor.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>

#include "TowerEditor.h"
#include "ui_EnemyEditor.h"

EnemyEditor::EnemyEditor(const std::shared_ptr<EnemyController> &EnemyController, QWidget *parent) : QWidget(parent),
	ui(new Ui::EnemyEditor), enemyController{EnemyController} {
	ui->setupUi(this);

	connect(ui->addEnemyButton, &QPushButton::clicked, this, &EnemyEditor::addEnemy);
	connect(ui->enemyList, &QListWidget::itemClicked, this, &EnemyEditor::onItemClicked);
	connect(ui->saveButton, &QPushButton::clicked, this, &EnemyEditor::onSaveButtonClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &EnemyEditor::onDeleteButtonClicked);
	connect(ui->chooseTextureButton, &QPushButton::clicked, this, &EnemyEditor::onChooseTextureButtonClicked);

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
	qDebug() << "enemy name: " << enemyName;
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
		qDebug() << "   enemy name: " << name;
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

void EnemyEditor::updateEnemyList() const {
	qDebug() << "Enemy list updated";

	const auto enemyNames = enemyController->getEnemyNames();

	BaseEditor::fillListWidget(ui->enemyList, enemyNames);
}

void EnemyEditor::fillPropertiesForm(const std::shared_ptr<EnemySample> &enemy) {
	auto j = enemy->toJson();

	for (auto &[key, value]: j.items()) {
		// Скрытые поля
		bool skip = false;
		for (auto &elem: doNotShowThisFields) {
			if (key == elem) {
				skip = true;
				break;
			}
		}
		if (skip)
			continue;

		// Превью текстуры
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
