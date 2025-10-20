#include "ProjectSelectorDialog.h"
#include "ui_ProjectSelectorDialog.h"
#include "ProjectCreationDialog.h"
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
    connect(&dlg, &ProjectCreationDialog::projectCreationSignal, this, [this]() {
        this->close();
    });

    if (dlg.exec() == QDialog::Accepted) {

    }
}

void ProjectSelectorDialog::onLoadProjectButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Project File", "", "Tower Defence Project (*.tdproj)");
    if (!fileName.isEmpty()) {
        QMessageBox::information(this, "Load Project", "Загружен проект: " + fileName);
    }
}
