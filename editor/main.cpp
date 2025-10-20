#include <QApplication>
#include "ProjectSelectorDialog.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ProjectSelectorDialog dlg;
    dlg.show();

    return app.exec();
}
