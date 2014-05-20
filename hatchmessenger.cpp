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


    connect(client, &QAbstractSocket::connected, [=](){

        sendMessage4000();
        //sendHeartBeat();
    });

    connectToHost();
    //QByteArray data = buildPackage(4000,1,"hello");

    //outlist.push_back(data);
    //initSendList();

}

void HatchMessenger::recvMessage()
{
    QByteArray data = client->readAll();
    QString str(data);
    qDebug() << str;
}

void HatchMessenger::connectToHost()
{
    qDebug() << "begin";
    QVariant va;
    client->setSocketOption(QAbstractSocket::KeepAliveOption, va);
    client->abort();
    client->close();
    client->connectToHost("192.168.1.13", 5555);



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
}

void HatchMessenger::connectionError(QAbstractSocket::SocketError socketError)
{
    qDebug() << socketError;
}

QByteArray HatchMessenger::buildPackage(quint16 msgType, quint16 msgId, const char *msg)
{
    EC_HEADER echeader={0,0,0,0};
    echeader.load_len = static_cast<quint16>(strlen(msg));
    echeader.msgType = msgType;
    memcpy(&(echeader.msgId),&msgId,sizeof(quint16));
    echeader.sevId = 0;
    quint32 len = echeader.load_len+sizeof(EC_HEADER)+1;

    quint8* pPack = new quint8[len];
    memcpy(pPack,&echeader,sizeof(EC_HEADER));
    memcpy(pPack+sizeof(EC_HEADER),msg,echeader.load_len);
    memset(pPack+len-1,0xff,1);

    QByteArray ba;
    QDataStream stream(&ba,QIODevice::WriteOnly);

    for(quint32 i = 0; i < len; i++)
    {
        stream << pPack[i];
    }

    //qDebug() <<  client->write(ba);

    delete[] pPack;
    pPack = NULL;

    return ba;
}


void HatchMessenger::initSendList()
{
    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    connect(timer, &QTimer::timeout,[=](){
        if( !outlist.isEmpty() )
        {
            qDebug() << outlist.first();
            outlist.pop_front();
        }
        QByteArray data = buildPackage(4000,1,"hello");

        outlist.push_back(data);
    });

}

void HatchMessenger::sendMessage(quint16 msgType, const QString &msg,ENUM_SEND_MODE sendmode)
{
    qDebug() << "Sending msg: " << msg;
    QByteArray data = buildPackage(msgType,1,msg.toStdString().c_str());
    client->write(data);
}

void HatchMessenger::sendHeartBeat()
{
    QString rdtId = "rh01";
    QString userId = "123";
    QString msg = rdtId.toUpper() + "*3*" + userId.toUpper() + "*N*0*nogps*";


    QTimer* timer = new QTimer;
    timer->setInterval(3000);
    timer->start();
    connect(timer, &QTimer::timeout,[=](){
        static int heartbeatno = 0;
        qDebug() << "send heartbeat no." << ++heartbeatno;
        sendMessage(ENUM_QUERY_RDT_STATUS_4002,msg);
    });

}

void HatchMessenger::sendMessage4000()
{
    QString rdtId = "t01";
    QString msg = "T01*1*0*";

    sendMessage(ENUM_QUERY_RDT_PARAM_4000,msg);
}
