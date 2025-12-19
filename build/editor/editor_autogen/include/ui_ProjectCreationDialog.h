/********************************************************************************
** Form generated from reading UI file 'ProjectCreationDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTCREATIONDIALOG_H
#define UI_PROJECTCREATIONDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ProjectCreationDialog
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *ProjectNameLine;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label_2;
    QLineEdit *ProjectLocationLine;
    QPushButton *BrowseLocationButton;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *CancelButton;
    QPushButton *CreateButton;

    void setupUi(QWidget *ProjectCreationDialog)
    {
        if (ProjectCreationDialog->objectName().isEmpty())
            ProjectCreationDialog->setObjectName("ProjectCreationDialog");
        ProjectCreationDialog->resize(400, 300);
        verticalLayout_2 = new QVBoxLayout(ProjectCreationDialog);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        label = new QLabel(ProjectCreationDialog);
        label->setObjectName("label");

        horizontalLayout_3->addWidget(label);

        ProjectNameLine = new QLineEdit(ProjectCreationDialog);
        ProjectNameLine->setObjectName("ProjectNameLine");

        horizontalLayout_3->addWidget(ProjectNameLine);


        verticalLayout->addLayout(horizontalLayout_3);

        verticalSpacer = new QSpacerItem(10, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Maximum);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        label_2 = new QLabel(ProjectCreationDialog);
        label_2->setObjectName("label_2");

        horizontalLayout->addWidget(label_2);

        ProjectLocationLine = new QLineEdit(ProjectCreationDialog);
        ProjectLocationLine->setObjectName("ProjectLocationLine");

        horizontalLayout->addWidget(ProjectLocationLine);

        BrowseLocationButton = new QPushButton(ProjectCreationDialog);
        BrowseLocationButton->setObjectName("BrowseLocationButton");

        horizontalLayout->addWidget(BrowseLocationButton);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Preferred);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        CancelButton = new QPushButton(ProjectCreationDialog);
        CancelButton->setObjectName("CancelButton");

        horizontalLayout_2->addWidget(CancelButton);

        CreateButton = new QPushButton(ProjectCreationDialog);
        CreateButton->setObjectName("CreateButton");

        horizontalLayout_2->addWidget(CreateButton);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);


        retranslateUi(ProjectCreationDialog);

        QMetaObject::connectSlotsByName(ProjectCreationDialog);
    } // setupUi

    void retranslateUi(QWidget *ProjectCreationDialog)
    {
        ProjectCreationDialog->setWindowTitle(QCoreApplication::translate("ProjectCreationDialog", "Form", nullptr));
        label->setText(QCoreApplication::translate("ProjectCreationDialog", "Project Name", nullptr));
        label_2->setText(QCoreApplication::translate("ProjectCreationDialog", "Project Location", nullptr));
        BrowseLocationButton->setText(QCoreApplication::translate("ProjectCreationDialog", "Browse", nullptr));
        CancelButton->setText(QCoreApplication::translate("ProjectCreationDialog", "Cancel", nullptr));
        CreateButton->setText(QCoreApplication::translate("ProjectCreationDialog", "Create", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectCreationDialog: public Ui_ProjectCreationDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTCREATIONDIALOG_H
