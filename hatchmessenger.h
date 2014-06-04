#ifndef HATCHMESSENGER_H
#define HATCHMESSENGER_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QByteArray>
#include "common.h"
#include "messagereader.h"
#include "ShipData.h"

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

    void sendHeartBeat(const QString &rdtId, const QString &userId);
    void sendMessage4000( const QString& rdtId ); //query rdt parameter
    void sendMessage4001(const QString& rdtId, const QString& userId, const QString& password);
    void sendmessage4004(const QString& pow, const QString& vesselRef, const QString& craneId, const QString& bundleId);
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

    int loadShipData( CShipData *pShipData,const QString& shipFile );
    QString getShipFileName( const QString& vesselClass );

    void loadSlotGroups();


};

#endif // HATCHMESSENGER_H
