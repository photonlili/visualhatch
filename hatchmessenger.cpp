#include "hatchmessenger.h"

#include <QDebug>

HatchMessenger::HatchMessenger(QObject *parent) :
    QObject(parent)
{
    client = new QTcpSocket(this);
    connectToHost();


    connect(client, &QTcpSocket::readyRead, this, &HatchMessenger::recvMessage);
}

void HatchMessenger::recvMessage()
{

}

void HatchMessenger::connectToHost()
{
    qDebug() << "begin";
    client->connectToHost("169.169.8.71", 9888);

    if( client->waitForConnected() )
        qDebug() << "connected";

    connect(client,&QAbstractSocket::stateChanged,
            this, &HatchMessenger::stateChanged);
}


void HatchMessenger::stateChanged(QAbstractSocket::SocketState socketState)
{
    qDebug() << "status changes to " << socketState;
}
