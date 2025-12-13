/********************************************************************************
** Form generated from reading UI file 'ProjectEditor.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTEDITOR_H
#define UI_PROJECTEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectEditor
{
public:
    QAction *actionOpen_project;
    QAction *actionSave_project;
    QAction *actionTower_editor;
    QAction *actionEnemy_editor;
    QAction *actionMap_editor;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *towerEditorPage;
    QWidget *enemyEditorPage;
    QWidget *mapEditorPage;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEditor;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *ProjectEditor)
    {
        if (ProjectEditor->objectName().isEmpty())
            ProjectEditor->setObjectName("ProjectEditor");
        ProjectEditor->resize(1050, 650);
        ProjectEditor->setMinimumSize(QSize(1050, 600));
        actionOpen_project = new QAction(ProjectEditor);
        actionOpen_project->setObjectName("actionOpen_project");
        actionSave_project = new QAction(ProjectEditor);
        actionSave_project->setObjectName("actionSave_project");
        actionTower_editor = new QAction(ProjectEditor);
        actionTower_editor->setObjectName("actionTower_editor");
        actionEnemy_editor = new QAction(ProjectEditor);
        actionEnemy_editor->setObjectName("actionEnemy_editor");
        actionMap_editor = new QAction(ProjectEditor);
        actionMap_editor->setObjectName("actionMap_editor");
        centralwidget = new QWidget(ProjectEditor);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        towerEditorPage = new QWidget();
        towerEditorPage->setObjectName("towerEditorPage");
        stackedWidget->addWidget(towerEditorPage);
        enemyEditorPage = new QWidget();
        enemyEditorPage->setObjectName("enemyEditorPage");
        stackedWidget->addWidget(enemyEditorPage);
        mapEditorPage = new QWidget();
        mapEditorPage->setObjectName("mapEditorPage");
        stackedWidget->addWidget(mapEditorPage);

        verticalLayout->addWidget(stackedWidget);

        ProjectEditor->setCentralWidget(centralwidget);
        menubar = new QMenuBar(ProjectEditor);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1050, 30));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuEditor = new QMenu(menubar);
        menuEditor->setObjectName("menuEditor");
        ProjectEditor->setMenuBar(menubar);
        statusbar = new QStatusBar(ProjectEditor);
        statusbar->setObjectName("statusbar");
        ProjectEditor->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEditor->menuAction());
        menuFile->addAction(actionOpen_project);
        menuFile->addAction(actionSave_project);
        menuEditor->addAction(actionTower_editor);
        menuEditor->addAction(actionEnemy_editor);
        menuEditor->addAction(actionMap_editor);

        retranslateUi(ProjectEditor);

        QMetaObject::connectSlotsByName(ProjectEditor);
    } // setupUi

    void retranslateUi(QMainWindow *ProjectEditor)
    {
        ProjectEditor->setWindowTitle(QCoreApplication::translate("ProjectEditor", "ProjectEditor", nullptr));
        actionOpen_project->setText(QCoreApplication::translate("ProjectEditor", "Open project", nullptr));
        actionSave_project->setText(QCoreApplication::translate("ProjectEditor", "Save project", nullptr));
        actionTower_editor->setText(QCoreApplication::translate("ProjectEditor", "Tower editor", nullptr));
        actionEnemy_editor->setText(QCoreApplication::translate("ProjectEditor", "Enemy editor", nullptr));
        actionMap_editor->setText(QCoreApplication::translate("ProjectEditor", "Map editor", nullptr));
        menuFile->setTitle(QCoreApplication::translate("ProjectEditor", "File", nullptr));
        menuEditor->setTitle(QCoreApplication::translate("ProjectEditor", "Editor", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectEditor: public Ui_ProjectEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTEDITOR_H
