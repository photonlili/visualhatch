#ifndef HATCHMESSENGER_H
#define HATCHMESSENGER_H

#include <QObject>
#include <QTcpSocket>

class HatchMessenger : public QObject
{
    Q_OBJECT
public:
    explicit HatchMessenger(QObject *parent = 0);

public:
    void connectToHost();

signals:

public slots:
    void connectionError(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket* client;

private:



    void recvMessage();

    void stateChanged(QAbstractSocket::SocketState socketState);

};

#endif // HATCHMESSENGER_H
