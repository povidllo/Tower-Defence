#ifndef TOWERDEFENCE_TOWEREDITOR_H
#define TOWERDEFENCE_TOWEREDITOR_H

#include <QWidget>

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

    void updateTowerList();

private:
    Ui::TowerEditor *ui;
    std::shared_ptr<TowerController> towerController;

private slots:
    void addTower();
};


#endif //TOWERDEFENCE_TOWEREDITOR_H
