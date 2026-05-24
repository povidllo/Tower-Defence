#include "EffectEditor.h"

#include <algorithm>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>

#include "EnemyEffectSample.h"
#include "TextureUtils.h"
#include "TowerEffectSample.h"
#include "ui_EffectEditor.h"

EffectEditor::EffectEditor(const std::shared_ptr<EffectController> &effectController, QWidget *parent)
	: QWidget(parent), ui(new Ui::EffectEditor), effectController(effectController) {
	ui->setupUi(this);
	configureUi();

	connect(ui->addEnemyEffectButton, &QPushButton::clicked, this, &EffectEditor::onAddEnemyEffectClicked);
	connect(ui->addTowerEffectButton, &QPushButton::clicked, this, &EffectEditor::onAddTowerEffectClicked);
	connect(ui->effectList, &QListWidget::itemClicked, this, &EffectEditor::onEffectClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &EffectEditor::onDeleteEffectClicked);
	connect(ui->saveButton, &QPushButton::clicked, this, &EffectEditor::onSaveEffectClicked);
	connect(ui->chooseTextureButton, &QPushButton::clicked, this, &EffectEditor::onChooseTextureClicked);
	connect(ui->addAfterFinishButton, &QPushButton::clicked, this, &EffectEditor::onAddAfterFinishClicked);
	connect(ui->removeAfterFinishButton, &QPushButton::clicked, this, &EffectEditor::onRemoveAfterFinishClicked);

	updateEffectList();
	setRightPanelVisible(false);
}

EffectEditor::~EffectEditor() {
	delete ui;
}

void EffectEditor::configureUi() const {
	const auto configureIntSpin = [](QDoubleSpinBox *spin) {
		spin->setRange(-1000000, 1000000);
		spin->setDecimals(0);
		spin->setSingleStep(1);
	};
	const auto configureSecondsSpin = [](QDoubleSpinBox *spin) {
		spin->setRange(0, 1000000);
		spin->setDecimals(3);
		spin->setSingleStep(0.1);
	};

	configureSecondsSpin(ui->durationSpin);
	configureSecondsSpin(ui->periodSpin);
	configureIntSpin(ui->startHealthImpactSpin);
	configureIntSpin(ui->startSpeedImpactSpin);
	configureIntSpin(ui->periodicHealthImpactSpin);
	configureIntSpin(ui->periodicSpeedImpactSpin);
	configureIntSpin(ui->startDamageFlatImpactSpin);
	configureIntSpin(ui->startDamagePercentImpactSpin);
	configureIntSpin(ui->startAttackSpeedPercentImpactSpin);
	configureIntSpin(ui->periodicDamageFlatImpactSpin);
	configureIntSpin(ui->periodicDamagePercentImpactSpin);
	configureIntSpin(ui->periodicAttackSpeedPercentImpactSpin);
	ui->startSpeedImpactSpin->setMinimum(-100);
	ui->periodicSpeedImpactSpin->setMinimum(-100);
}

void EffectEditor::onAddEnemyEffectClicked() {
	std::string baseName = "enemy_effect";
	int counter = 1;
	while (effectController->effectExists(baseName)) {
		baseName = "enemy_effect_" + std::to_string(counter);
		counter++;
	}
	effectController->addEffect(baseName, EffectSample::Kind::Enemy);
	updateEffectList();
	setRightPanelVisible(true);
	loadEffectToForm(effectController->getCurrentEffect());
}

void EffectEditor::onAddTowerEffectClicked() {
	std::string baseName = "tower_effect";
	int counter = 1;
	while (effectController->effectExists(baseName)) {
		baseName = "tower_effect_" + std::to_string(counter);
		counter++;
	}
	effectController->addEffect(baseName, EffectSample::Kind::Tower);
	updateEffectList();
	setRightPanelVisible(true);
	loadEffectToForm(effectController->getCurrentEffect());
}

void EffectEditor::onEffectClicked(const QListWidgetItem *item) {
	effectController->setCurrentEffect(item->text().toStdString());
	setRightPanelVisible(true);
	loadEffectToForm(effectController->getCurrentEffect());
}

