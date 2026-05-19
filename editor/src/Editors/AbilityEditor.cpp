#include "AbilityEditor.h"

#include <algorithm>
#include <QInputDialog>
#include <QMessageBox>

#include "ui_AbilityEditor.h"

AbilityEditor::AbilityEditor(const std::shared_ptr<AbilityController> &abilityController, QWidget *parent)
	: QWidget(parent), ui(new Ui::AbilityEditor), abilityController(abilityController) {
	ui->setupUi(this);
	configureUi();

	connect(ui->addAbilityButton, &QPushButton::clicked, this, &AbilityEditor::onAddAbilityClicked);
	connect(ui->abilityList, &QListWidget::itemClicked, this, &AbilityEditor::onAbilityClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &AbilityEditor::onDeleteAbilityClicked);
	connect(ui->saveButton, &QPushButton::clicked, this, &AbilityEditor::onSaveAbilityClicked);
	connect(ui->addEffectCreatorButton, &QPushButton::clicked, this, &AbilityEditor::onAddEffectCreatorClicked);
	connect(ui->removeEffectCreatorButton, &QPushButton::clicked, this, &AbilityEditor::onRemoveEffectCreatorClicked);

	updateAbilityList();
	setRightPanelVisible(false);
}

AbilityEditor::~AbilityEditor() {
	delete ui;
}

void AbilityEditor::configureUi() const {
	ui->targetSelectionCombo->addItem("None", "none");
	ui->targetSelectionCombo->addItem("Point", "point");
	ui->targetSelectionCombo->addItem("Tower", "tower");
	ui->targetSelectionCombo->addItem("Unit", "unit");
	ui->chargesSpin->setRange(1, 1000000);
	for (auto *spin: {ui->chargeCooldownSpin, ui->fullCooldownSpin}) {
		spin->setRange(0, 1000000);
		spin->setDecimals(3);
		spin->setSingleStep(0.1);
	}
}

void AbilityEditor::onAddAbilityClicked() {
	std::string baseName = "ability";
	int counter = 1;
	while (abilityController->abilityExists(baseName)) {
		baseName = "ability_" + std::to_string(counter);
		counter++;
	}
	abilityController->addAbility(baseName);
	updateAbilityList();
	setRightPanelVisible(true);
	loadAbilityToForm(abilityController->getCurrentAbility());
}

void AbilityEditor::onAbilityClicked(const QListWidgetItem *item) {
	abilityController->setCurrentAbility(item->text().toStdString());
	setRightPanelVisible(true);
	loadAbilityToForm(abilityController->getCurrentAbility());
}

void AbilityEditor::onDeleteAbilityClicked() {
	const auto ability = abilityController->getCurrentAbility();
	if (!ability) {
		return;
	}
	abilityController->removeAbility(ability->getName());
	updateAbilityList();
	setRightPanelVisible(false);
}

void AbilityEditor::onSaveAbilityClicked() {
	const auto ability = abilityController->getCurrentAbility();
	if (!ability) {
		return;
	}

	const auto newName = ui->nameEdit->text().toStdString();
	if (newName.empty()) {
		QMessageBox::warning(this, "Warning", "Ability name can't be empty", QMessageBox::Ok);
		return;
	}
	if (ability->getName() != newName && abilityController->abilityExists(newName)) {
		QMessageBox::warning(this, "Warning", "Ability with this name already exists", QMessageBox::Ok);
		return;
	}

	const auto oldName = ability->getName();
	saveFormToAbility(ability);
	if (oldName != ability->getName()) {
		abilityController->renameReferences(oldName, ability->getName());
	}
	BaseEditor::flashSaveButton(ui->saveButton);
	updateAbilityList();
}

void AbilityEditor::onAddEffectCreatorClicked() {
	const auto ability = abilityController->getCurrentAbility();
	if (!ability) {
		return;
	}

	QStringList availableEffectCreators;
	const auto currentEffectCreators = ability->getEffectCreatorsOnCast();
	for (const auto &name: abilityController->getEffectCreatorNames()) {
		if (std::find(currentEffectCreators.begin(), currentEffectCreators.end(), name) == currentEffectCreators.end()) {
			availableEffectCreators << QString::fromStdString(name);
		}
	}

	if (availableEffectCreators.isEmpty()) {
		QMessageBox::information(this, "No Effect Creators", "No available effect creators to add.");
		return;
	}

	bool ok = false;
	const QString selected = QInputDialog::getItem(
		this,
		tr("Add Effect Creator"),
		tr("Choose effect creator:"),
		availableEffectCreators,
		0,
		false,
		&ok
	);
	if (ok && !selected.isEmpty()) {
		ability->addEffectCreatorOnCast(selected.toStdString());
		updateEffectCreatorList();
	}
}

void AbilityEditor::onRemoveEffectCreatorClicked() {
	const auto ability = abilityController->getCurrentAbility();
	const auto item = ui->effectCreatorsOnCastList->currentItem();
	if (!ability || !item) {
		return;
	}
	ability->removeEffectCreatorOnCast(item->text().toStdString());
	updateEffectCreatorList();
}

void AbilityEditor::updateAbilityList() const {
	BaseEditor::fillListWidget(ui->abilityList, abilityController->getAbilityNames());
}

void AbilityEditor::updateEffectCreatorList() const {
	ui->effectCreatorsOnCastList->clear();
	const auto ability = abilityController->getCurrentAbility();
	if (!ability) {
		return;
	}
	for (const auto &name: ability->getEffectCreatorsOnCast()) {
		ui->effectCreatorsOnCastList->addItem(QString::fromStdString(name));
	}
}

void AbilityEditor::loadAbilityToForm(const std::shared_ptr<AbilitySample> &ability) const {
	if (!ability) {
		return;
	}

	ui->editorTitle->setText("Editing: " + QString::fromStdString(ability->getName()));
	ui->nameEdit->setText(QString::fromStdString(ability->getName()));
	const int targetIndex = ui->targetSelectionCombo->findData(QString::fromStdString(ability->getTargetSelection()));
	ui->targetSelectionCombo->setCurrentIndex(targetIndex >= 0 ? targetIndex : 0);
	ui->chargesSpin->setValue(ability->getChargesCount());
	ui->chargeCooldownSpin->setValue(ability->getChargeCooldownSeconds());
	ui->fullCooldownSpin->setValue(ability->getFullCooldownSeconds());
	updateEffectCreatorList();
}

void AbilityEditor::saveFormToAbility(const std::shared_ptr<AbilitySample> &ability) const {
	ability->setName(ui->nameEdit->text().toStdString());
	ability->setTargetSelection(ui->targetSelectionCombo->currentData().toString().toStdString());
	ability->setChargesCount(ui->chargesSpin->value());
	ability->setChargeCooldownSeconds(ui->chargeCooldownSpin->value());
	ability->setFullCooldownSeconds(ui->fullCooldownSpin->value());
}

void AbilityEditor::setRightPanelVisible(const bool visible) const {
	ui->rightPanel->setVisible(visible);
}
