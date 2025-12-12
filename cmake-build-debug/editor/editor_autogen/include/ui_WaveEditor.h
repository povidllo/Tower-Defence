/********************************************************************************
** Form generated from reading UI file 'WaveEditor.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAVEEDITOR_H
#define UI_WAVEEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WaveEditor
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *waveNameEdit;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QListWidget *enemiesList;
    QHBoxLayout *horizontalLayout;
    QComboBox *enemyComboBox;
    QSpinBox *countSpinBox;
    QPushButton *addEnemyButton;
    QPushButton *removeEnemyButton;
    QFormLayout *propertiesForm;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *WaveEditor)
    {
        if (WaveEditor->objectName().isEmpty())
            WaveEditor->setObjectName("WaveEditor");
        WaveEditor->resize(500, 400);
        verticalLayout = new QVBoxLayout(WaveEditor);
        verticalLayout->setObjectName("verticalLayout");
        waveNameEdit = new QLineEdit(WaveEditor);
        waveNameEdit->setObjectName("waveNameEdit");

        verticalLayout->addWidget(waveNameEdit);

        groupBox = new QGroupBox(WaveEditor);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        enemiesList = new QListWidget(groupBox);
        enemiesList->setObjectName("enemiesList");

        verticalLayout_2->addWidget(enemiesList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        enemyComboBox = new QComboBox(groupBox);
        enemyComboBox->setObjectName("enemyComboBox");

        horizontalLayout->addWidget(enemyComboBox);

        countSpinBox = new QSpinBox(groupBox);
        countSpinBox->setObjectName("countSpinBox");
        countSpinBox->setMinimum(1);
        countSpinBox->setMaximum(999);
        countSpinBox->setValue(5);

        horizontalLayout->addWidget(countSpinBox);

        addEnemyButton = new QPushButton(groupBox);
        addEnemyButton->setObjectName("addEnemyButton");

        horizontalLayout->addWidget(addEnemyButton);

        removeEnemyButton = new QPushButton(groupBox);
        removeEnemyButton->setObjectName("removeEnemyButton");

        horizontalLayout->addWidget(removeEnemyButton);


        verticalLayout_2->addLayout(horizontalLayout);

        propertiesForm = new QFormLayout();
        propertiesForm->setObjectName("propertiesForm");

        verticalLayout_2->addLayout(propertiesForm);


        verticalLayout->addWidget(groupBox);

        buttonBox = new QDialogButtonBox(WaveEditor);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(WaveEditor);

        QMetaObject::connectSlotsByName(WaveEditor);
    } // setupUi

    void retranslateUi(QDialog *WaveEditor)
    {
        WaveEditor->setWindowTitle(QCoreApplication::translate("WaveEditor", "Edit Wave", nullptr));
        waveNameEdit->setPlaceholderText(QCoreApplication::translate("WaveEditor", "Wave name", nullptr));
        groupBox->setTitle(QCoreApplication::translate("WaveEditor", "Enemies in Wave", nullptr));
        addEnemyButton->setText(QCoreApplication::translate("WaveEditor", "Add", nullptr));
        removeEnemyButton->setText(QCoreApplication::translate("WaveEditor", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WaveEditor: public Ui_WaveEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAVEEDITOR_H
