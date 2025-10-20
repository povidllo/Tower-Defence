#ifndef EDITOR_PROJECTSELECTORDIALOG_H
#define EDITOR_PROJECTSELECTORDIALOG_H

#include <QWidget>
#include "MainWindow.h"

QT_BEGIN_NAMESPACE

namespace Ui {
    class ProjectSelectorDialog;
}

QT_END_NAMESPACE

class ProjectSelectorDialog : public QWidget {
    Q_OBJECT

public:
    explicit ProjectSelectorDialog(QWidget *parent = nullptr);

    ~ProjectSelectorDialog() override;


private:
    Ui::ProjectSelectorDialog *ui;

private slots:
    void onNewProjectButtonClicked();
    void onLoadProjectButtonClicked();
    void startProject(const QString &path, const QString &name);
};


#endif //EDITOR_PROJECTSELECTORDIALOG_H