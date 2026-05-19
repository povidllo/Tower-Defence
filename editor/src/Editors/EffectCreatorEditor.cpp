#include "EffectCreatorEditor.h"

#include <algorithm>
#include <QInputDialog>
#include <QMessageBox>

#include "ui_EffectCreatorEditor.h"

EffectCreatorEditor::EffectCreatorEditor(
	const std::shared_ptr<EffectCreatorController> &effectCreatorController,
	QWidget *parent
) : QWidget(parent), ui(new Ui::EffectCreatorEditor), effectCreatorController(effectCreatorController) {
	ui->setupUi(this);
	configureUi();

	connect(ui->addButton, &QPushButton::clicked, this, &EffectCreatorEditor::onAddClicked);
	connect(ui->effectCreatorList, &QListWidget::itemClicked, this, &EffectCreatorEditor::onItemClicked);
	connect(ui->deleteButton, &QPushButton::clicked, this, &EffectCreatorEditor::onDeleteClicked);
	connect(ui->saveButton, &QPushButton::clicked, this, &EffectCreatorEditor::onSaveClicked);
	connect(ui->addStartButton, &QPushButton::clicked, this, &EffectCreatorEditor::onAddStartEffectClicked);
	connect(ui->removeStartButton, &QPushButton::clicked, this, &EffectCreatorEditor::onRemoveStartEffectClicked);
	connect(ui->addPeriodicButton, &QPushButton::clicked, this, &EffectCreatorEditor::onAddPeriodicEffectClicked);
	connect(ui->removePeriodicButton, &QPushButton::clicked, this, &EffectCreatorEditor::onRemovePeriodicEffectClicked);

	updateEffectCreatorList();
	setRightPanelVisible(false);
}

EffectCreatorEditor::~EffectCreatorEditor() {
	delete ui;
}

void EffectCreatorEditor::configureUi() const {
	ui->targetRelationCombo->addItem("Enemies", "enemies");
	ui->targetRelationCombo->addItem("Allies", "allies");
	ui->targetRelationCombo->addItem("Neutral", "neutral");
	ui->attachmentPreferenceCombo->addItem("Point", "point");
	ui->attachmentPreferenceCombo->addItem("Object", "object");

	for (auto *spin: {ui->radiusSpin, ui->durationSpin, ui->periodSpin}) {
		spin->setRange(0, 1000000);
		spin->setDecimals(3);
		spin->setSingleStep(0.1);
	}
}

void EffectCreatorEditor::onAddClicked() {
	std::string baseName = "effect_creator";
	int counter = 1;
	while (effectCreatorController->effectCreatorExists(baseName)) {
		baseName = "effect_creator_" + std::to_string(counter);
		counter++;
	}
	effectCreatorController->addEffectCreator(baseName);
	updateEffectCreatorList();
	setRightPanelVisible(true);
	loadEffectCreatorToForm(effectCreatorController->getCurrentEffectCreator());
}

void EffectCreatorEditor::onItemClicked(const QListWidgetItem *item) {
	effectCreatorController->setCurrentEffectCreator(item->text().toStdString());
	setRightPanelVisible(true);
	loadEffectCreatorToForm(effectCreatorController->getCurrentEffectCreator());
}

void EffectCreatorEditor::onDeleteClicked() {
	const auto effectCreator = effectCreatorController->getCurrentEffectCreator();
	if (!effectCreator) {
		return;
	}
	effectCreatorController->removeEffectCreator(effectCreator->getName());
	updateEffectCreatorList();
	setRightPanelVisible(false);
}

void EffectCreatorEditor::onSaveClicked() {
	const auto effectCreator = effectCreatorController->getCurrentEffectCreator();
	if (!effectCreator) {
		return;
	}

	const auto newName = ui->nameEdit->text().toStdString();
	if (newName.empty()) {
		QMessageBox::warning(this, "Warning", "Effect creator name can't be empty", QMessageBox::Ok);
		return;
	}
	if (effectCreator->getName() != newName && effectCreatorController->effectCreatorExists(newName)) {
		QMessageBox::warning(this, "Warning", "Effect creator with this name already exists", QMessageBox::Ok);
		return;
	}

	const auto oldName = effectCreator->getName();
	saveFormToEffectCreator(effectCreator);
	if (oldName != effectCreator->getName()) {
		effectCreatorController->renameReferences(oldName, effectCreator->getName());
	}
	BaseEditor::flashSaveButton(ui->saveButton);
	updateEffectCreatorList();
}

void EffectCreatorEditor::onAddStartEffectClicked() {
	addEffectToList(false);
}

