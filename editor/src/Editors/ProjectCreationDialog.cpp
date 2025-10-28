#include "../../include/Editors/ProjectCreationDialog.h"
#include "ui_ProjectCreationDialog.h"
#include <QFileDialog>
#include <filesystem>



ProjectCreationDialog::ProjectCreationDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ProjectCreationDialog) {
    ui->setupUi(this);

    connect(ui->BrowseLocationButton, &QPushButton::clicked, this, &ProjectCreationDialog::onBrowseLocationButtonClicked);
    connect(ui->CancelButton, &QPushButton::clicked, this, &ProjectCreationDialog::onCancelButtonClicked);
    connect(ui->CreateButton, &QPushButton::clicked, this, &ProjectCreationDialog::onCreateButtonClicked);
}

ProjectCreationDialog::~ProjectCreationDialog() {
    delete ui;
}

void ProjectCreationDialog::onBrowseLocationButtonClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select New Project Directory");
    if (!dir.isEmpty()) {
        ui->ProjectLocationLine->setText(dir);
    }
}

void ProjectCreationDialog::onCancelButtonClicked() {
    this->close();
}

void ProjectCreationDialog::onCreateButtonClicked() {
    std::string basePath = ui->ProjectLocationLine->text().toStdString();
    std::string projectName = ui->ProjectNameLine->text().toStdString();
    if (!ui->ProjectLocationLine->text().isEmpty() && !ui->ProjectNameLine->text().isEmpty()) {
        std::filesystem::path path = basePath + "/" + projectName;
        std::filesystem::create_directory(path);
        this->close();
        emit projectCreationSignal(ui->ProjectLocationLine->text(),
                                   ui->ProjectNameLine->text());
    }
}