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
    QList<QQuickItem *> children = item->childItems();


    qDebug() << item->objectName();
    foreach( QQuickItem* cc, item->childItems() )
    {
        qDebug() << cc->objectName();
    }




    //QQuickView loginview(QUrl("/Users/lixu/Project/Qt/visualhatch/qml/visualhatch/PopupDialog.qml"));

    //QQuickItem* item2 = loginview.rootObject();

    QObject::connect(item, SIGNAL(message4000(QString)),msger, SLOT(sendMessage4000(QString)));
    QObject::connect(item, SIGNAL(message4001(QString, QString, QString)),
                                  msger, SLOT(sendMessage4001(QString,QString,QString)) );
    QObject::connect(item, SIGNAL(message4004(QString, QString, QString, QString)),
                                  msger, SLOT(sendmessage4004(QString,QString,QString,QString)));



    return app.exec();
}
