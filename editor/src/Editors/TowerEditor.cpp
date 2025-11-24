#include "TowerEditor.h"
#include "ui_TowerEditor.h"
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

TowerEditor::TowerEditor(const std::shared_ptr<TowerController> &towerController, QWidget *parent) : QWidget(parent),
    ui(new Ui::TowerEditor), towerController(towerController) {
    ui->setupUi(this);

    connect(ui->addTowerButton, &QPushButton::clicked, this, &TowerEditor::addTower);
    connect(ui->towerList, &QListWidget::itemClicked, this, &TowerEditor::onItemClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &TowerEditor::onSaveButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &TowerEditor::onDeleteButtonClicked);
    connect(ui->chooseTextureButton, &QPushButton::clicked, this, &TowerEditor::onChooseTextureButtonClicked);

    updateTowerList();
}

TowerEditor::~TowerEditor() {
    delete ui;
}

void TowerEditor::addTower() const {
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

void TowerEditor::onItemClicked(const QListWidgetItem *item) {
    const std::string towerName = item->text().toStdString();
    qDebug() << "tower name: " << towerName;
    qDebug() << "item clicked";
    towerController->setCurrentTower(towerName);

    auto currentTower = towerController->getCurrentTower();

    clearPropertiesForm();

    if (!currentTower) {
        ui->editorTitle->setText("Tower Properties");
        return;
    }

    ui->editorTitle->setText("Editing: " + QString::fromStdString(currentTower->getName()));

    fillPropertiesForm(currentTower);
}

void TowerEditor::onSaveButtonClicked() {
    const auto tower = towerController->getCurrentTower();
    if (!tower) return;

    nlohmann::json j;
    for (auto it = m_propertyEditors.constBegin(); it != m_propertyEditors.constEnd(); it++) {
        const QString &key = it.key();
        QWidget *widget = it.value();
        if (auto *edit = qobject_cast<QLineEdit *>(widget)) {
            j[key.toStdString()] = edit->text().toStdString();
        } else if (auto *spin = qobject_cast<QDoubleSpinBox *>(widget)) {
            j[key.toStdString()] = spin->value();
        } else if (auto *check = qobject_cast<QCheckBox *>(widget)) {
            j[key.toStdString()] = check->isChecked();
        }
    }
    if (tower->getName() != j["name"].get<std::string>()) {
        if (towerController->towerExists(j["name"].get<std::string>())) {
            QMessageBox::warning(this,
                                 "Warning",
                                 "Tower with this name has already exist",
                                 QMessageBox::Ok);
            return;
        }
    }
    tower->fromJson(j);

    ui->saveButton->setText("Saved");
    ui->saveButton->setEnabled(false);
    QTimer::singleShot(1500, this, [this]() {
        ui->saveButton->setText("Save");
        ui->saveButton->setEnabled(true);
    });

    auto towers = towerController->getTowerNames();
    for (const auto &name: towers) {
        qDebug() << "   tower name: " << name;
    }

    updateTowerList();
}

void TowerEditor::onDeleteButtonClicked() {
    const auto tower = towerController->getCurrentTower();
    if (!tower) return;

    towerController->removeTower(tower->getName());

    updateTowerList();
}

void TowerEditor::onChooseTextureButtonClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Choose Texture",
        QDir::currentPath(),
        ".png"
    );

    if (filePath.isEmpty()) {
        return;
    }
    try {
        towerController->setTowerTexture(filePath.toStdString());
        for (auto it = m_propertyEditors.constBegin(); it != m_propertyEditors.constEnd(); it++) {
            const QString &key = it.key();
            if (key == "towerTexturePath") {
                if (auto *edit = qobject_cast<QLineEdit *>(it.value())) {
                    edit->setText(filePath);
                }
                break;
            }
        }
        showTowerTexturePreview(filePath.toStdString());
    } catch (const std::exception &e) {
        qDebug() << "Error: " << e.what();
        return;
    }
}

void TowerEditor::updateTowerList() const {
    qDebug() << "Tower list updated";
    ui->towerList->clear();

    const auto towerNames = towerController->getTowerNames();

    for (const auto &name: towerNames) {
        ui->towerList->addItem(QString::fromStdString(name));
    }
}

void TowerEditor::clearPropertiesForm() {
    qDeleteAll(ui->propertiesForm->findChildren<QWidget *>("", Qt::FindDirectChildrenOnly));
    while (ui->propertiesForm->rowCount() > 0)
        ui->propertiesForm->removeRow(0);
    m_propertyEditors.clear();
}

void TowerEditor::fillPropertiesForm(const std::shared_ptr<TowerSample> &tower) {
    auto j = tower->toJson();

    for (auto &[key, value]: j.items()) {
        bool flag = false;
        for (auto &elem: doNotShowThisFields) {
            if (key == elem) {
                flag = true;
                break;
            }
        }
        if (flag) {
            continue;
        }

        if (key == "towerTexturePath") {
            if (!value.empty()) {
                showTowerTexturePreview(value);
            } else {
                ui->towerPreview->setText("No preview");
            }
        }

        QString fieldName = QString::fromStdString(key);
        QString label = fieldName;
        label[0] = label[0].toUpper();

        QWidget *editor = nullptr;

        if (value.is_string()) {
            auto *edit = new QLineEdit(QString::fromStdString(value.get<std::string>()), this);
            editor = edit;
        } else if (value.is_number_float() || value.is_number()) {
            auto *spin = new QDoubleSpinBox(this);
            spin->setRange(-1e6, 1e6);
            spin->setDecimals(3);
            spin->setValue(value.get<double>());
            editor = spin;
        } else if (value.is_boolean()) {
            auto *check = new QCheckBox(this);
            check->setChecked(value.get<bool>());
            editor = check;
        }

        if (editor) {
            editor->setObjectName(fieldName);
            ui->propertiesForm->addRow(label + ":", editor);
            m_propertyEditors[fieldName] = editor;
        }
    }
}

void TowerEditor::showTowerTexturePreview(const std::string &path) const {
    QPixmap pixmap(QString::fromStdString(path));
    ui->towerPreview->setPixmap(pixmap.scaled(
        ui->towerPreview->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
    ));
}
