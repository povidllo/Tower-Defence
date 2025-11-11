// You may need to build the project (run Qt uic code generator) to get "ui_TowerEditor.h" resolved

#include "TowerEditor.h"
#include "ui_TowerEditor.h"


TowerEditor::TowerEditor(const std::shared_ptr<TowerController> &towerController, QWidget *parent) : QWidget(parent),
    ui(new Ui::TowerEditor), towerController(towerController) {
    ui->setupUi(this);
    connect(ui->addTowerButton, &QPushButton::clicked, this, &TowerEditor::addTower);

    updateTowerList();
}

TowerEditor::~TowerEditor() {
    delete ui;
}

void TowerEditor::updateTowerList() {
    ui->towerList->clear();

    const auto towerNames = towerController->getTowerNames();

    for (const auto &name: towerNames) {
        ui->towerList->addItem(QString::fromStdString(name));
    }
}

void TowerEditor::addTower() {
    qDebug() << "opening tower editor";
    std::string baseName = "tower";
    int counter = 1;
    while (towerController->towerExists(baseName)) {
        baseName = "tower_" + std::to_string(counter);
        counter++;
    }
    towerController->addTower(baseName);
    updateTowerList();
}
