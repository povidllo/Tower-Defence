#ifndef EDITOR_MAINWINDOW_H
#define EDITOR_MAINWINDOW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE

namespace Ui {
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(std::string path, std::string name, QWidget *parent = nullptr);

    ~MainWindow() override;

private:
    // std::shared_ptr<ProjectEditor> project;
    Ui::MainWindow *ui;
};


#endif //EDITOR_MAINWINDOW_H