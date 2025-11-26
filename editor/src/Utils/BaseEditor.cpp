#include "BaseEditor.h"

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QTimer>
nlohmann::json BaseEditor::collectPropertiesToJson(const QMap<QString, QWidget *> &editors) {
	nlohmann::json j;

	for (auto it = editors.constBegin(); it != editors.constEnd(); ++it) {
		const QString &key = it.key();
		QWidget *widget = it.value();

		if (auto *edit = qobject_cast<QLineEdit *>(widget)) {
			j[key.toStdString()] = edit->text().toStdString();
		} else if (auto *spin = qobject_cast<QDoubleSpinBox *>(widget)) {
			j[key.toStdString()] = spin->value();
		} else if (auto *check = qobject_cast<QCheckBox *>(widget)) {
			j[key.toStdString()] = check->isChecked();
		}
	}

	return j;
}

void BaseEditor::flashSaveButton(QPushButton *saveButton) {
	saveButton->setText("Saved");
	saveButton->setEnabled(false);
	QTimer::singleShot(1500, saveButton->parent(), [saveButton]() {
		saveButton->setText("Save");
		saveButton->setEnabled(true);
	});
}

void BaseEditor::fillListWidget(QListWidget *list, const std::vector<std::string> &elements) {
	list->clear();
	for (const auto &elem: elements) {
		list->addItem(QString::fromStdString(elem));
	}
}
void BaseEditor::clearPropertiesForm(QFormLayout *propertiesForm, QMap<QString, QWidget *> &m_propertyEditors) {
	qDeleteAll(propertiesForm->findChildren<QWidget *>("", Qt::FindDirectChildrenOnly));
	while (propertiesForm->rowCount() > 0) {
		propertiesForm->removeRow(0);
	}
	m_propertyEditors.clear();
}
