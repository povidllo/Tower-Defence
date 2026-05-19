#ifndef TOWERDEFENCE_ABILITYEDITOR_H
#define TOWERDEFENCE_ABILITYEDITOR_H

#include <QListWidgetItem>
#include <QWidget>
#include <memory>

#include "AbilityController.h"
#include "BaseEditor.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class AbilityEditor;
}
QT_END_NAMESPACE

class AbilityEditor : public QWidget, protected BaseEditor {
	Q_OBJECT

public:
	explicit AbilityEditor(const std::shared_ptr<AbilityController> &abilityController, QWidget *parent = nullptr);

	~AbilityEditor() override;

private slots:
	void onAddAbilityClicked();

	void onAbilityClicked(const QListWidgetItem *item);

	void onDeleteAbilityClicked();

	void onSaveAbilityClicked();

	void onAddEffectCreatorClicked();

	void onRemoveEffectCreatorClicked();

private:
	Ui::AbilityEditor *ui;
	std::shared_ptr<AbilityController> abilityController;

	void configureUi() const;

	void updateAbilityList() const;

	void updateEffectCreatorList() const;

	void loadAbilityToForm(const std::shared_ptr<AbilitySample> &ability) const;

	void saveFormToAbility(const std::shared_ptr<AbilitySample> &ability) const;

	void setRightPanelVisible(bool visible) const;
};

#endif // TOWERDEFENCE_ABILITYEDITOR_H
