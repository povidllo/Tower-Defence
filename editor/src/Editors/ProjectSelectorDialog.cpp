#include "../../include/Editors/ProjectSelectorDialog.h"

#include <iostream>

#include "ui_ProjectSelectorDialog.h"
#include "../../include/Editors/ProjectCreationDialog.h"
#include <QFileDialog>
#include <QMessageBox>



ProjectSelectorDialog::ProjectSelectorDialog(QWidget *parent) : QWidget(parent), ui(new Ui::ProjectSelectorDialog) {
    ui->setupUi(this);

    connect(ui->NewProjectButton, &QPushButton::clicked, this, &ProjectSelectorDialog::onNewProjectButtonClicked);
    connect(ui->LoadProjectButton, &QPushButton::clicked, this, &ProjectSelectorDialog::onLoadProjectButtonClicked);
}

ProjectSelectorDialog::~ProjectSelectorDialog() {
    delete ui;
}

void ProjectSelectorDialog::onNewProjectButtonClicked()
{
    ProjectCreationDialog dlg(this);
    connect(&dlg, &ProjectCreationDialog::projectCreationSignal, this, &ProjectSelectorDialog::createProject);

    dlg.exec();
}

void ProjectSelectorDialog::onLoadProjectButtonClicked()
{
    QString directory = QFileDialog::getExistingDirectory(
        this,
        "Select Project Folder",
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
    );

    qDebug() << "Selected project folder:" << directory;

    loadProject(directory);
}



void ProjectSelectorDialog::createProject(const QString &path, const QString &name)
{
    qDebug() << "Project :" << name << "at" << path;
    try {
        const auto mainWindow = new ProjectEditor(path.toStdString(), name.toStdString());
        mainWindow->show();
        this->close();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        qDebug() << "Ooops, some mistakes" << path.toStdString();
    }

}

void ProjectSelectorDialog::loadProject(const QString &path) {
    qDebug() << "Project path :" << path;
    try {
        const auto mainWindow = new ProjectEditor(path.toStdString());
        mainWindow->show();
        this->close();
    } catch (const std::filesystem::filesystem_error &e) {
        std::cerr << e.what() << std::endl;
        qDebug() << "Ooops, some mistakes with file system " << path.toStdString();
    }
}