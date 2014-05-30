#ifndef HATCHMESSENGER_H
#define HATCHMESSENGER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include "common.h"
#include "messagereader.h"

class HatchMessenger : public QObject
{
    Q_OBJECT
public:
    explicit HatchMessenger(QObject *parent = 0);

public:
    void connectToHost();
    void sendMessage(quint16 msgType, const QString &msg, ENUM_SEND_MODE sendmode=ENUM_NEED_ANSWER);

    static QByteArray buildPackage(quint16 msgType, quint16 msgId, const char *msg);

signals:

public slots:
    void connectionError(QAbstractSocket::SocketError socketError);

    void sendMessage4000( const QString& rdtId ); //login
    void sendMessage4069();
    void sendMessage4067();

private:
    QTcpSocket* client;
    QList<QByteArray> outlist;

private:


    void initSendList();


    QList<MessageReader*> readerList;
    void initReaderList();

    void recvMessage();
    bool splitMessage(QByteArray* data);

    void stateChanged(QAbstractSocket::SocketState socketState);

    void sendHeartBeat();

};

#endif // HATCHMESSENGER_H
