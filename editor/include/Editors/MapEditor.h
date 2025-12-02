#ifndef TOWERDEFENCE_MAPEDITOR_H
#define TOWERDEFENCE_MAPEDITOR_H

#include <QWidget>

#include "BaseEditor.h"
#include "MapController.h"


QT_BEGIN_NAMESPACE
namespace Ui {
	class MapEditor;
}
QT_END_NAMESPACE

class MapEditor : public QWidget, protected BaseEditor {
	Q_OBJECT

public:
	explicit MapEditor(const std::shared_ptr<MapController> &mapController, QWidget *parent = nullptr);
	~MapEditor() override;

private slots:
	void onAddMapButtonClicked();
	void onMapItemClicked(const QListWidgetItem *item);
	void onTextureItemClicked(QListWidgetItem *item);
	void onAddTextureButtonClicked();


private:
	Ui::MapEditor *ui;

	void updateTextureList();
	void updateMapList() const;

	std::shared_ptr<MapController> mapController;

protected:
	bool eventFilter(QObject *obj, QEvent *event) override;
};


#endif // TOWERDEFENCE_MAPEDITOR_H