void EffectEditor::onDeleteEffectClicked() {
	const auto effect = effectController->getCurrentEffect();
	if (!effect) {
		return;
	}

	const auto name = effect->getName();
	effectController->removeEffect(name);
	updateEffectList();
	setRightPanelVisible(false);
}

void EffectEditor::onSaveEffectClicked() {
	const auto effect = effectController->getCurrentEffect();
	if (!effect) {
		return;
	}

	const std::string newName = ui->nameEdit->text().toStdString();
	if (newName.empty()) {
		QMessageBox::warning(this, "Warning", "Effect name can't be empty", QMessageBox::Ok);
		return;
	}

	if (effect->getName() != newName && effectController->effectExists(newName)) {
		QMessageBox::warning(this, "Warning", "Effect with this name already exists", QMessageBox::Ok);
		return;
	}

	const auto oldName = effect->getName();
	saveFormToEffect(effect);
	if (oldName != effect->getName()) {
		effectController->renameReferences(oldName, effect->getName());
	}

	BaseEditor::flashSaveButton(ui->saveButton);
	updateEffectList();
	updateEffectsAfterFinishList();
}

void EffectEditor::onChooseTextureClicked() {
	QString filePath = QFileDialog::getOpenFileName(this, "Choose Effect Texture", QDir::currentPath(), "*.png");
	if (filePath.isEmpty()) {
		return;
	}

	filePath = TextureUtils::returnRelativeOrAbsolutePath(filePath);
	ui->visualTexturePathEdit->setText(filePath);
	showVisualPreview(filePath.toStdString());
}

void EffectEditor::onAddAfterFinishClicked() {
	const auto effect = effectController->getCurrentEffect();
	if (!effect) {
		return;
	}

	QStringList availableEffects;
	const auto currentEffects = effect->getEffectsAfterFinish();
	for (const auto &name: effectController->getEffectNames()) {
		if (name == effect->getName()) {
			continue;
		}
		if (std::find(currentEffects.begin(), currentEffects.end(), name) != currentEffects.end()) {
			continue;
		}
		availableEffects << QString::fromStdString(name);
	}

	if (availableEffects.isEmpty()) {
		QMessageBox::information(this, "No Effects", "No available effects to add.");
		return;
	}

	bool ok = false;
	const QString selected = QInputDialog::getItem(
		this,
		tr("Add Effect"),
		tr("Choose effect to apply after finish:"),
		availableEffects,
		0,
		false,
		&ok
	);

	if (ok && !selected.isEmpty()) {
		effect->addEffectAfterFinish(selected.toStdString());
		updateEffectsAfterFinishList();
	}
}

void EffectEditor::onRemoveAfterFinishClicked() {
	const auto effect = effectController->getCurrentEffect();
	const auto item = ui->effectsAfterFinishList->currentItem();
	if (!effect || !item) {
		return;
	}

	effect->removeEffectAfterFinish(item->text().toStdString());
	updateEffectsAfterFinishList();
}

void EffectEditor::updateEffectList() const {
	BaseEditor::fillListWidget(ui->effectList, effectController->getEffectNames());
}

void EffectEditor::updateEffectsAfterFinishList() const {
	ui->effectsAfterFinishList->clear();
	const auto effect = effectController->getCurrentEffect();
	if (!effect) {
		return;
	}

	for (const auto &name: effect->getEffectsAfterFinish()) {
		ui->effectsAfterFinishList->addItem(QString::fromStdString(name));
	}
}

