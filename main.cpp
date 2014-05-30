#include <QtGui/QGuiApplication>
#include "qtquick2applicationviewer.h"
#include "hatchmessenger.h"
#include <QQuickItem>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QtQuick2ApplicationViewer viewer;
    viewer.setMainQmlFile(QStringLiteral("qml/visualhatch/main.qml"));
    viewer.showExpanded();

    HatchMessenger* msger = new HatchMessenger;

    //QQuickView view(("qml/visualhatch/main.qml"));
    //view.show();

    QQuickItem *item = viewer.rootObject();



    //QQuickView loginview(QUrl("/Users/lixu/Project/Qt/visualhatch/qml/visualhatch/PopupDialog.qml"));

    //QQuickItem* item2 = loginview.rootObject();

    QObject::connect(item, SIGNAL(qmlSignal(QString)),msger, SLOT(sendMessage4000(QString)));

    return app.exec();
}
