#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "hatchmessenger.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/visualhatch/main.qml"));
    viewer.showExpanded();

    HatchMessenger* msger = new HatchMessenger;

    return app.exec();
}
