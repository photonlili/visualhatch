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
    ENUM_MSG_TYPE getMsgType() {return m_type;}

signals:

public slots:

protected:
    ENUM_MSG_TYPE m_type;

};


//READ 4000 RETURN
class Reader4100 : public MessageReader
{
    Q_OBJECT
public:
    explicit Reader4100(QObject* parent = 0);
    void read(QByteArray *data);
};


///READ 4101 LOGIN RETURN
/// ENUM_QUERY_RDT_LOGIN_4001
class Reader4101 : public MessageReader
{
    Q_OBJECT
public:
    explicit Reader4101(QObject* parent = 0);
    void read(QByteArray *data);
};

class Reader4110 : public MessageReader
{
    Q_OBJECT
public:
    explicit Reader4110(QObject* parent = 0);
    void read(QByteArray *data);
};


/// Read 4130 ENUM_ANSWER_RDT_LOGIN_VESSEL_POW_INFO_4130
/// ENUM_ANSWER_RDT_LOGIN_VESSEL_POW_INFO_4130
class Reader4130 : public MessageReader
{
    Q_OBJECT
public:
    explicit Reader4130(QObject* parent = 0):MessageReader(parent){
        m_type = ENUM_ANSWER_RDT_LOGIN_VESSEL_POW_INFO_4130;
    }
    void read(QByteArray *data);
};

#endif // MESSAGEREADER_H
