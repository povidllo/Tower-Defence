/********************************************************************************
** Form generated from reading UI file 'ProjectSelectorDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTSELECTORDIALOG_H
#define UI_PROJECTSELECTORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectSelectorDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *ProjectSelectorLabel;
    QListWidget *RecentProjectListWidget;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *NewProjectButton;
    QPushButton *LoadProjectButton;

    void setupUi(QWidget *ProjectSelectorDialog)
    {
        if (ProjectSelectorDialog->objectName().isEmpty())
            ProjectSelectorDialog->setObjectName("ProjectSelectorDialog");
        ProjectSelectorDialog->resize(398, 296);
        verticalLayout_2 = new QVBoxLayout(ProjectSelectorDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        ProjectSelectorLabel = new QLabel(ProjectSelectorDialog);
        ProjectSelectorLabel->setObjectName("ProjectSelectorLabel");

        verticalLayout->addWidget(ProjectSelectorLabel);

        RecentProjectListWidget = new QListWidget(ProjectSelectorDialog);
        RecentProjectListWidget->setObjectName("RecentProjectListWidget");

        verticalLayout->addWidget(RecentProjectListWidget);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        NewProjectButton = new QPushButton(ProjectSelectorDialog);
        NewProjectButton->setObjectName("NewProjectButton");

        horizontalLayout_4->addWidget(NewProjectButton);

        LoadProjectButton = new QPushButton(ProjectSelectorDialog);
        LoadProjectButton->setObjectName("LoadProjectButton");

        horizontalLayout_4->addWidget(LoadProjectButton);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ProjectSelectorDialog);

        QMetaObject::connectSlotsByName(ProjectSelectorDialog);
    } // setupUi

    void retranslateUi(QWidget *ProjectSelectorDialog)
    {
        ProjectSelectorDialog->setWindowTitle(QCoreApplication::translate("ProjectSelectorDialog", "ProjectSelectorDialog", nullptr));
        ProjectSelectorLabel->setText(QCoreApplication::translate("ProjectSelectorDialog", "Tower Defence Editor", nullptr));
        NewProjectButton->setText(QCoreApplication::translate("ProjectSelectorDialog", "[New Project]", nullptr));
        LoadProjectButton->setText(QCoreApplication::translate("ProjectSelectorDialog", "[Load Project]", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectSelectorDialog: public Ui_ProjectSelectorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTSELECTORDIALOG_H
