#ifndef TOWERDEFENCE_ENEMYEDITOR_H
#define TOWERDEFENCE_ENEMYEDITOR_H

#include <QWidget>

#include "BaseEditor.h"
#include "EnemyController.h"
#include "EnemySample.h"

QT_BEGIN_NAMESPACE

namespace Ui {
	class EnemyEditor;
}

QT_END_NAMESPACE

class EnemyEditor : public QWidget, protected BaseEditor {
	Q_OBJECT

public:
	explicit EnemyEditor(const std::shared_ptr<EnemyController> &EnemyController, QWidget *parent = nullptr);

	~EnemyEditor() override;

private slots:
	void addEnemy() const;

	void onItemClicked(const QListWidgetItem *item);

	void onSaveButtonClicked();

	void onDeleteButtonClicked();

	void onChooseTextureButtonClicked();

private:
	Ui::EnemyEditor *ui;

	void updateEnemyList() const;

	void fillPropertiesForm(const std::shared_ptr<EnemySample> &enemy);

	void showEnemyTexturePreview(const std::string &path) const;

	std::shared_ptr<EnemyController> enemyController;
	QMap<QString, QWidget *> m_propertyEditors;
	std::vector<std::string> doNotShowThisFields{};
};

#endif // TOWERDEFENCE_ENEMYEDITOR_H
