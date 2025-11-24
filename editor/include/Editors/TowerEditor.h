#ifndef TOWERDEFENCE_TOWEREDITOR_H
#define TOWERDEFENCE_TOWEREDITOR_H

#include <memory>
#include <QListWidgetItem>
#include "TowerController.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class TowerEditor;
}

QT_END_NAMESPACE

class TowerEditor : public QWidget {
    Q_OBJECT

public:
    explicit TowerEditor(const std::shared_ptr<TowerController> &towerController, QWidget *parent = nullptr);

    ~TowerEditor() override;

private slots:
    void addTower() const;

    void onItemClicked(const QListWidgetItem *item);

    void onSaveButtonClicked();

    void onDeleteButtonClicked();

    void onChooseTextureButtonClicked();

private:
    void updateTowerList() const;

    void clearPropertiesForm();

    void fillPropertiesForm(const std::shared_ptr<TowerSample> &tower);

    Ui::TowerEditor *ui;
    std::shared_ptr<TowerController> towerController;

    QMap<QString, QWidget *> m_propertyEditors;

    std::vector<std::string> doNotShowThisFields{"x", "y"};

    void showTowerTexturePreview(const std::string &path) const;
};

#endif // TOWERDEFENCE_TOWEREDITOR_H
