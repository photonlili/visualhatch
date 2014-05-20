#ifndef HATCHMESSENGER_H
#define HATCHMESSENGER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include "common.h"

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

private:
    QTcpSocket* client;
    QList<QByteArray> outlist;

private:


    void initSendList();

    void recvMessage();

    void stateChanged(QAbstractSocket::SocketState socketState);

    void sendHeartBeat();
    void sendMessage4000(); //login
};

#endif // HATCHMESSENGER_H
