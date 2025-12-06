#ifndef ProjectEditor_H
#define ProjectEditor_H

#include <QMainWindow>

#include "EnemyEditor.h"
#include "MapEditor.h"
#include "Project.h"
#include "ProjectController.h"
#include "TowerEditor.h"

QT_BEGIN_NAMESPACE

namespace Ui {
	class ProjectEditor;
}

QT_END_NAMESPACE

class ProjectEditor : public QMainWindow {
	Q_OBJECT

public:
	explicit ProjectEditor(std::string path, std::string name, QWidget *parent = nullptr);
	explicit ProjectEditor(std::string path, QWidget *parent = nullptr);

	~ProjectEditor() override;

private:
	// std::shared_ptr<Project> currentProject;

	std::shared_ptr<ProjectController> projectController;
	// CampaignEditor* campaignEditor;
	// AbilityEditor* abilityEditor;

	Ui::ProjectEditor *ui;
	QWidget *container;


	std::unique_ptr<TowerEditor> towerEditor;
	std::unique_ptr<EnemyEditor> enemyEditor;
	std::unique_ptr<MapEditor> mapEditor;

	// TowerEditor *towerEditor;
	// EnemyEditor *enemyEditor;
	void commonSetUp();


private slots:
	void openTowerEditor();
	void openEnemyEditor();
	void openMapEditor();
	void onSaveProjectClicked();
};


#endif
