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
    void sendMessage4004(const QString& pow, const QString& vesselRef, const QString& craneId, const QString& bundleId);
    //void sendMessage4008();
    void sendMessage4069();
    void sendMessage4067(const QString &start, const QString &end);

private:
    QTcpSocket* client;
    QList<QByteArray> outlist;

    QList<QByteArray> inList;
    QByteArray tmpData;

private:


    void initSendList();


    QList<MessageReader*> readerList;
    void initReaderList();

    void recvMessage();

    void readHeader(int *len, int *type);
    void readHeader(QByteArray* data , int *len, int *type);
    int readBody( QByteArray* data, int len, int type );
    int readBody(int len , int type);

    bool splitMessage(QByteArray* data);

    void stateChanged(QAbstractSocket::SocketState socketState);

    int loadShipData( CShipData *pShipData,const QString& shipFile );
    QString getShipFileName( const QString& vesselClass );

    void loadSlotGroups();


};

#endif // HATCHMESSENGER_H
