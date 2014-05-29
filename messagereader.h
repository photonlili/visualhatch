#ifndef MESSAGEREADER_H
#define MESSAGEREADER_H

#include <QObject>
#include <QByteArray>
#include "common.h"

class MessageReader : public QObject
{
    Q_OBJECT
public:
    explicit MessageReader(QObject *parent = 0);

    virtual void read(QByteArray* data) = 0;
    ENUM_MSG_TYPE getMsgType() {return m_type;} ;

signals:

public slots:

protected:
    ENUM_MSG_TYPE m_type;

};

#endif // MESSAGEREADER_H