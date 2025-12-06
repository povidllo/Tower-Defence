#include "../../include/Editors/WaveEditor.h"

#include <QMessageBox>

#include "MapController.h"
#include "ui_WaveEditor.h"


WaveEditor::WaveEditor(const std::shared_ptr<MapController> &mapController, const std::string &waveName,
					   QWidget *parent) :
	QDialog(parent), ui(new Ui::WaveEditor), originalWaveName(waveName), mapController(mapController) {
	ui->setupUi(this);
	setWindowTitle(tr("Edit Wave: %1").arg(QString::fromStdString(waveName)));
	auto enemies = mapController->getAvailableEnemies();
	for (const auto &enemy: enemies) {
		ui->enemyComboBox->addItem(QString::fromStdString(enemy));
	}

	auto wave = mapController->getWave(waveName);
	updateEnemyList();

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

		accept();
	});
}

WaveEditor::~WaveEditor() { delete ui; }
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

	// Проверяем дубли
	for (int i = 0; i < ui->enemiesList->count(); ++i) {
		if (ui->enemiesList->item(i)->data(Qt::UserRole).toString() == enemyName) {
			QMessageBox::information(this, "Info", "This enemy is already in the wave!");
			return;
		}
	}

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
		if (wave.getName() == name && wave.getName() != originalWaveName) {
			return false;
		}
	}
	return true;
}