void EffectCreatorEditor::onRemoveStartEffectClicked() {
	removeEffectFromList(false);
}

void EffectCreatorEditor::onAddPeriodicEffectClicked() {
	addEffectToList(true);
}

void EffectCreatorEditor::onRemovePeriodicEffectClicked() {
	removeEffectFromList(true);
}

void EffectCreatorEditor::updateEffectCreatorList() const {
	BaseEditor::fillListWidget(ui->effectCreatorList, effectCreatorController->getEffectCreatorNames());
}

void EffectCreatorEditor::loadEffectCreatorToForm(const std::shared_ptr<EffectCreatorSample> &effectCreator) const {
	if (!effectCreator) {
		return;
	}

	ui->editorTitle->setText("Editing: " + QString::fromStdString(effectCreator->getName()));
	ui->nameEdit->setText(QString::fromStdString(effectCreator->getName()));
	ui->radiusSpin->setValue(effectCreator->getRadius());
	ui->durationSpin->setValue(effectCreator->getDurationSeconds());
	ui->periodSpin->setValue(effectCreator->getPeriodSeconds());
	const int targetIndex = ui->targetRelationCombo->findData(QString::fromStdString(effectCreator->getTargetRelation()));
	ui->targetRelationCombo->setCurrentIndex(targetIndex >= 0 ? targetIndex : 0);
	const int attachmentIndex = ui->attachmentPreferenceCombo->findData(
		QString::fromStdString(effectCreator->getAttachmentPreference())
	);
	ui->attachmentPreferenceCombo->setCurrentIndex(attachmentIndex >= 0 ? attachmentIndex : 0);
	updateEffectLists();
}

void EffectCreatorEditor::saveFormToEffectCreator(const std::shared_ptr<EffectCreatorSample> &effectCreator) const {
	effectCreator->setName(ui->nameEdit->text().toStdString());
	effectCreator->setRadius(ui->radiusSpin->value());
	effectCreator->setDurationSeconds(ui->durationSpin->value());
	effectCreator->setPeriodSeconds(ui->periodSpin->value());
	effectCreator->setTargetRelation(ui->targetRelationCombo->currentData().toString().toStdString());
	effectCreator->setAttachmentPreference(ui->attachmentPreferenceCombo->currentData().toString().toStdString());
}

void EffectCreatorEditor::updateEffectLists() const {
	ui->startEffectsList->clear();
	ui->periodicEffectsList->clear();
	const auto effectCreator = effectCreatorController->getCurrentEffectCreator();
	if (!effectCreator) {
		return;
	}

	for (const auto &name: effectCreator->getStartEffects()) {
		ui->startEffectsList->addItem(QString::fromStdString(name));
	}
	for (const auto &name: effectCreator->getPeriodicEffects()) {
		ui->periodicEffectsList->addItem(QString::fromStdString(name));
	}
}

void EffectCreatorEditor::addEffectToList(const bool periodic) {
	const auto effectCreator = effectCreatorController->getCurrentEffectCreator();
	if (!effectCreator) {
		return;
	}

	const auto currentEffects = periodic ? effectCreator->getPeriodicEffects() : effectCreator->getStartEffects();
	QStringList availableEffects;
	for (const auto &name: effectCreatorController->getEffectNames()) {
		if (std::find(currentEffects.begin(), currentEffects.end(), name) == currentEffects.end()) {
			availableEffects << QString::fromStdString(name);
		}
	}

	if (availableEffects.isEmpty()) {
		QMessageBox::information(this, "No Effects", "No available effects to add.");
		return;
	}

	bool ok = false;
	const QString selected = QInputDialog::getItem(
		this,
		tr("Add Effect"),
		tr("Choose effect:"),
		availableEffects,
		0,
		false,
		&ok
	);
	if (!ok || selected.isEmpty()) {
		return;
	}

	if (periodic) {
		effectCreator->addPeriodicEffect(selected.toStdString());
	} else {
		effectCreator->addStartEffect(selected.toStdString());
	}
	updateEffectLists();
}

void EffectCreatorEditor::removeEffectFromList(const bool periodic) {
	const auto effectCreator = effectCreatorController->getCurrentEffectCreator();
	auto *item = periodic ? ui->periodicEffectsList->currentItem() : ui->startEffectsList->currentItem();
	if (!effectCreator || !item) {
		return;
	}

	if (periodic) {
		effectCreator->removePeriodicEffect(item->text().toStdString());
	} else {
		effectCreator->removeStartEffect(item->text().toStdString());
	}
	updateEffectLists();
}

void EffectCreatorEditor::setRightPanelVisible(const bool visible) const {
	ui->rightPanel->setVisible(visible);
}
