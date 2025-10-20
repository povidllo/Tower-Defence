#ifndef EDITOR_PROJECTCREATIONDIALOG_H
#define EDITOR_PROJECTCREATIONDIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE

namespace Ui {
    class ProjectCreationDialog;
}

QT_END_NAMESPACE

class ProjectCreationDialog : public QDialog {
    Q_OBJECT

public:
    explicit ProjectCreationDialog(QWidget *parent = nullptr);

    ~ProjectCreationDialog() override;

private:
    Ui::ProjectCreationDialog *ui;

private slots:
    void onBrowseLocationButtonClicked();
    void onCancelButtonClicked();
    void onCreateButtonClicked();

signals:
    void projectCreationSignal(const QString &path, const QString &name);
};


#endif //EDITOR_PROJECTCREATIONDIALOG_H