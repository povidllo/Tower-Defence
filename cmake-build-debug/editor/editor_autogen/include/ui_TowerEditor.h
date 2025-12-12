/********************************************************************************
** Form generated from reading UI file 'TowerEditor.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOWEREDITOR_H
#define UI_TOWEREDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TowerEditor
{
public:
    QHBoxLayout *horizontalLayout;
    QWidget *widget;
    QVBoxLayout *leftLayout;
    QLabel *label;
    QListWidget *towerList;
    QPushButton *addTowerButton;
    QWidget *rightPanel;
    QVBoxLayout *rightLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *towerPreview;
    QPushButton *chooseTextureButton;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *nextTowerList;
    QVBoxLayout *verticalLayout;
    QPushButton *addNextTowerButton;
    QPushButton *removeNextTowerButton;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *editorLayout;
    QFormLayout *propertiesForm;
    QPushButton *projectileSettings;
    QHBoxLayout *buttonLayout;
    QPushButton *saveButton;
    QPushButton *deleteButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *TowerEditor)
    {
        if (TowerEditor->objectName().isEmpty())
            TowerEditor->setObjectName("TowerEditor");
        TowerEditor->resize(1056, 703);
        horizontalLayout = new QHBoxLayout(TowerEditor);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        widget = new QWidget(TowerEditor);
        widget->setObjectName("widget");
        leftLayout = new QVBoxLayout(widget);
        leftLayout->setSpacing(0);
        leftLayout->setObjectName("leftLayout");
        label = new QLabel(widget);
        label->setObjectName("label");
        QFont font;
        font.setBold(true);
        label->setFont(font);

        leftLayout->addWidget(label);

        towerList = new QListWidget(widget);
        towerList->setObjectName("towerList");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(towerList->sizePolicy().hasHeightForWidth());
        towerList->setSizePolicy(sizePolicy);

        leftLayout->addWidget(towerList);

        addTowerButton = new QPushButton(widget);
        addTowerButton->setObjectName("addTowerButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        addTowerButton->setIcon(icon);

        leftLayout->addWidget(addTowerButton);


        horizontalLayout->addWidget(widget);

        rightPanel = new QWidget(TowerEditor);
        rightPanel->setObjectName("rightPanel");
        rightLayout = new QVBoxLayout(rightPanel);
        rightLayout->setSpacing(0);
        rightLayout->setObjectName("rightLayout");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        towerPreview = new QLabel(rightPanel);
        towerPreview->setObjectName("towerPreview");
        towerPreview->setEnabled(true);
        towerPreview->setMinimumSize(QSize(150, 150));
        towerPreview->setMaximumSize(QSize(200, 200));
        towerPreview->setScaledContents(true);
        towerPreview->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_4->addWidget(towerPreview);

        chooseTextureButton = new QPushButton(rightPanel);
        chooseTextureButton->setObjectName("chooseTextureButton");
        chooseTextureButton->setEnabled(true);

        horizontalLayout_4->addWidget(chooseTextureButton);


        rightLayout->addLayout(horizontalLayout_4);

        widget1 = new QWidget(rightPanel);
        widget1->setObjectName("widget1");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget1->sizePolicy().hasHeightForWidth());
        widget1->setSizePolicy(sizePolicy1);
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        nextTowerList = new QListWidget(widget1);
        nextTowerList->setObjectName("nextTowerList");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(nextTowerList->sizePolicy().hasHeightForWidth());
        nextTowerList->setSizePolicy(sizePolicy2);

        horizontalLayout_2->addWidget(nextTowerList);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        addNextTowerButton = new QPushButton(widget1);
        addNextTowerButton->setObjectName("addNextTowerButton");
        addNextTowerButton->setEnabled(true);

        verticalLayout->addWidget(addNextTowerButton);

        removeNextTowerButton = new QPushButton(widget1);
        removeNextTowerButton->setObjectName("removeNextTowerButton");
        removeNextTowerButton->setEnabled(true);

        verticalLayout->addWidget(removeNextTowerButton);


        horizontalLayout_2->addLayout(verticalLayout);


        rightLayout->addWidget(widget1);

        scrollArea = new QScrollArea(rightPanel);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollWidget = new QWidget();
        scrollWidget->setObjectName("scrollWidget");
        scrollWidget->setGeometry(QRect(0, 0, 764, 287));
        editorLayout = new QVBoxLayout(scrollWidget);
        editorLayout->setSpacing(0);
        editorLayout->setObjectName("editorLayout");
        propertiesForm = new QFormLayout();
        propertiesForm->setObjectName("propertiesForm");
        propertiesForm->setLabelAlignment(Qt::AlignmentFlag::AlignRight);
        propertiesForm->setFormAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);

        editorLayout->addLayout(propertiesForm);

        projectileSettings = new QPushButton(scrollWidget);
        projectileSettings->setObjectName("projectileSettings");

        editorLayout->addWidget(projectileSettings);

        scrollArea->setWidget(scrollWidget);

        rightLayout->addWidget(scrollArea);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        saveButton = new QPushButton(rightPanel);
        saveButton->setObjectName("saveButton");

        buttonLayout->addWidget(saveButton);

        deleteButton = new QPushButton(rightPanel);
        deleteButton->setObjectName("deleteButton");

        buttonLayout->addWidget(deleteButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);


        rightLayout->addLayout(buttonLayout);


        horizontalLayout->addWidget(rightPanel);


        retranslateUi(TowerEditor);

        saveButton->setDefault(true);


        QMetaObject::connectSlotsByName(TowerEditor);
    } // setupUi

    void retranslateUi(QWidget *TowerEditor)
    {
        TowerEditor->setWindowTitle(QCoreApplication::translate("TowerEditor", "Tower Editor", nullptr));
        label->setText(QCoreApplication::translate("TowerEditor", "Existing Towers", nullptr));
        addTowerButton->setText(QCoreApplication::translate("TowerEditor", "+ Add New Tower", nullptr));
        towerPreview->setStyleSheet(QCoreApplication::translate("TowerEditor", "border: 2px dashed #888; background: #333; color: #aaa;", nullptr));
        towerPreview->setText(QCoreApplication::translate("TowerEditor", "No preview", nullptr));
        chooseTextureButton->setText(QCoreApplication::translate("TowerEditor", "Choose Texture...", nullptr));
        addNextTowerButton->setText(QCoreApplication::translate("TowerEditor", "Add next tower", nullptr));
        removeNextTowerButton->setText(QCoreApplication::translate("TowerEditor", "Remove next tower", nullptr));
        projectileSettings->setText(QCoreApplication::translate("TowerEditor", "Projectile settings", nullptr));
        saveButton->setText(QCoreApplication::translate("TowerEditor", "Save", nullptr));
        deleteButton->setStyleSheet(QCoreApplication::translate("TowerEditor", "color: red;", nullptr));
        deleteButton->setText(QCoreApplication::translate("TowerEditor", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TowerEditor: public Ui_TowerEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOWEREDITOR_H
