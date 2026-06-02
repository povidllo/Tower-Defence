#ifndef TOWERDEFENCE_WAVECHAINEDITOR_H
#define TOWERDEFENCE_WAVECHAINEDITOR_H

#include <QDialog>
#include <memory>

#include "WaveChain.h"

class MapController;

QT_BEGIN_NAMESPACE
namespace Ui {
	class WaveChainEditor;
}
QT_END_NAMESPACE

class WaveChainEditor : public QDialog {
	Q_OBJECT

public:
	explicit WaveChainEditor(const std::shared_ptr<MapController> &mapController,
							 const std::shared_ptr<WaveChain> &chain,
							 QWidget *parent = nullptr);

	~WaveChainEditor() override;

private:
	void refreshStepsList();
	void onAddStepClicked();
	void onRemoveStepClicked();
	void onMoveUpClicked();
	void onMoveDownClicked();
	void moveStep(int from, int to);

	Ui::WaveChainEditor *ui;
	std::shared_ptr<MapController> mapController;
	std::shared_ptr<WaveChain> chain;
};

#endif // TOWERDEFENCE_WAVECHAINEDITOR_H
