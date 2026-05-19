#include "EffectEditor.h"

#include <algorithm>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QPixmap>

#include "TextureUtils.h"
#include "ui_EffectEditor.h"

EffectEditor::EffectEditor(const std::shared_ptr<EffectController> &effectController, QWidget *parent)
	: QWidget(parent), ui(new Ui::EffectEditor), effectController(effectController) {
	ui->setupUi(this);
	configureUi();

	connect(ui->addEffectButton, &QPushButton::clicked, this, &EffectEditor::onAddEffectClicked);
	connect(ui->effectList, &QListWidget::itemClicked, this, &EffectEditor::onEffectClicked);
	connect(ui->targetTypeCombo, &QComboBox::currentIndexChanged, this, &EffectEditor::onTargetTypeChanged);
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
	ui->targetTypeCombo->addItem("Unit", "unit");
	ui->targetTypeCombo->addItem("Tower", "tower");

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

void EffectEditor::onAddEffectClicked() {
	std::string baseName = "effect";
	int counter = 1;
	while (effectController->effectExists(baseName)) {
		baseName = "effect_" + std::to_string(counter);
		counter++;
	}
	effectController->addEffect(baseName);
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

void EffectEditor::onTargetTypeChanged(const int index) const {
	const bool isTower = ui->targetTypeCombo->itemData(index).toString() == "tower";
	ui->unitGroup->setVisible(!isTower);
	ui->towerGroup->setVisible(isTower);
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
	ui->targetTypeCombo->setCurrentIndex(effect->getTargetType() == EffectSample::TargetType::Tower ? 1 : 0);
	ui->durationSpin->setValue(effect->getDurationSeconds());
	ui->periodSpin->setValue(effect->getPeriodSeconds());
	ui->stackableCheck->setChecked(effect->isStackable());
	ui->visualTexturePathEdit->setText(QString::fromStdString(effect->getVisualTexturePath()));
	showVisualPreview(effect->getVisualTexturePath());

	ui->startHealthImpactSpin->setValue(effect->getStartHealthImpact());
	ui->startSpeedImpactSpin->setValue(effect->getStartSpeedImpactPercent());
	ui->periodicHealthImpactSpin->setValue(effect->getPeriodicHealthImpact());
	ui->periodicSpeedImpactSpin->setValue(effect->getPeriodicSpeedImpactPercent());

	ui->startDamageFlatImpactSpin->setValue(effect->getStartDamageFlatImpact());
	ui->startDamagePercentImpactSpin->setValue(effect->getStartDamagePercentImpact());
	ui->startAttackSpeedPercentImpactSpin->setValue(effect->getStartAttackSpeedPercentImpact());
	ui->periodicDamageFlatImpactSpin->setValue(effect->getPeriodicDamageFlatImpact());
	ui->periodicDamagePercentImpactSpin->setValue(effect->getPeriodicDamagePercentImpact());
	ui->periodicAttackSpeedPercentImpactSpin->setValue(effect->getPeriodicAttackSpeedPercentImpact());

	updateEffectsAfterFinishList();
	onTargetTypeChanged(ui->targetTypeCombo->currentIndex());
}

void EffectEditor::saveFormToEffect(const std::shared_ptr<EffectSample> &effect) const {
	effect->setName(ui->nameEdit->text().toStdString());
	effect->setTargetTypeString(ui->targetTypeCombo->currentData().toString().toStdString());
	effect->setDurationSeconds(ui->durationSpin->value());
	effect->setPeriodSeconds(ui->periodSpin->value());
	effect->setStackable(ui->stackableCheck->isChecked());
	effect->setVisualTexturePath(ui->visualTexturePathEdit->text().toStdString());

	effect->setStartHealthImpact(static_cast<int>(ui->startHealthImpactSpin->value()));
	effect->setStartSpeedImpactPercent(static_cast<int>(ui->startSpeedImpactSpin->value()));
	effect->setPeriodicHealthImpact(static_cast<int>(ui->periodicHealthImpactSpin->value()));
	effect->setPeriodicSpeedImpactPercent(static_cast<int>(ui->periodicSpeedImpactSpin->value()));

	effect->setStartDamageFlatImpact(static_cast<int>(ui->startDamageFlatImpactSpin->value()));
	effect->setStartDamagePercentImpact(static_cast<int>(ui->startDamagePercentImpactSpin->value()));
	effect->setStartAttackSpeedPercentImpact(static_cast<int>(ui->startAttackSpeedPercentImpactSpin->value()));
	effect->setPeriodicDamageFlatImpact(static_cast<int>(ui->periodicDamageFlatImpactSpin->value()));
	effect->setPeriodicDamagePercentImpact(static_cast<int>(ui->periodicDamagePercentImpactSpin->value()));
	effect->setPeriodicAttackSpeedPercentImpact(static_cast<int>(ui->periodicAttackSpeedPercentImpactSpin->value()));
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
