#ifndef TOWERDEFENCE_WAVEEDITOR_H
#define TOWERDEFENCE_WAVEEDITOR_H

#include <QDialog>
#include <qlistwidget.h>


class MapController;
QT_BEGIN_NAMESPACE
namespace Ui {
	class WaveEditor;
}
QT_END_NAMESPACE

class WaveEditor : public QDialog {
	Q_OBJECT

public:
	explicit WaveEditor(const std::shared_ptr<MapController> &mapController, const std::string &waveName,
						QWidget *parent = nullptr);
	~WaveEditor() override;

private:
	void updateEnemyList();
	void onAddEnemyClicked();
	void onRemoveEnemyClicked();
	void onEnemyCountChanged(QListWidgetItem *item);
	bool isWaveNameUnique(const std::string &name) const;

	Ui::WaveEditor *ui;
	std::string originalWaveName;
	std::shared_ptr<MapController> mapController;
};


#endif // TOWERDEFENCE_WAVEEDITOR_H