void EffectEditor::loadEffectToForm(const std::shared_ptr<EffectSample> &effect) const {
	if (!effect) {
		return;
	}

	ui->editorTitle->setText("Editing: " + QString::fromStdString(effect->getName()));
	ui->nameEdit->setText(QString::fromStdString(effect->getName()));
	ui->effectTypeValueLabel->setText(
		effect->getKind() == EffectSample::Kind::Tower ? tr("Tower effect") : tr("Enemy effect"));
	ui->durationSpin->setValue(effect->getDurationSeconds());
	ui->periodSpin->setValue(effect->getPeriodSeconds());
	ui->stackableCheck->setChecked(effect->isStackable());
	ui->visualTexturePathEdit->setText(QString::fromStdString(effect->getVisualTexturePath()));
	showVisualPreview(effect->getVisualTexturePath());

	const bool isTower = effect->getKind() == EffectSample::Kind::Tower;
	ui->unitGroup->setVisible(!isTower);
	ui->towerGroup->setVisible(isTower);

	if (const auto enemyEffect = std::dynamic_pointer_cast<EnemyEffectSample>(effect)) {
		ui->startHealthImpactSpin->setValue(enemyEffect->getStartHealthImpact());
		ui->startSpeedImpactSpin->setValue(enemyEffect->getStartSpeedImpactPercent());
		ui->periodicHealthImpactSpin->setValue(enemyEffect->getPeriodicHealthImpact());
		ui->periodicSpeedImpactSpin->setValue(enemyEffect->getPeriodicSpeedImpactPercent());
	}

	if (const auto towerEffect = std::dynamic_pointer_cast<TowerEffectSample>(effect)) {
		ui->startDamageFlatImpactSpin->setValue(towerEffect->getStartDamageFlatImpact());
		ui->startDamagePercentImpactSpin->setValue(towerEffect->getStartDamagePercentImpact());
		ui->startAttackSpeedPercentImpactSpin->setValue(towerEffect->getStartAttackSpeedPercentImpact());
		ui->periodicDamageFlatImpactSpin->setValue(towerEffect->getPeriodicDamageFlatImpact());
		ui->periodicDamagePercentImpactSpin->setValue(towerEffect->getPeriodicDamagePercentImpact());
		ui->periodicAttackSpeedPercentImpactSpin->setValue(towerEffect->getPeriodicAttackSpeedPercentImpact());
	}

	updateEffectsAfterFinishList();
}

void EffectEditor::saveFormToEffect(const std::shared_ptr<EffectSample> &effect) const {
	effect->setName(ui->nameEdit->text().toStdString());
	effect->setDurationSeconds(ui->durationSpin->value());
	effect->setPeriodSeconds(ui->periodSpin->value());
	effect->setStackable(ui->stackableCheck->isChecked());
	effect->setVisualTexturePath(ui->visualTexturePathEdit->text().toStdString());

	if (const auto enemyEffect = std::dynamic_pointer_cast<EnemyEffectSample>(effect)) {
		enemyEffect->setStartHealthImpact(static_cast<int>(ui->startHealthImpactSpin->value()));
		enemyEffect->setStartSpeedImpactPercent(static_cast<int>(ui->startSpeedImpactSpin->value()));
		enemyEffect->setPeriodicHealthImpact(static_cast<int>(ui->periodicHealthImpactSpin->value()));
		enemyEffect->setPeriodicSpeedImpactPercent(static_cast<int>(ui->periodicSpeedImpactSpin->value()));
	}

	if (const auto towerEffect = std::dynamic_pointer_cast<TowerEffectSample>(effect)) {
		towerEffect->setStartDamageFlatImpact(static_cast<int>(ui->startDamageFlatImpactSpin->value()));
		towerEffect->setStartDamagePercentImpact(static_cast<int>(ui->startDamagePercentImpactSpin->value()));
		towerEffect->setStartAttackSpeedPercentImpact(static_cast<int>(ui->startAttackSpeedPercentImpactSpin->value()));
		towerEffect->setPeriodicDamageFlatImpact(static_cast<int>(ui->periodicDamageFlatImpactSpin->value()));
		towerEffect->setPeriodicDamagePercentImpact(static_cast<int>(ui->periodicDamagePercentImpactSpin->value()));
		towerEffect->setPeriodicAttackSpeedPercentImpact(static_cast<int>(ui->periodicAttackSpeedPercentImpactSpin->value()));
	}
}

void EffectEditor::setRightPanelVisible(const bool visible) const {
	ui->rightPanel->setVisible(visible);
}

void EffectEditor::showVisualPreview(const std::string &path) const {
	if (path.empty()) {
		ui->visualPreview->setText("No preview");
		ui->visualPreview->setPixmap(QPixmap());
		return;
	}

	QPixmap pixmap(QString::fromStdString(path));
	if (pixmap.isNull()) {
		ui->visualPreview->setText("Invalid image");
		ui->visualPreview->setPixmap(QPixmap());
		return;
	}

	ui->visualPreview->setText("");
	ui->visualPreview->setPixmap(pixmap.scaled(ui->visualPreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
