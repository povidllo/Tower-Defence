// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "MainWindow.h"
#include "ui_MainWindow.h"


MainWindow::MainWindow(std::string path, std::string name, QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    project = std::make_shared<ProjectEditor>(path, name);
    qDebug() << "created project";
}

MainWindow::~MainWindow() {
    delete ui;
}