#include "../../include/Editors/WaveChainEditor.h"

#include <QListWidgetItem>
#include <QMessageBox>

#include "MapController.h"
#include "ui_WaveChainEditor.h"

WaveChainEditor::WaveChainEditor(const std::shared_ptr<MapController> &mapController,
								 const std::shared_ptr<WaveChain> &chain,
								 QWidget *parent)
	: QDialog(parent), ui(new Ui::WaveChainEditor), mapController(mapController), chain(chain) {
	ui->setupUi(this);

	for (const auto &waveName : mapController->getWavesNames()) {
		ui->waveComboBox->addItem(QString::fromStdString(waveName));
	}

	connect(ui->addStepButton, &QPushButton::clicked, this, &WaveChainEditor::onAddStepClicked);
	connect(ui->removeStepButton, &QPushButton::clicked, this, &WaveChainEditor::onRemoveStepClicked);
	connect(ui->moveUpButton, &QPushButton::clicked, this, &WaveChainEditor::onMoveUpClicked);
	connect(ui->moveDownButton, &QPushButton::clicked, this, &WaveChainEditor::onMoveDownClicked);
	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	refreshStepsList();
}

WaveChainEditor::~WaveChainEditor() {
	delete ui;
}

void WaveChainEditor::refreshStepsList() {
	ui->stepsList->clear();
	if (!chain) {
		return;
	}

	const auto &steps = chain->getChain();
	const auto &delays = chain->getDelays();
	for (size_t i = 0; i < steps.size(); ++i) {
		const double delay = i < delays.size() ? delays[i] : 0.0;
		const QString label = QString("%1. %2  (delay: %3 sec)")
								  .arg(static_cast<int>(i + 1))
								  .arg(QString::fromStdString(steps[i]))
								  .arg(delay, 0, 'f', 1);
		auto *item = new QListWidgetItem(label);
		item->setData(Qt::UserRole, static_cast<int>(i));
		ui->stepsList->addItem(item);
	}
}

void WaveChainEditor::onAddStepClicked() {
	if (!chain) {
		return;
	}

	const QString waveName = ui->waveComboBox->currentText();
	if (waveName.isEmpty()) {
		QMessageBox::warning(this, "Error", "Select a wave first.");
		return;
	}

	chain->addStep(waveName.toStdString(), ui->delaySpinBox->value());
	refreshStepsList();
}

void WaveChainEditor::onRemoveStepClicked() {
	if (!chain) {
		return;
	}

	const auto *item = ui->stepsList->currentItem();
	if (!item) {
		return;
	}

	chain->removeStep(static_cast<size_t>(item->data(Qt::UserRole).toInt()));
	refreshStepsList();
}

void WaveChainEditor::onMoveUpClicked() {
	const auto *item = ui->stepsList->currentItem();
	if (!item) {
		return;
	}
	const int row = ui->stepsList->row(item);
	if (row <= 0) {
		return;
	}
	moveStep(row, row - 1);
}

void WaveChainEditor::onMoveDownClicked() {
	const auto *item = ui->stepsList->currentItem();
	if (!item) {
		return;
	}
	const int row = ui->stepsList->row(item);
	if (row < 0 || row >= ui->stepsList->count() - 1) {
		return;
	}
	moveStep(row, row + 1);
}

void WaveChainEditor::moveStep(const int from, const int to) {
	if (!chain) {
		return;
	}

	auto chainCopy = chain->getChain();
	auto delaysCopy = chain->getDelays();
	if (from < 0 || to < 0 || from >= static_cast<int>(chainCopy.size()) || to >= static_cast<int>(chainCopy.size())) {
		return;
	}

	std::swap(chainCopy[static_cast<size_t>(from)], chainCopy[static_cast<size_t>(to)]);
	std::swap(delaysCopy[static_cast<size_t>(from)], delaysCopy[static_cast<size_t>(to)]);
	chain->setChain(chainCopy);
	chain->setDelays(delaysCopy);
	refreshStepsList();
	ui->stepsList->setCurrentRow(to);
}
