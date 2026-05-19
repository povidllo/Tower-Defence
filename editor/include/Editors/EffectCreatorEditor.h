#ifndef TOWERDEFENCE_EFFECTCREATOREDITOR_H
#define TOWERDEFENCE_EFFECTCREATOREDITOR_H

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QWidget>
#include <memory>

#include "BaseEditor.h"
#include "EffectCreatorController.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class EffectCreatorEditor;
}
QT_END_NAMESPACE

class EffectCreatorEditor : public QWidget, protected BaseEditor {
	Q_OBJECT

public:
	explicit EffectCreatorEditor(
		const std::shared_ptr<EffectCreatorController> &effectCreatorController,
		QWidget *parent = nullptr
	);

	~EffectCreatorEditor() override;

private slots:
	void onAddClicked();

	void onItemClicked(const QListWidgetItem *item);

	void onDeleteClicked();

	void onSaveClicked();

	void onAddStartEffectClicked();

	void onRemoveStartEffectClicked();

	void onAddPeriodicEffectClicked();

	void onRemovePeriodicEffectClicked();

private:
	Ui::EffectCreatorEditor *ui;
	std::shared_ptr<EffectCreatorController> effectCreatorController;

	void configureUi() const;

	void updateEffectCreatorList() const;

	void loadEffectCreatorToForm(const std::shared_ptr<EffectCreatorSample> &effectCreator) const;

	void saveFormToEffectCreator(const std::shared_ptr<EffectCreatorSample> &effectCreator) const;

	void updateEffectLists() const;

	void addEffectToList(bool periodic);

	void removeEffectFromList(bool periodic);

	void setRightPanelVisible(bool visible) const;
};

#endif // TOWERDEFENCE_EFFECTCREATOREDITOR_H
