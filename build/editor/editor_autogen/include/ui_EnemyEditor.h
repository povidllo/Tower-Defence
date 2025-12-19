/********************************************************************************
** Form generated from reading UI file 'EnemyEditor.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ENEMYEDITOR_H
#define UI_ENEMYEDITOR_H

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

class Ui_EnemyEditor
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *leftLayout;
    QLabel *label;
    QListWidget *enemyList;
    QPushButton *addEnemyButton;
    QVBoxLayout *rightLayout;
    QLabel *editorTitle;
    QHBoxLayout *horizontalLayout_2;
    QLabel *enemyPreview;
    QPushButton *chooseTextureButton;
    QScrollArea *scrollArea;
    QWidget *scrollWidget;
    QVBoxLayout *editorLayout;
    QFormLayout *propertiesForm;
    QHBoxLayout *buttonLayout;
    QPushButton *saveButton;
    QPushButton *deleteButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *EnemyEditor)
    {
        if (EnemyEditor->objectName().isEmpty())
            EnemyEditor->setObjectName("EnemyEditor");
        EnemyEditor->resize(947, 603);
        horizontalLayout = new QHBoxLayout(EnemyEditor);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        leftLayout = new QVBoxLayout();
        leftLayout->setSpacing(0);
        leftLayout->setObjectName("leftLayout");
        label = new QLabel(EnemyEditor);
        label->setObjectName("label");
        QFont font;
        font.setBold(true);
        label->setFont(font);

        leftLayout->addWidget(label);

        enemyList = new QListWidget(EnemyEditor);
        enemyList->setObjectName("enemyList");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(enemyList->sizePolicy().hasHeightForWidth());
        enemyList->setSizePolicy(sizePolicy);

        leftLayout->addWidget(enemyList);

        addEnemyButton = new QPushButton(EnemyEditor);
        addEnemyButton->setObjectName("addEnemyButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        addEnemyButton->setIcon(icon);

        leftLayout->addWidget(addEnemyButton);


        horizontalLayout->addLayout(leftLayout);

        rightLayout = new QVBoxLayout();
        rightLayout->setSpacing(0);
        rightLayout->setObjectName("rightLayout");
        editorTitle = new QLabel(EnemyEditor);
        editorTitle->setObjectName("editorTitle");
        editorTitle->setFont(font);

        rightLayout->addWidget(editorTitle);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        enemyPreview = new QLabel(EnemyEditor);
        enemyPreview->setObjectName("enemyPreview");
        enemyPreview->setMinimumSize(QSize(150, 150));
        enemyPreview->setMaximumSize(QSize(200, 200));
        enemyPreview->setScaledContents(true);
        enemyPreview->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_2->addWidget(enemyPreview);

        chooseTextureButton = new QPushButton(EnemyEditor);
        chooseTextureButton->setObjectName("chooseTextureButton");

        horizontalLayout_2->addWidget(chooseTextureButton);


        rightLayout->addLayout(horizontalLayout_2);

        scrollArea = new QScrollArea(EnemyEditor);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollWidget = new QWidget();
        scrollWidget->setObjectName("scrollWidget");
        scrollWidget->setGeometry(QRect(0, 0, 465, 383));
        editorLayout = new QVBoxLayout(scrollWidget);
        editorLayout->setSpacing(0);
        editorLayout->setObjectName("editorLayout");
        propertiesForm = new QFormLayout();
        propertiesForm->setObjectName("propertiesForm");
        propertiesForm->setLabelAlignment(Qt::AlignmentFlag::AlignRight);
        propertiesForm->setFormAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignTop);

        editorLayout->addLayout(propertiesForm);

        scrollArea->setWidget(scrollWidget);

        rightLayout->addWidget(scrollArea);

        buttonLayout = new QHBoxLayout();
        buttonLayout->setObjectName("buttonLayout");
        saveButton = new QPushButton(EnemyEditor);
        saveButton->setObjectName("saveButton");

        buttonLayout->addWidget(saveButton);

        deleteButton = new QPushButton(EnemyEditor);
        deleteButton->setObjectName("deleteButton");

        buttonLayout->addWidget(deleteButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        buttonLayout->addItem(horizontalSpacer);


        rightLayout->addLayout(buttonLayout);


        horizontalLayout->addLayout(rightLayout);


        retranslateUi(EnemyEditor);

        saveButton->setDefault(true);


        QMetaObject::connectSlotsByName(EnemyEditor);
    } // setupUi

    void retranslateUi(QWidget *EnemyEditor)
    {
        EnemyEditor->setWindowTitle(QCoreApplication::translate("EnemyEditor", "Enemy Editor", nullptr));
        label->setText(QCoreApplication::translate("EnemyEditor", "Existing Enemies", nullptr));
        addEnemyButton->setText(QCoreApplication::translate("EnemyEditor", "+ Add New Enemy", nullptr));
        editorTitle->setText(QCoreApplication::translate("EnemyEditor", "Enemy Properties", nullptr));
        enemyPreview->setStyleSheet(QCoreApplication::translate("EnemyEditor", "border: 2px dashed #888; background: #333; color: #aaa;", nullptr));
        enemyPreview->setText(QCoreApplication::translate("EnemyEditor", "No preview", nullptr));
        chooseTextureButton->setText(QCoreApplication::translate("EnemyEditor", "Choose Texture...", nullptr));
        saveButton->setText(QCoreApplication::translate("EnemyEditor", "Save", nullptr));
        deleteButton->setStyleSheet(QCoreApplication::translate("EnemyEditor", "color: red;", nullptr));
        deleteButton->setText(QCoreApplication::translate("EnemyEditor", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EnemyEditor: public Ui_EnemyEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ENEMYEDITOR_H
