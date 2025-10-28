#include "../../include/Editors/ProjectSelectorDialog.h"
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
    connect(&dlg, &ProjectCreationDialog::projectCreationSignal, this, &ProjectSelectorDialog::startProject);

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

    if (directory.isEmpty() || directory == "/") {
        return;
    }

    QString projectName = QDir(directory).dirName();

    qDebug() << "Selected project folder:" << directory;
    qDebug() << "Project name:" << projectName;

    startProject(directory, projectName);
}



void ProjectSelectorDialog::startProject(const QString &path, const QString &name)
{
    qDebug() << "Project :" << name << "at" << path;

    MainWindow *mainWindow = new MainWindow(path.toStdString(), name.toStdString());
    mainWindow->show();

    this->close();
}
