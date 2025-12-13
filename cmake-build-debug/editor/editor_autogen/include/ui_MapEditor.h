/********************************************************************************
** Form generated from reading UI file 'MapEditor.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAPEDITOR_H
#define UI_MAPEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MapEditor
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *mapSelectorGroup;
    QHBoxLayout *hboxLayout;
    QComboBox *mapComboBox;
    QPushButton *addMapButton;
    QPushButton *deleteMapButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *mainHorizontalLayout;
    QWidget *leftPanel;
    QVBoxLayout *leftPanelLayout;
    QListWidget *modeList;
    QStackedWidget *editorStack;
    QWidget *tilesEditorPage;
    QVBoxLayout *vboxLayout;
    QGroupBox *texturesGroup;
    QVBoxLayout *vboxLayout1;
    QListWidget *textureList;
    QHBoxLayout *hboxLayout1;
    QPushButton *addTextureButton;
    QPushButton *deleteTextureButton;
    QWidget *wavesEditorPage;
    QVBoxLayout *vboxLayout2;
    QGroupBox *wavesGroup;
    QVBoxLayout *vboxLayout3;
    QListWidget *wavesList;
    QHBoxLayout *hboxLayout2;
    QPushButton *addWaveButton;
    QPushButton *editWaveButton;
    QPushButton *deleteWaveButton;
    QLabel *waveInfoLabel;
    QPushButton *editCurrentWavePathButton;
    QWidget *spotsEditorPage;
    QVBoxLayout *verticalLayout_3;
    QListWidget *spotsList;
    QHBoxLayout *horizontalLayout;
    QPushButton *editSpotButton;
    QPushButton *removeSpotButton;
    QPushButton *addSpotButton;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *propertiesForm;
    QPushButton *saveSettingsButton;
    QSpacerItem *verticalSpacer;
    QGraphicsView *mapView;

    void setupUi(QWidget *MapEditor)
    {
        if (MapEditor->objectName().isEmpty())
            MapEditor->setObjectName("MapEditor");
        MapEditor->resize(1000, 600);
        MapEditor->setMinimumSize(QSize(1000, 600));
        verticalLayout = new QVBoxLayout(MapEditor);
        verticalLayout->setObjectName("verticalLayout");
        mapSelectorGroup = new QGroupBox(MapEditor);
        mapSelectorGroup->setObjectName("mapSelectorGroup");
        hboxLayout = new QHBoxLayout(mapSelectorGroup);
        hboxLayout->setObjectName("hboxLayout");
        mapComboBox = new QComboBox(mapSelectorGroup);
        mapComboBox->setObjectName("mapComboBox");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mapComboBox->sizePolicy().hasHeightForWidth());
        mapComboBox->setSizePolicy(sizePolicy);
        mapComboBox->setMinimumSize(QSize(0, 0));

        hboxLayout->addWidget(mapComboBox);

        addMapButton = new QPushButton(mapSelectorGroup);
        addMapButton->setObjectName("addMapButton");

        hboxLayout->addWidget(addMapButton);

        deleteMapButton = new QPushButton(mapSelectorGroup);
        deleteMapButton->setObjectName("deleteMapButton");

        hboxLayout->addWidget(deleteMapButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        hboxLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(mapSelectorGroup);

        mainHorizontalLayout = new QHBoxLayout();
        mainHorizontalLayout->setSpacing(10);
        mainHorizontalLayout->setObjectName("mainHorizontalLayout");
        leftPanel = new QWidget(MapEditor);
        leftPanel->setObjectName("leftPanel");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(leftPanel->sizePolicy().hasHeightForWidth());
        leftPanel->setSizePolicy(sizePolicy1);
        leftPanel->setMinimumSize(QSize(100, 0));
        leftPanelLayout = new QVBoxLayout(leftPanel);
        leftPanelLayout->setSpacing(8);
        leftPanelLayout->setContentsMargins(0, 0, 0, 0);
        leftPanelLayout->setObjectName("leftPanelLayout");
        modeList = new QListWidget(leftPanel);
        QIcon icon;
        icon.addFile(QString::fromUtf8("\n"
"                                                    :/icons/tiles.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(modeList);
        __qlistwidgetitem->setIcon(icon);
        QIcon icon1;
        icon1.addFile(QString::fromUtf8("\n"
"                                                    :/icons/wave.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        QListWidgetItem *__qlistwidgetitem1 = new QListWidgetItem(modeList);
        __qlistwidgetitem1->setIcon(icon1);
        new QListWidgetItem(modeList);
        new QListWidgetItem(modeList);
        modeList->setObjectName("modeList");
        modeList->setIconSize(QSize(64, 64));
        modeList->setFlow(QListView::Flow::TopToBottom);
        modeList->setViewMode(QListView::ViewMode::IconMode);

        leftPanelLayout->addWidget(modeList);

        editorStack = new QStackedWidget(leftPanel);
        editorStack->setObjectName("editorStack");
        tilesEditorPage = new QWidget();
        tilesEditorPage->setObjectName("tilesEditorPage");
        vboxLayout = new QVBoxLayout(tilesEditorPage);
        vboxLayout->setObjectName("vboxLayout");
        texturesGroup = new QGroupBox(tilesEditorPage);
        texturesGroup->setObjectName("texturesGroup");
        vboxLayout1 = new QVBoxLayout(texturesGroup);
        vboxLayout1->setObjectName("vboxLayout1");
        textureList = new QListWidget(texturesGroup);
        textureList->setObjectName("textureList");
        textureList->setIconSize(QSize(48, 48));
        textureList->setFlow(QListView::Flow::LeftToRight);
        textureList->setResizeMode(QListView::ResizeMode::Adjust);
        textureList->setViewMode(QListView::ViewMode::IconMode);
        textureList->setProperty("wrapping", QVariant(true));

        vboxLayout1->addWidget(textureList);

        hboxLayout1 = new QHBoxLayout();
        hboxLayout1->setObjectName("hboxLayout1");
        addTextureButton = new QPushButton(texturesGroup);
        addTextureButton->setObjectName("addTextureButton");

        hboxLayout1->addWidget(addTextureButton);

        deleteTextureButton = new QPushButton(texturesGroup);
        deleteTextureButton->setObjectName("deleteTextureButton");

        hboxLayout1->addWidget(deleteTextureButton);


        vboxLayout1->addLayout(hboxLayout1);


        vboxLayout->addWidget(texturesGroup);

        editorStack->addWidget(tilesEditorPage);
        wavesEditorPage = new QWidget();
        wavesEditorPage->setObjectName("wavesEditorPage");
        vboxLayout2 = new QVBoxLayout(wavesEditorPage);
        vboxLayout2->setObjectName("vboxLayout2");
        wavesGroup = new QGroupBox(wavesEditorPage);
        wavesGroup->setObjectName("wavesGroup");
        vboxLayout3 = new QVBoxLayout(wavesGroup);
        vboxLayout3->setObjectName("vboxLayout3");
        wavesList = new QListWidget(wavesGroup);
        wavesList->setObjectName("wavesList");

        vboxLayout3->addWidget(wavesList);

        hboxLayout2 = new QHBoxLayout();
        hboxLayout2->setObjectName("hboxLayout2");
        addWaveButton = new QPushButton(wavesGroup);
        addWaveButton->setObjectName("addWaveButton");

        hboxLayout2->addWidget(addWaveButton);

        editWaveButton = new QPushButton(wavesGroup);
        editWaveButton->setObjectName("editWaveButton");

        hboxLayout2->addWidget(editWaveButton);

        deleteWaveButton = new QPushButton(wavesGroup);
        deleteWaveButton->setObjectName("deleteWaveButton");

        hboxLayout2->addWidget(deleteWaveButton);


        vboxLayout3->addLayout(hboxLayout2);

        waveInfoLabel = new QLabel(wavesGroup);
        waveInfoLabel->setObjectName("waveInfoLabel");
        waveInfoLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        vboxLayout3->addWidget(waveInfoLabel);

        editCurrentWavePathButton = new QPushButton(wavesGroup);
        editCurrentWavePathButton->setObjectName("editCurrentWavePathButton");

        vboxLayout3->addWidget(editCurrentWavePathButton);


        vboxLayout2->addWidget(wavesGroup);

        editorStack->addWidget(wavesEditorPage);
        spotsEditorPage = new QWidget();
        spotsEditorPage->setObjectName("spotsEditorPage");
        spotsEditorPage->setMinimumSize(QSize(337, 0));
        verticalLayout_3 = new QVBoxLayout(spotsEditorPage);
        verticalLayout_3->setObjectName("verticalLayout_3");
        spotsList = new QListWidget(spotsEditorPage);
        spotsList->setObjectName("spotsList");

        verticalLayout_3->addWidget(spotsList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        editSpotButton = new QPushButton(spotsEditorPage);
        editSpotButton->setObjectName("editSpotButton");

        horizontalLayout->addWidget(editSpotButton);

        removeSpotButton = new QPushButton(spotsEditorPage);
        removeSpotButton->setObjectName("removeSpotButton");

        horizontalLayout->addWidget(removeSpotButton);


        verticalLayout_3->addLayout(horizontalLayout);

        addSpotButton = new QPushButton(spotsEditorPage);
        addSpotButton->setObjectName("addSpotButton");

        verticalLayout_3->addWidget(addSpotButton);

        editorStack->addWidget(spotsEditorPage);
        page = new QWidget();
        page->setObjectName("page");
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setObjectName("verticalLayout_2");
        propertiesForm = new QFormLayout();
        propertiesForm->setObjectName("propertiesForm");

        verticalLayout_2->addLayout(propertiesForm);

        saveSettingsButton = new QPushButton(page);
        saveSettingsButton->setObjectName("saveSettingsButton");

        verticalLayout_2->addWidget(saveSettingsButton);

        editorStack->addWidget(page);

        leftPanelLayout->addWidget(editorStack);

        verticalSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        leftPanelLayout->addItem(verticalSpacer);


        mainHorizontalLayout->addWidget(leftPanel);

        mapView = new QGraphicsView(MapEditor);
        mapView->setObjectName("mapView");
        QSizePolicy sizePolicy2(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(mapView->sizePolicy().hasHeightForWidth());
        mapView->setSizePolicy(sizePolicy2);
        mapView->setMinimumSize(QSize(300, 0));

        mainHorizontalLayout->addWidget(mapView);


        verticalLayout->addLayout(mainHorizontalLayout);


        retranslateUi(MapEditor);

        editorStack->setCurrentIndex(3);


        QMetaObject::connectSlotsByName(MapEditor);
    } // setupUi

    void retranslateUi(QWidget *MapEditor)
    {
        MapEditor->setWindowTitle(QCoreApplication::translate("MapEditor", "Map Editor", nullptr));
        mapSelectorGroup->setTitle(QCoreApplication::translate("MapEditor", "Current Map", nullptr));
        addMapButton->setText(QCoreApplication::translate("MapEditor", "+", nullptr));
        deleteMapButton->setText(QCoreApplication::translate("MapEditor", "\342\210\222", nullptr));

        const bool __sortingEnabled = modeList->isSortingEnabled();
        modeList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = modeList->item(0);
        ___qlistwidgetitem->setText(QCoreApplication::translate("MapEditor", "Tiles", nullptr));
        QListWidgetItem *___qlistwidgetitem1 = modeList->item(1);
        ___qlistwidgetitem1->setText(QCoreApplication::translate("MapEditor", "Waves & Paths", nullptr));
        QListWidgetItem *___qlistwidgetitem2 = modeList->item(2);
        ___qlistwidgetitem2->setText(QCoreApplication::translate("MapEditor", "Spots", nullptr));
        QListWidgetItem *___qlistwidgetitem3 = modeList->item(3);
        ___qlistwidgetitem3->setText(QCoreApplication::translate("MapEditor", "Settings", nullptr));
        modeList->setSortingEnabled(__sortingEnabled);

        texturesGroup->setTitle(QCoreApplication::translate("MapEditor", "Textures", nullptr));
        addTextureButton->setText(QCoreApplication::translate("MapEditor", "Add", nullptr));
        deleteTextureButton->setText(QCoreApplication::translate("MapEditor", "Delete", nullptr));
        wavesGroup->setTitle(QCoreApplication::translate("MapEditor", "Waves", nullptr));
        addWaveButton->setText(QCoreApplication::translate("MapEditor", "Add Wave", nullptr));
        editWaveButton->setText(QCoreApplication::translate("MapEditor", "Edit wave", nullptr));
        deleteWaveButton->setText(QCoreApplication::translate("MapEditor", "Delete", nullptr));
        waveInfoLabel->setText(QCoreApplication::translate("MapEditor", "Select a wave to edit its path and\n"
"                                                                            enemies\n"
"                                                                        ", nullptr));
        editCurrentWavePathButton->setText(QCoreApplication::translate("MapEditor", "Edit Path for Selected Wave", nullptr));
        editSpotButton->setText(QCoreApplication::translate("MapEditor", "Edit spot", nullptr));
        removeSpotButton->setText(QCoreApplication::translate("MapEditor", "Remove spot", nullptr));
        addSpotButton->setText(QCoreApplication::translate("MapEditor", "Add spot", nullptr));
        saveSettingsButton->setText(QCoreApplication::translate("MapEditor", "Save settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MapEditor: public Ui_MapEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAPEDITOR_H
