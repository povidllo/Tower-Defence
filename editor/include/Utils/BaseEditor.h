#ifndef TOWERDEFENCE_BASEEDITOR_H
#define TOWERDEFENCE_BASEEDITOR_H
#include <QFormLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QWidget>
#include <nlohmann/json.hpp>


class BaseEditor {
protected:
	static nlohmann::json collectPropertiesToJson(const QMap<QString, QWidget *> &editors);

	static void flashSaveButton(QPushButton *saveButton);

	static void fillListWidget(QListWidget *list, const std::vector<std::string> &elements);

	static void clearPropertiesForm(QFormLayout *propertiesForm, QMap<QString, QWidget *> &m_propertyEditors);
};


#endif // TOWERDEFENCE_BASEEDITOR_H
