#include "hatchmessenger.h"

#include <QDebug>
#include <QTimer>
#include <QVariant>
#include <QThread>

HatchMessenger::HatchMessenger(QObject *parent) :
    QObject(parent)
{
    client = new QTcpSocket(this);



    connect(client, &QTcpSocket::readyRead, this, &HatchMessenger::recvMessage);
    connect(client, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(client, &QAbstractSocket::stateChanged,this, &HatchMessenger::stateChanged);

    connect(client, &QTcpSocket::disconnected, this, &HatchMessenger::connectToHost);

    connect(client, &QAbstractSocket::connected, [](){
        qDebug() << "im connected";
    });

    connectToHost();
}

void HatchMessenger::recvMessage()
{

}

void HatchMessenger::connectToHost()
{
    qDebug() << "begin";
    QVariant va;
    client->setSocketOption(QAbstractSocket::KeepAliveOption, va);
    client->abort();
    client->close();
    client->connectToHost("192.168.0.115", 9888);



    if( client->waitForConnected() )
        qDebug() << "connected";
    else
    {
        QThread::sleep(3);
        emit client->disconnected();
    }



}


void HatchMessenger::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "status changes to " << socketState;
    //if( socketState == QAbstractSocket::UnconnectedState )
    //    connectToHost();
}

void HatchMessenger::connectionError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;


   /* client->abort();
    client->close();
    qDebug() << client->state() << client->isValid();


    if( socketError == QAbstractSocket::ConnectionRefusedError )
    {
        QThread::sleep(3);
        //emit client->disconnected();
        connectToHost();
    }
    */

}
