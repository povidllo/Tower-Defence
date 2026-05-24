#ifndef TOWERDEFENCE_EFFECTEDITOR_H
#define TOWERDEFENCE_EFFECTEDITOR_H

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QWidget>
#include <memory>

#include "BaseEditor.h"
#include "EffectController.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class EffectEditor;
}
QT_END_NAMESPACE

class EffectEditor : public QWidget, protected BaseEditor {
	Q_OBJECT

public:
	explicit EffectEditor(const std::shared_ptr<EffectController> &effectController, QWidget *parent = nullptr);

	~EffectEditor() override;

private slots:
	void onAddEnemyEffectClicked();

	void onAddTowerEffectClicked();

	void onEffectClicked(const QListWidgetItem *item);

	void onDeleteEffectClicked();

	void onSaveEffectClicked();

	void onChooseTextureClicked();

	void onAddAfterFinishClicked();

	void onRemoveAfterFinishClicked();

private:
	Ui::EffectEditor *ui;
	std::shared_ptr<EffectController> effectController;

	void configureUi() const;

	void updateEffectList() const;

	void updateEffectsAfterFinishList() const;

	void loadEffectToForm(const std::shared_ptr<EffectSample> &effect) const;

	void saveFormToEffect(const std::shared_ptr<EffectSample> &effect) const;

	void setRightPanelVisible(bool visible) const;

	void showVisualPreview(const std::string &path) const;
};

#endif // TOWERDEFENCE_EFFECTEDITOR_H
