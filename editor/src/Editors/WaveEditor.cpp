#include "../../include/Editors/WaveEditor.h"

#include <QMessageBox>

#include "MapController.h"
#include "ui_WaveEditor.h"

WaveEditor::WaveEditor(const std::shared_ptr<MapController> &mapController, const std::string &waveName,
						QWidget *parent) : QDialog(parent), ui(new Ui::WaveEditor), originalWaveName(waveName),
											mapController(mapController) {
	ui->setupUi(this);
	setWindowTitle(tr("Edit Wave: %1").arg(QString::fromStdString(waveName)));
	auto enemies = mapController->getAvailableEnemies();
	for (const auto &enemy: enemies) {
		ui->enemyComboBox->addItem(QString::fromStdString(enemy));
	}

	auto wave = mapController->getWave(waveName);
	updateEnemyList();
	fillWaveSettingsForm();


	auto currentMap = mapController->getCurrentMap();
	connect(ui->addEnemyButton, &QPushButton::clicked, this, &WaveEditor::onAddEnemyClicked);
	connect(ui->removeEnemyButton, &QPushButton::clicked, this, &WaveEditor::onRemoveEnemyClicked);
	connect(ui->enemiesList, &QListWidget::itemDoubleClicked, this, &WaveEditor::onEnemyCountChanged);

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this, currentMap, wave]() {
		QString newName = ui->waveNameEdit->text().trimmed();
		if (newName.isEmpty()) {
			QMessageBox::warning(this, "Error", "Wave name cannot be empty!");
			return;
		}
		if (newName.toStdString() != originalWaveName && !isWaveNameUnique(newName.toStdString())) {
			QMessageBox::warning(this, "Error", "Wave with this name already exists!");
			return;
		}

		wave->setName(newName.toStdString());

		wave->clearEnemies();
		for (int i = 0; i < ui->enemiesList->count(); ++i) {
			auto *item = ui->enemiesList->item(i);
			QString enemyName = item->data(Qt::UserRole).toString();
			int count = item->data(Qt::UserRole + 1).toInt();
			wave->addEnemy(enemyName.toStdString(), count);
		}

		if (waveSettingsEditors.contains("timeForWave")) {
			auto *spin = qobject_cast<QDoubleSpinBox *>(waveSettingsEditors["timeForWave"]);
			if (spin) {
				wave->setTimeForWave(spin->value());
			}
		}

		if (waveSettingsEditors.contains("enemySpawnInterval")) {
			auto *spin = qobject_cast<QDoubleSpinBox *>(waveSettingsEditors["enemySpawnInterval"]);
			if (spin) {
				wave->setEnemySpawnInterval(spin->value());
			}
		}

		accept();
	});

	connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

WaveEditor::~WaveEditor() { delete ui; }

void WaveEditor::fillWaveSettingsForm() {
	auto wave = mapController->getWave(originalWaveName);
	if (!wave) {
		return;
	}

	ui->waveNameEdit->setText(QString::fromStdString(originalWaveName));

	while (ui->propertiesForm->rowCount() > 0) {
		ui->propertiesForm->removeRow(0);
	}

	{
		QDoubleSpinBox *spin = new QDoubleSpinBox(this);
		spin->setRange(1.0, 600.0);
		spin->setDecimals(1);
		spin->setSuffix(" sec");
		spin->setValue(wave->getTimeForWave());

		ui->propertiesForm->addRow("Time for Wave:", spin);
		waveSettingsEditors["timeForWave"] = spin;
	}

	{
		QDoubleSpinBox *spin = new QDoubleSpinBox(this);
		spin->setRange(0.1, 60.0);
		spin->setDecimals(2);
		spin->setSuffix(" sec");
		spin->setValue(wave->getEnemySpawnInterval());

		ui->propertiesForm->addRow("Spawn Interval:", spin);
		waveSettingsEditors["enemySpawnInterval"] = spin;
	}
}

void WaveEditor::updateEnemyList() {
	ui->enemiesList->clear();

	auto currentMap = mapController->getCurrentMap();
	if (!currentMap)
		return;

	auto wave = mapController->getWave(originalWaveName);
	for (const auto &[enemyName, count]: wave->getEnemies()) {
		QListWidgetItem *item = new QListWidgetItem(tr("%1 × %2").arg(QString::fromStdString(enemyName)).arg(count));
		item->setData(Qt::UserRole, QString::fromStdString(enemyName));
		item->setData(Qt::UserRole + 1, count);
		item->setFlags(item->flags() | Qt::ItemIsEditable);
		ui->enemiesList->addItem(item);
	}
}

void WaveEditor::onAddEnemyClicked() {
	QString enemyName = ui->enemyComboBox->currentText();
	int count = ui->countSpinBox->value();
	if (enemyName.isEmpty())
		return;

	auto *item = new QListWidgetItem(tr("%1 × %2").arg(enemyName).arg(count));
	item->setData(Qt::UserRole, enemyName);
	item->setData(Qt::UserRole + 1, count);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	ui->enemiesList->addItem(item);
}

void WaveEditor::onRemoveEnemyClicked() {
	auto *item = ui->enemiesList->currentItem();
	if (!item)
		return;
	delete item;
}

void WaveEditor::onEnemyCountChanged(QListWidgetItem *item) {
	bool ok;
	int newCount = item->text().split("×").last().trimmed().toInt(&ok);
	if (ok && newCount > 0) {
		item->setData(Qt::UserRole + 1, newCount);
		item->setText(tr("%1 × %2").arg(item->data(Qt::UserRole).toString()).arg(newCount));
	}
}

bool WaveEditor::isWaveNameUnique(const std::string &name) const {
	auto currentMap = mapController->getCurrentMap();
	if (!currentMap)
		return true;

	for (const auto &wave: currentMap->getWaves()) {
		if (wave->getName() == name && wave->getName() != originalWaveName) {
			return false;
		}
	}
	return true;
}
