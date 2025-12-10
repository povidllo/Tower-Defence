#ifndef TOWERDEFENCE_TOWEREDITOR_H
#define TOWERDEFENCE_TOWEREDITOR_H

#include <QListWidgetItem>
#include <memory>

#include "BaseEditor.h"
#include "TowerController.h"

QT_BEGIN_NAMESPACE

namespace Ui {
	class TowerEditor;
}

QT_END_NAMESPACE

class TowerEditor : public QWidget, protected BaseEditor {
	Q_OBJECT

public:
	explicit TowerEditor(const std::shared_ptr<TowerController> &towerController, QWidget *parent = nullptr);

	~TowerEditor() override;

private slots:
	void addTower() const;

	void onItemClicked(const QListWidgetItem *item);

	void onSaveButtonClicked();

	void onDeleteButtonClicked() const;

	void onChooseTextureButtonClicked();

	void onAddNextUpgradeButtonClicked();

	void onRemoveNextUpgradeButtonClicked();

private:
	Ui::TowerEditor *ui;

	void updateTowerList() const;

	void updateUpgradeList() const;

	void fillPropertiesForm(const std::shared_ptr<TowerSample> &tower);

	void showTowerTexturePreview(const std::string &path) const;

	std::shared_ptr<TowerController> towerController;
	QMap<QString, QWidget *> m_propertyEditors;
	std::vector<std::string> doNotShowThisFields{"x", "y"};
};

#endif // TOWERDEFENCE_TOWEREDITOR_H
